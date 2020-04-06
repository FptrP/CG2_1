#pragma once

#include "GameStructures.h"
#include "EcsEntities.h"
#include "Game.h"
#include "GameEffects.h"
namespace game
{
    enum
    {
        SIMPLE=1,
        MAGIC_BALL=2,
        POISONOUS=3,
        POISONOUS_WEAK=4,
        POISON_SPELL=5,
        FIERY=6,
        CURSE=7
    };
     class AttackFuncts
    {
        public:
        static bool SimpleAttack(const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
        static bool MagicBalls(const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
        static bool PoisonousAttack(const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
        static bool Attack(int attack_type,const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
        static bool Poison(const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
        static bool FieryAttack(const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
        static bool SpellCurse(const game::Weapon *w,ecs::IEntity *attacker,ecs::IEntity *area);
    };
}