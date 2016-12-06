#ifndef BOSS__H
#define BOSS__H
#include <string>
#include <vector>
#include "drawable.h"
#include "sprite.h"

class Boss : public Sprite {
    public:
        //Boss();
        Boss(const std::string& s);
        ~Boss() {};
        void reset() { 
            boss_hp = 100;
        }

        int getHp() { return boss_hp; }
        int totalHp() {return boss_total_hp;}
        void getHit() { 
            if(!boss_killed())
            {boss_hp -= 1;}
        }
        bool boss_killed() { return boss_hp <= 0;}

    private:
        int boss_hp;
        int boss_total_hp;
};

#endif
