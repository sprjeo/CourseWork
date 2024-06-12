#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>

class Equipment
{
public:
    std::vector<std::vector<int>> equip_vecs;
    std::vector<int> helmet;
    std::vector<int> gloves;
    std::vector<int> shoulder_strap;
    std::vector<int> boots;
    std::vector<int> cloak;

    int luckBonus;

    enum equip_slots
    {
        helmet_index = 0,
        gloves_index,
        shoulder_strap_index,
        boots_index,
        cloak_index
    };

    Equipment()
    {
        equip_vecs.push_back(helmet);
        equip_vecs.push_back(gloves);
        equip_vecs.push_back(shoulder_strap);
        equip_vecs.push_back(boots);
        equip_vecs.push_back(cloak);
    }

public:
    void generate(equip_slots index)
    {
        //первые две в каждом здоровье и броня, третья задана для каждого, последняя одна или две выпадают случайно
        //helmet[2] cила
        //gloves[2] ловкость
        //boots[2] ловкость
        //shoulder_strap[2] cила
        //cloak[2] ум
        luckBonus = std::rand() % 5 + 1;

        if (luckBonus % 2 == 1)
        {
            for (int i = 0; i < 5; ++i)
            {
                equip_vecs[index].push_back(std::rand() % luckBonus + 1);
            }
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                equip_vecs[index].push_back(std::rand() % luckBonus + 1);
            }
            //если 0 то меткость, 1 мастерство
            equip_vecs[index].push_back(std::rand() % luckBonus + 1);
            // для того чтобы отличить по размеру
            equip_vecs[index].push_back(0);
        }
    }
};

class Character {
public:
    Equipment equipment;
    int health;
    int armor;
    int accuracy;
    int luck;
    int skill;
    int intelligence;
    int agility;
    int strength;


    virtual int calculateDamage()
    {
        int miss = rand() % accuracy;
        miss == 0 ? miss = 0 : miss = 1;
        int double_damage = rand() % luck;
        double_damage > 5 ? double_damage = 2 : double_damage = 1;
        int damage = miss * (double_damage * (strength + skill * 3));

        nullify_ability();

        return damage;
    }

    virtual void useAbility() = 0;

    virtual void nullify_ability() = 0;

    virtual void print_stats()
    {
        std::cout << "health: " << health << std::endl;
        std::cout << "armor: " << armor << std::endl;
        std::cout << "accuracy: " << accuracy << std::endl;
        std::cout << "luck: " << luck << std::endl;
        std::cout << "skill: " << skill << std::endl;
        std::cout << "intelligence: " << intelligence << std::endl;
        std::cout << "agility: " << agility << std::endl;
        std::cout << "strength: " << strength << std::endl;
    }

    virtual void generate_equipment(Equipment::equip_slots index)
    {
        if (index > 4) throw std::invalid_argument("invalid argument in generate_equipment()");
        equipment.generate(index);
        luck = luck + equipment.luckBonus;
        health = health + equipment.equip_vecs[index][0];
        armor = armor + equipment.equip_vecs[index][1];
        if (index == Equipment::equip_slots::helmet_index || index == Equipment::equip_slots::shoulder_strap_index)
        {
            strength = strength + equipment.equip_vecs[index][2];

        }
        else if (index == Equipment::equip_slots::gloves_index || index == Equipment::equip_slots::boots_index)
        {
            agility = agility + equipment.equip_vecs[index][2];
        }
        else if (index == Equipment::equip_slots::cloak_index)
        {
            intelligence = intelligence + equipment.equip_vecs[index][2];
        }

        if (equipment.equip_vecs[index].size() == 5)
        {
            accuracy = accuracy + equipment.equip_vecs[index][3];
            skill = skill + equipment.equip_vecs[index][4];
        }
        else
        {
            if (equipment.equip_vecs[index][4] == 1)
            {
                skill = skill + equipment.equip_vecs[index][3];
            }
            else
            {
                accuracy = accuracy + equipment.equip_vecs[index][3];
            }
        }

    }

};


class Defender : public Character
{
public:
    int bonus_armor;

    Defender()
    {
        health = 35;

        armor = 5;
        accuracy = 1;
        luck = 0;
        skill = 0;
        intelligence = 3;
        agility = 3;
        strength = 5;
        bonus_armor = 0;
    }

    void useAbility() override
    {
        bonus_armor = health * 3 + intelligence + agility * 2;
        armor = armor + bonus_armor;
    }

    void nullify_ability() override
    {
        armor = armor - bonus_armor;
        bonus_armor = 0;
    }
};

class Healer : public Character
{
public:
    int bonus_health;

    Healer()
    {
        health = 25;
        intelligence = 20;

        armor = 2;
        accuracy = 1;
        luck = 0;
        skill = 0;
        agility = 2;
        strength = 1;
        bonus_health = 0;
    }

public:

    void useAbility() override
    {
        bonus_health = intelligence * 2 + agility;
        health = health + bonus_health;
    }

    void nullify_ability() override
    {
        health = health - bonus_health;
        bonus_health = 0;
    }
};

class Melee_Fighter : public Character
{
public:
    int strength_bonus;
    Melee_Fighter()
    {
        health = 5;
        agility = 20;
        strength = 20;

        armor = 4;
        accuracy = 4;
        luck = 0;
        skill = 0;
        intelligence = 0;
        strength_bonus = 0;
    }

public:

    void useAbility() override
    {
        strength_bonus = (agility + strength) * 2;
        strength = strength + strength_bonus;
    }

    void nullify_ability() override
    {
        strength = strength - strength_bonus;
        strength_bonus = 0;
    }
};

class Ranged_Fighter : public Character
{
public:
    int strength_bonus;
    int accuracy_bonus;

    Ranged_Fighter()
    {
        health = 5;
        intelligence = 30;

        armor = 2;
        accuracy = 6;
        luck = 0;
        skill = 0;
        agility = 2;
        strength = 2;
        strength_bonus = 0;
        accuracy_bonus = 0;
    }

public:

    void useAbility() override
    {
        strength_bonus = intelligence * 2 + agility;
        strength = strength + strength_bonus;
        accuracy_bonus = (intelligence - accuracy) ^ 2;
        accuracy = accuracy + accuracy_bonus;
    }

    void nullify_ability() override
    {
        strength = strength - strength_bonus;
        strength_bonus = 0;
        accuracy = accuracy - accuracy_bonus;
        accuracy_bonus = 0;
    }
};

int main()
{
    try {
        std::srand(std::time(nullptr));
        Character* healer = new Healer();
        Character* meelee = new Melee_Fighter();
        Character* defender = new Defender();
        Character* ranged = new Ranged_Fighter();

        healer->generate_equipment(Equipment::equip_slots::helmet_index);
        meelee->generate_equipment(Equipment::equip_slots::cloak_index);
        defender->generate_equipment(Equipment::equip_slots::boots_index);
        ranged->generate_equipment(Equipment::equip_slots::cloak_index);

        healer->generate_equipment(Equipment::equip_slots::cloak_index);
        meelee->generate_equipment(Equipment::equip_slots::shoulder_strap_index);
        defender->generate_equipment(Equipment::equip_slots::helmet_index);
        ranged->generate_equipment(Equipment::equip_slots::boots_index);

        healer->generate_equipment(Equipment::equip_slots::gloves_index);
        meelee->generate_equipment(Equipment::equip_slots::gloves_index);
        defender->generate_equipment(Equipment::equip_slots::gloves_index);
        ranged->generate_equipment(Equipment::equip_slots::gloves_index);

        healer->useAbility();
        meelee->useAbility();
        defender->useAbility();
        ranged->useAbility();

        std::cout << std::endl << "healer's stats after using the ability: " << std::endl;
        healer->print_stats();
        std::cout << std::endl;
        std::cout << std::endl << "meelee fighter's stats after using the ability: " << std::endl;
        meelee->print_stats();
        std::cout << std::endl;
        std::cout << std::endl << "defender's stats after using the ability: " << std::endl;
        defender->print_stats();
        std::cout << std::endl;
        std::cout << std::endl << "ranged fighter's stats after using the ability: " << std::endl;
        ranged->print_stats();
        std::cout << std::endl;

        std::cout << "healer's damage: " << healer->calculateDamage() << std::endl;
        std::cout << "meelee fighter's damage: " << meelee->calculateDamage() << std::endl;
        std::cout << "defender's damage: " << defender->calculateDamage() << std::endl;
        std::cout << "ranged fighter's damage: " << ranged->calculateDamage() << std::endl;
        std::cout << std::endl;

        std::cout << std::endl << "healer's stats after dealing damage: " << std::endl;
        healer->print_stats();
        std::cout << std::endl << "meelee fighter's stats after dealing damage: " << std::endl;
        meelee->print_stats();
        std::cout << std::endl << "defender's stats after dealing damage: " << std::endl;
        defender->print_stats();
        std::cout << std::endl << "ranged fighter's stats after dealing damage: " << std::endl;
        ranged->print_stats();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    return 0;

}