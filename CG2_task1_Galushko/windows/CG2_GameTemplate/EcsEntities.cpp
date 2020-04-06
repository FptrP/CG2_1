#include "EcsEntities.h"
#include "WeaponsCatalogue.h"
#include "Renderer.h"
#include "Game.h"
#include "AttackFunctions.h"
constexpr unsigned enemyUpdatePeriod = 4;

void Player::OnCreated()
{
    GetComp<CompParty>()->party = CompParty::ALLIANCE;

    int attackAreaTileId = game::gRenderer.AddTile(sf::Color(255, 0, 0, 128));
    auto eid = ecs::gEntityManager.CreateEntity<AttackArea>();
    ecs::IEntity *ent = ecs::gEntityManager.GetEntityByDesc(eid);
    ent->GetComp<CompDrawTileInfo>()->tileId = attackAreaTileId;
    ent->GetComp<CompDrawTileInfo>()->layer = CompDrawTileInfo::LAYER_EFFECTS;
    ent->GetComp<CompVisibility>()->visible = false;
    ent->GetComp<CompOwner>()->owner = this->GetDesc();

    GetComp<CompCurrentAttackArea>()->ent = eid;
}

void Player::OnDestroy()
{
    ecs::gEntityManager.DestroyEntity(GetComp<CompCurrentAttackArea>()->ent);
    auto desc = ecs::gEntityManager.CreateEntity<PlayerGrave>();
    auto ent = ecs::gEntityManager.GetEntityByDesc(desc);
    ent->GetComp<CompPos>()->v = GetComp<CompPos>()->v;
    auto tinf = ent->GetComp<CompDrawTileInfo>();
    tinf->layer = CompDrawTileInfo::LAYER_OBJECTS;
    tinf->tileId = game::gNamedTiles["grave"]; 
    game::gRenderer.GuiAddText(
        "deathmsg", CompDrawTileInfo::LAYER_OBJECTS,
        sf::Vector2i(WIDTH/2 - 100, HEIGHT/2), "YOU DIED", 40, sf::Color::Red);
}

void Player::OnUpdate()
{
    CompHealth *h = GetComp<CompHealth>();
    CompStamina *s = GetComp<CompStamina>();
    CompMana *m = GetComp<CompMana>();
    if (h->value < 100.0f)
        h->value = std::min(100.0f, h->value + 0.5f);
    if (s->value < 100.0f)
        s->value = std::min(100.0f, s->value + 0.5f);
    if (m->value < 100.0f)
        m->value = std::min(100.0f, m->value + 0.5f);
}

void Enemy::OnCreated()
{
    GetComp<CompParty>()->party = CompParty::ENEMIES;

    auto eid = ecs::gEntityManager.CreateEntity<AttackArea>();
    ecs::IEntity *area = ecs::gEntityManager.GetEntityByDesc(eid);

    area->GetComp<CompVisibility>()->visible = false;
    area->GetComp<CompOwner>()->owner = this->GetDesc();
    area->GetComp<CompActivity>()->active = false;
    area->GetComp<CompActivity>()->disableAfterOneTick = true;
    
    // 3x3 attack area.
    area->GetComp<CompPosPattern>()->positions.Init(3, 3, 1);

    GetComp<CompCurrentAttackArea>()->ent = eid;
    GetComp<CompCurrentAttackArea>()->shift = sf::Vector2i(-1, -1);
}

void Enemy::OnDestroy()
{
    ecs::gEntityManager.DestroyEntity(GetComp<CompCurrentAttackArea>()->ent);
}

void Enemy::OnUpdate()
{
    ++curTick;
    
    IEntity *area = ecs::gEntityManager.GetEntityByDesc(GetComp<CompCurrentAttackArea>()->ent);
    /*CompDamage *myDamage = GetComp<CompDamage>();
    area->GetComp<CompDamage>()->physDamage = myDamage->physDamage;
    area->GetComp<CompDamage>()->magicDamage = myDamage->magicDamage;*/
    if (!(curTick % enemyUpdatePeriod))
    {
        area->GetComp<CompActivity>()->active = true;
        auto visibility = area->GetComp<CompVisibility>();
        if (!GetComp<CompOwnedItems>())
        {
            //по какаим-то причинам враг не владеет оружием
            game::Weapon *w=game::wc.getWeapon(4);
            game::AttackFuncts::Attack(game::SIMPLE,w,this,area);
        }
        else
        {
            game::Weapon *w=game::wc.getWeapon(4);
            game::AttackFuncts::Attack(game::SIMPLE,w,this,area);
        }
        
    }
    else
    {
        area->GetComp<CompActivity>()->active = false;
        GetComp<CompMoveDir>()->v = sf::Vector2i(0, 0);
        return;
    }

    unsigned vr = GetComp<CompVisionRadius>()->value;

    sf::Vector2i dir(0, 0);
    sf::Vector2i pos = GetComp<CompPos>()->v;

    // Now enemies are just hunt for the player.
    // Now there is very stupid algorithm:
    // just go in the player direction.
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

    GetComp<CompMoveDir>()->v = dir;
}


StatusMessage::StatusMessage(const std::string& smsg, int lifetime, sf::Vector2i &pos, sf::Color color) :
    mcolor(color)
{
    GetComp<CompTimeToLive>()->ticks = lifetime;
    GetComp<CompPos>()->v = pos;
    msg = smsg;
}

void StatusMessage::OnCreated()
{
    GetComp<CompFloatingText>()->tid = game::gRenderer.AddText(
        msg, sf::Vector2f(GetComp<CompPos>()->v), 15, mcolor
    );
}

void StatusMessage::OnDestroy()
{
    game::gRenderer.RemoveText(GetComp<CompFloatingText>()->tid);
}

Missle::Missle(int tileid, const sf::Vector2i &pos, const sf::Vector2i &dir,
 const CompDamage & dmg)
{
    GetComp<CompDrawTileInfo>()->layer = CompDrawTileInfo::LAYER_EFFECTS;
    GetComp<CompDrawTileInfo>()->tileId = tileid;
    GetComp<CompPos>()->v = pos;
    GetComp<CompMoveDir>()->v = dir;
    *GetComp<CompDamage>() = dmg;
}
void Missle::OnDestroy() 
{
    sf::Vector2i pos = this->GetComp<CompPos>()->v;
    pos += this->GetComp<CompMoveDir>()->v;
    auto desc = ecs::gEntityManager.CreateEntity<AttackArea>();
    auto ent = ecs::gEntityManager.GetEntityByDesc(desc);
    game::PosPattern pattern;
    pattern.Init(5, 5, true);
    pattern.pos = pos;
    pattern.tileId = this->GetComp<CompDrawTileInfo>()->tileId;
    ent->GetComp<CompPosPattern>()->positions = pattern;
    ent->GetComp<CompActivity>()->active = true;
    ent->GetComp<CompDamage>()->physDamage = this->GetComp<CompDamage>()->physDamage;
    ent->GetComp<CompDamage>()->magicDamage = this->GetComp<CompDamage>()->magicDamage;
    game::AttackFuncts::Attack(game::POISONOUS,NULL,this,ent);
    ent->GetComp<CompVisibility>()->visible = true;
    ent->GetComp<CompDrawTileInfo>()->layer = 2;
    ent->GetComp<CompDrawTileInfo>()->tileId = pattern.tileId; 
    ent->GetComp<CompParty>()->party=this->GetComp<CompParty>()->party;
    ent->GetComp<CompOwner>()->owner=this->GetDesc();
}

FireBall::FireBall(const CompAnimInfo &anim, const sf::Vector2i &pos, const sf::Vector2i &dst, const CompDamage &dmg)
{
    *GetComp<CompAnimInfo>() = anim;
    GetComp<CompPos>()->v = pos;
    dstPos = dst;
    *GetComp<CompDamage>() = dmg;
    GetComp<CompMoveDir>()->v = sf::Vector2i(0, 0);
    GetComp<CompHealth>()->value = 1;
}

void FireBall::OnCreated()
{
    
}

void FireBall::OnUpdate() 
{
    auto dir = sf::Vector2i(0, 0);
    auto &pos = GetComp<CompPos>()->v;
    if(pos == dstPos){
        GetComp<CompMoveDir>()->v = sf::Vector2i(0, 0);
        GetComp<CompHealth>()->value = -1;
        return;
    }

    if(dstPos.x < pos.x)
        dir.x = -1;
    else if(dstPos.x > pos.x)
        dir.x = 1;
    if(dstPos.y < pos.y)
        dir.y = -1;
    else if(dstPos.y > pos.y)
        dir.y = 1;
    GetComp<CompMoveDir>()->v = dir;
}

void FireBall::OnDestroy()
{
    sf::Vector2i pos = this->GetComp<CompPos>()->v;
    auto desc = ecs::gEntityManager.CreateEntity<AttackArea>();
    auto ent = ecs::gEntityManager.GetEntityByDesc(desc);
    game::PosPattern pattern;
    pattern.Init(3, 3, true);
    pattern.pos = pos;
    ent->GetComp<CompPosPattern>()->positions = pattern;
    ent->GetComp<CompActivity>()->active = true;
    *ent->GetComp<CompDamage>()= *GetComp<CompDamage>();
    ent->GetComp<CompVisibility>()->visible = false;
    ent->GetComp<CompParty>()->party = GetComp<CompParty>()->party;
    ent->GetComp<CompOwner>()->owner = GetDesc();
    game::AttackFuncts::Attack(game::SIMPLE, nullptr, this, ent);
    
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            auto des = ecs::gEntityManager.CreateEntity<Decoration>(
                    -1, -1, -1, -1, 5, sf::Vector2i(pos.x + i, pos.y + j)
            );
            *ecs::gEntityManager.GetEntityByDesc(des)->GetComp<CompAnimInfo>() = 
                *GetComp<CompAnimInfo>();
        }
    }
}

void NPC::UpdateMap(IEntity *player)
{
    if(state != FINAL && state != KILL){
        auto ppos = player->GetComp<CompPos>()->v;
        auto tpos = ecs::gEntityManager.GetEntitiesByType(ecs::GetEntityTid<BossTriger>());
        if(tpos && tpos->size() && tpos->at(0)){
            auto pos = tpos->at(0)->GetComp<CompPos>()->v;
            //std::cout << pos.x << " " << pos.y << "\n";
            if(pos == ppos){ // need spawn 
                state = WAIT;
                auto lockS = ecs::gEntityManager.GetEntitiesByType(ecs::GetEntityTid<LockSpawn>());
                auto spawnPos = lockS->at(0)->GetComp<CompPos>()->v;
                auto d = ecs::gEntityManager.CreateEntity<Lock>();
                auto lock = ecs::gEntityManager.GetEntityByDesc(d);
                lock->GetComp<CompPos>()->v = spawnPos;
                auto dti = lock->GetComp<CompDrawTileInfo>();
                dti->layer = CompDrawTileInfo::LAYER_OBJECTS;
                dti->tileId = game::gNamedTiles["door"];
            }
        }
    }

    if(state == WAIT) {
        auto boss1_dead = true;
        auto boss2_dead = true;
        auto boss1 = ecs::gEntityManager.GetEntitiesByType(ecs::GetEntityTid<Charger>());
        auto boss2 = ecs::gEntityManager.GetEntitiesByType(ecs::GetEntityTid<FireDemon>());
        for(const auto& e : *boss1){
            if(e) boss1_dead = false;
        }
        for(const auto& e : *boss2){
            if(e) boss2_dead = false;
        }
        if(boss1_dead && boss2_dead){
            auto lock = ecs::gEntityManager.GetEntitiesByType(ecs::GetEntityTid<Lock>());
            if(lock && lock->size() && lock->at(0)){
                ecs::gEntityManager.DestroyEntity(lock->at(0)->GetDesc());
            }
            state = FINAL;
            ticks = 0;
        }
    }
}

void NPC::OnUpdate()
{
    ecs::Apply<TagPlayer>([&](IEntity *player)
    {
        UpdateMap(player);
        auto v = player->GetComp<CompPos>()->v - GetComp<CompPos>()->v;
        if(std::abs(v.x) + std::abs(v.y) <= GetComp<CompVisionRadius>()->value){
            if(ticks <= 0){
                std::string s;
                switch (state)
                {
                case GREET:
                    s = "Hello stanger!";
                    state = TREASURE;
                    ticks = 30;
                    ecs::gEntityManager.CreateEntity<StatusMessage>(s, ticks, GetComp<CompPos>()->v, sf::Color::White);
                    break;
                case TREASURE:
                    s = "You will find treasure in the North";
                    ticks = 30; 
                    ecs::gEntityManager.CreateEntity<StatusMessage>(s, ticks, GetComp<CompPos>()->v, sf::Color::White);
                    state = IDLE;
                    break;
                case WAIT:
                case IDLE: 
                    break;
                case FINAL:
                    s = "You escaped my trap!\n Now I kill you";
                    state = KILL;
                    ticks = 30;
                    ecs::gEntityManager.CreateEntity<StatusMessage>(s, ticks, GetComp<CompPos>()->v, sf::Color::White);
                    break;
                case KILL:
                    ecs::gEntityManager.DestroyEntity(player->GetDesc());
                    state = GREET;
                    break;
                }
            } else {
                ticks--;   
            }
        }
    });
}