#include "pool.h"
#include <iostream>

Bullet::Bullet(const std::string& s) :
    Sprite(s),
    life(false),
    life_time(0)
{ }

bool Bullet::if_collide( Drawable* obj ) {
    int COL_DISTANCE = this->getFrame()->getWidth()/2 + obj->getFrame()->getWidth()/2;
    float x1 = this->X() + this->getFrame()->getWidth()/2;
    float x2 = obj->X() + obj->getFrame()->getWidth()/2;
    float y1 = this->Y() + this->getFrame()->getHeight()/2;
    float y2 = obj->Y() + obj->getFrame()->getHeight()/2;

    return hypot((x2-x1), (y2-y1)) < COL_DISTANCE+5;
}



void Bullet::setLife_F() {
    life = false;
}

void Bullet::setLife_T() {
    life = true;
}

void Bullet::reset(){
    life_time = 0;
    life = false;

}

void Bullet::end_life(){
    life = false;
}

void Bullet::set_lifetime(){
    life_time = SDL_GetTicks();
    life = true;
}


Pool& Pool::getInstance(){
    static Pool instance;
    return instance;
}

Pool::Pool() : bullets(), b_inuse() {
    for (int i=0; i<6; i++)
    {
      bullets.push_back(new Bullet("bullet"));
    }
}

Bullet* Pool::getBullet(){
    Bullet* temp = bullets.front();
    bullets.pop_front();
    return temp;
}

void Pool::returnBullet(Bullet* s) {
    s->reset();
    bullets.push_back(s);
    std::cout << "bullet size after return" << bullets.size() << std::endl;
}

void Pool::draw_b() const {
    std::list<Bullet*>::const_iterator ptr = b_inuse.begin();
    while ( ptr != b_inuse.end() ){
        if((*ptr)->is_alive())
        (*ptr)->Sprite::draw();
        ptr++;
    }
}

void Pool::update_b(Uint32 ticks, std::vector<Drawable*>& s, Boss* boss) {
   if(b_inuse.size() != 0){
        std::list<Bullet*>::iterator ptr = b_inuse.begin();
        while (ptr != b_inuse.end() ) {
            if (SDL_GetTicks() - (*ptr)->get_lifetime() > 3000)
                (*ptr)->end_life();
            (*ptr)->Sprite::update(ticks);
            // detect if collide with flyings.
            std::vector<Drawable*>::iterator sptr = s.begin()+1;
            while( sptr != s.end() ) {

                if ( (*ptr)->if_collide(*sptr) )
                {
                    (*ptr)->end_life();
                    (*sptr)->explode();
                }
                sptr++;
            }

            //detect if collide with boss
            if( (*ptr)->is_alive() ) {
                if( (*ptr)->if_collide(boss) )
                {
                    (*ptr)->end_life();
                    boss->getHit();
                    if( boss->boss_killed()){
                    boss->explode();
                    }
                }
            }

            if(!(*ptr)->is_alive()){
                (*ptr)->reset();
                std::list<Bullet*>::iterator temp = ptr;
                bullets.push_back((*ptr));
                ptr++;
                b_inuse.erase(temp);
            }
            else{
                ptr++;
            }

        }
    }
}

void Pool::shoot(Drawable* s) {
    
    if (bullets.size()>0){
        if(b_inuse.size()<6){
            b_inuse.push_back((*this).getBullet());
            b_inuse.back()->set_lifetime();
            b_inuse.back()->Drawable::velocityX(550);
            b_inuse.back()->Drawable::velocityY(0);
            b_inuse.back()->Drawable::X(s->X()+10);
            b_inuse.back()->Drawable::Y(s->Y()+10);
        }
    }
}

void Pool::printsize() {

    std::cout<< bullets.size() << std::endl;
    std::cout<< b_inuse.size() << std::endl;
}
