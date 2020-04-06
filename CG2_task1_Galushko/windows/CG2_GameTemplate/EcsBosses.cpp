#include "EcsEntities.h"
#include "WeaponsCatalogue.h"
#include "Renderer.h"
#include "Game.h"
#include "AttackFunctions.h"

void Charger::OnCreated() {
    GetComp<CompParty>()->party = CompParty::ENEMIES;

    auto eid = ecs::gEntityManager.CreateEntity<AttackArea>();
    ecs::IEntity *area = ecs::gEntityManager.GetEntityByDesc(eid);

    area->GetComp<CompVisibility>()->visible = false;
    area->GetComp<CompOwner>()->owner = this->GetDesc();
    area->GetComp<CompActivity>()->active = false;
    area->GetComp<CompActivity>()->disableAfterOneTick = true;
    area->GetComp<CompDrawTileInfo>()->layer = CompDrawTileInfo::LAYER_EFFECTS;
    area->GetComp<CompDrawTileInfo>()->tileId = game::gRenderer.AddTile(sf::Color(0xffa50020));
    // 3x3 attack area.
    area->GetComp<CompPosPattern>()->positions.Init(3, 3, 1);

    GetComp<CompCurrentAttackArea>()->ent = eid;
    GetComp<CompCurrentAttackArea>()->shift = sf::Vector2i(-1, -1);
    
    weapon.physDamage = weapon.magicDamage = 20;
}

void Charger::OnUpdate() {
    ++curTick;
    IEntity *area = ecs::gEntityManager.GetEntityByDesc(GetComp<CompCurrentAttackArea>()->ent); 
    switch (state)
    {
    case IDLE:
        if(curTick >= idleTime) 
        {
            curTick = 0;
            state = PREPARATION;
            ecs::gEntityManager.CreateEntity<StatusMessage>(
                "DANGER!", prepTime, GetComp<CompPos>()->v, sf::Color::Red
            );
        }
        break;
    case PREPARATION:
        if(curTick >= prepTime) 
        {
            curTick = 0;
            area->GetComp<CompVisibility>()->visible = true;
            state = CHARGING;
        }
        break;
    case CHARGING:
        if(curTick >= chargeTime) 
        {
            curTick = 0;
            state = IDLE;
            GetComp<CompMoveDir>()->v = sf::Vector2i(0, 0);
            area->GetComp<CompActivity>()->active = false;
            area->GetComp<CompVisibility>()->visible = false;
            return;
        } 
    }

    if(state == CHARGING){
        area->GetComp<CompActivity>()->active = true;
        game::AttackFuncts::Attack(game::SIMPLE, &weapon, this, area);
        unsigned vr = GetComp<CompVisionRadius>()->value;

        sf::Vector2i dir(0, 0);
        sf::Vector2i pos = GetComp<CompPos>()->v;
        ecs::Apply<TagPlayer>([&](IEntity *player)
        {
            CompPos *playerPos = player->GetComp<CompPos>();
        
            int dx = abs(playerPos->v.x - pos.x);
            int dy = abs(playerPos->v.y - pos.y);
            if (std::max(dx, dy) > GetComp<CompVisionRadius>()->value)
                return;

            if (playerPos->v.x < pos.x)
                dir.x = -1;
            else if (playerPos->v.x > pos.x)
                dir.x = 1;
            if (playerPos->v.y < pos.y)
                dir.y = -1;
            else if (playerPos->v.y > pos.y)
                dir.y = 1;
        });
        if(curTick % 3)
            GetComp<CompMoveDir>()->v = dir;
    }
    
}

void Charger::OnDestroy() {
    ecs::gEntityManager.DestroyEntity(GetComp<CompCurrentAttackArea>()->ent);
}

void FireDemon::OnCreated(){
    GetComp<CompParty>()->party = CompParty::ENEMIES;
    GetComp<CompVisionRadius>()->value = 10;
    lastHealth = -1.f;

    fireballAnim.id = game::gNamedAnimations["fire_anim"];
    fireballAnim.frame_count = game::gRenderer.GetAnimFramesCount(fireballAnim.id);
    fireballAnim.frame = fireballAnim.ticks = 0;
    fireballAnim.update_ticks = 1;
    fireballAnim.layer = CompDrawTileInfo::LAYER_EFFECTS;

    teleportAnim.id = game::gNamedAnimations["teleport_anim"];
    teleportAnim.frame_count = game::gRenderer.GetAnimFramesCount(teleportAnim.id);
    teleportAnim.frame = teleportAnim.ticks = 0;
    teleportAnim.update_ticks = 1;
    teleportAnim.layer = CompDrawTileInfo::LAYER_EFFECTS;
    
}

void FireDemon::OnUpdate()
{
    curTick++;
    if(lastHealth < 0) lastHealth = GetComp<CompHealth>()->value;
    auto hp = GetComp<CompHealth>()->value;
    if(lastHealth > hp)
    {
        auto ents = ecs::gEntityManager.GetEntitiesByType(ecs::GetComponentTid<TeleportPoint>());
        int n = std::rand() % ents->size();
        auto lastPos = GetComp<CompPos>()->v;
        GetComp<CompPos>()->v = (*ents)[n]->GetComp<CompPos>()->v;
        ecs::gEntityManager.CreateEntity<Decoration>(teleportAnim, 2, lastPos);
        state = PREPARATION;
        curTick = 0;
        tpCount--;
        if(tpCount <= 0){
            lastHealth = GetComp<CompHealth>()->value;
            tpCount = 5;
        }
    }

    switch (state)
    {
    case IDLE:
        if(curTick >= idleTime) 
        {
            curTick = 0;
            state = PREPARATION;
            ecs::gEntityManager.CreateEntity<StatusMessage>(
                "DANGER!", prepTime, GetComp<CompPos>()->v, sf::Color::Black);
        }
        break;
    case PREPARATION:
        if(curTick >= prepTime) 
        {
            curTick = 0;
            state = SHOOTING;
        }
        break;
    case SHOOTING:
        if(curTick >= shootTime) 
        {
            curTick = 0;
            state = IDLE;
            return;
        } 
    }

    if(state == SHOOTING && (curTick % 2))
    {
        auto vision = GetComp<CompVisionRadius>()->value;
        auto demonpos = GetComp<CompPos>()->v;
        ecs::Apply<TagPlayer, CompPos>([&](IEntity *player)
        {
            auto ppos = player->GetComp<CompPos>()->v;
            if(std::abs(ppos.x - demonpos.x) + std::abs(ppos.y - demonpos.y) <= vision){
                auto desc = ecs::gEntityManager.CreateEntity<FireBall>(
                    fireballAnim, demonpos, ppos, *GetComp<CompDamage>()
                );
                *ecs::gEntityManager.GetEntityByDesc(desc)->GetComp<CompParty>() = *GetComp<CompParty>();
            }
        });
    }
}

void FireDemon::OnDestroy()
{

}