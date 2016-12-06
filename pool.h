#ifndef POOL__H
#define POOL__H
#include <list>
#include "sprite.h"
#include <SDL.h>
#include "boss.h"
#include "player.h"

class Bullet: public Sprite {
    public:
        Bullet(const std::string& s);
        bool if_collide(Drawable*);
        void setLife_T();
        void setLife_F();

        void reset();
        ~Bullet() {} ;
        void end_life();
        bool is_alive() {return life;};
        void set_lifetime();
        unsigned int get_lifetime(){ return life_time; }
    private:
        bool life;
        unsigned int life_time;

};
       
class Pool {
    public:
        
        static Pool& getInstance();
        Pool(); 
        Bullet* getBullet();

        ~Pool() {
            std::list<Bullet*>::iterator ptr = bullets.begin();
            while(ptr != bullets.end()){
                delete *ptr;
                ptr++;
            }
            ptr = b_inuse.begin();
            while (ptr != b_inuse.end()){
                delete *ptr;
                ptr++;
            }
            bullets.clear();
            b_inuse.clear();
        }
        void returnBullet(Bullet* s);
        void draw_b() const;
        void update_b(Uint32 ticks, std::vector<Drawable*>&, Boss*);
        void shoot(Drawable* s);
        void printsize();
        int get_inuse_number() {return b_inuse.size();}
        int get_bullet_number() {return bullets.size();}

    private:
        std::list<Bullet*> bullets;
        std::list<Bullet*> b_inuse;
        
};

#endif
