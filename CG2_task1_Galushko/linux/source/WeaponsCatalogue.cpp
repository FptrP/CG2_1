#include "WeaponsCatalogue.h"
#include "AttackFunctions.h"
#include <string.h>
namespace game
{
    WeaponsCatalogue wc;
    stl::vector<Weapon> ws;
static bool swordPattern[3 * 2] = {
    0, 0,
    1, 1,
    0, 0
};
static bool daggerPattern[3 * 2] = {
    0, 0,
    1, 0,
    0, 0
};
static bool clubPattern[3 * 2] = {
    1, 0,
    0, 1,
    1, 0
};
static bool longClubPattern[3 * 3] = {
    0, 0, 1,
    1, 1, 1, 
    0, 0, 1
};

static bool desolatorPattern[3 * 3] = {
    1, 1, 1,
    1, 0, 1,
    1, 1, 1
};
static bool rangedWeaponPattern[3 * 8] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0
};
    void WeaponsCatalogue::Init(int type)
    {
        Weapon w;
        int id;
        bool *pattern;
        int patsize;
        /**/
        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 4.0f;
        w.manaConsumption = 8.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 15.0f;
        w.attack_speed=1.0f;
        w.shift = 0;
        pattern=swordPattern;
        patsize = sizeof(swordPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_215.png");
        w.name="Sword";
        w.description="An ordinary sword made by an ordinary master";
        w.attack_type=SIMPLE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 15.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 15.0f;
        w.attack_speed = 0.6f;
        w.shift = 0;
        pattern=clubPattern;
        patsize = sizeof(clubPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_190.png");
        w.name="Club";
        w.description="Just big stick made of oak wood.";
        w.attack_type=SIMPLE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 3);
        w.staminaConsumption = 30.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 20.0f;
        w.attack_speed = 0.4f;
        w.shift = 0;
        pattern=longClubPattern;
        patsize = sizeof(longClubPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_189.png");
        w.name="Big club";
        w.description="Long and very heavy";
        w.attack_type=SIMPLE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 10.0f;
        w.manaConsumption = 20.0f;
        w.magicDamage = 15.0f;
        w.physDamage = 10.0f;
        w.attack_speed = 0.8f;
        w.shift = 0;
        pattern=swordPattern;
        patsize = sizeof(swordPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_173.png");
        w.name="Fire axe";
        w.description="Magic axe sets enemies on fire";
        w.attack_type=FIERY;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 3.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 5.0f;
        w.attack_speed = 1.67f;
        w.shift = 0;
        pattern=daggerPattern;
        patsize = sizeof(daggerPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_206.png");
        w.name="Dagger";
        w.description="Small but sharp";
        w.attack_type=SIMPLE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 3.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 5.0f;
        w.attack_speed = 1.67f;
        w.shift = 0;
        pattern=daggerPattern;
        patsize = sizeof(daggerPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_206.png");
        w.name="Poisonous dagger";
        w.description="Small, sharp and deadly";
        w.attack_type=POISONOUS;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 15.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 15.0f;
        w.attack_speed = 0.8f;
        w.shift = 0;
        pattern=swordPattern;
        patsize = sizeof(swordPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_169.png");
        w.name="Battleaxe";
        w.description="";
        w.attack_type=SIMPLE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 15.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 15.0f;
        w.attack_speed = 1.0f;
        w.shift = 0;
        pattern=swordPattern;
        patsize = sizeof(swordPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_161.png");
        w.name="Mace";
        w.description="";
        w.attack_type=SIMPLE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 2);
        w.staminaConsumption = 20.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 5.0f;
        w.attack_speed=0.67f;
        w.shift = 0;
        pattern=swordPattern;
        patsize = sizeof(swordPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_171.png");
        w.name="King's axe";
        w.description="Make an axe from gold sounds like a bad idea";
        w.attack_type=CURSE;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 8);
        w.staminaConsumption = 20.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 25.0f;
        w.attack_speed=0.33f;
        w.shift = 0;
        pattern=rangedWeaponPattern;
        patsize = sizeof(rangedWeaponPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_196.png");
        w.name="Crossbow";
        w.description="Can kill a troll in one shot";
        w.attack_type=MAGIC_BALL;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

        w.areaSize = sf::Vector2u(3, 8);
        w.staminaConsumption = 20.0f;
        w.manaConsumption = 0.0f;
        w.magicDamage = 0.0f;
        w.physDamage = 15.0f;
        w.attack_speed=0.5f;
        w.shift = 0;
        pattern=swordPattern;
        patsize = sizeof(swordPattern);
        id = gRenderer.AddTexture("tileset/Sliced/items_16x16/oryx_16bit_fantasy_items_191.png");
        w.name="Bow";
        w.description="Strong and reliable";
        w.attack_type=MAGIC_BALL;
        w.pattern.resize(patsize / sizeof(bool));
        for (unsigned i = 0; i < w.pattern.size(); ++i)
            w.pattern[i] = pattern[i];
        w.tileId = gRenderer.AddTile(id);
        ws.push_back(w);

    }
Weapon *WeaponsCatalogue::getWeapon(int ID)
{
    Weapon *w=&ws[ID%ws.size()];
    return(w);
}
Weapon *WeaponsCatalogue::getRandom()
{
    int k=rand()%ws.size();
    return(&ws[k]);
}

std::vector<Weapon*> WeaponsCatalogue::getUniqRandom(){
    std::vector<Weapon*> result;
    for(auto& weapon : ws){
        result.push_back(&weapon);
    }
    return result;
}

}