#include "hud.h"
#include <iostream>

Hud& Hud::getInstance() {
    static Hud instance;
    return instance;
}

Hud::Hud():
    io(IOManager::getInstance()),
    clock(Clock::getInstance()),
    hud_life(true),
    hud_f1(false),
    hud_f2(true)
{ }

void Hud::draw() const{
}

void Hud::draw_line(SDL_Surface* screen, const Uint32 RED, int fps, std::vector<Drawable*> sprites) const {
    SDL_Rect rect = {5, 10, 170, 200};
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 178, 102));
    SDL_Rect dest = {5, 10, 170, 200};
    SDL_BlitSurface(screen, &rect, screen, &dest);

    Sint16 y1 = 10;
    Sint16 x1 = 5, x2 = 175;
    
    //draw aaline
    Draw_AALine(screen, x1, y1, x2, y1, 2.0f, RED);
    Draw_AALine(screen, x1, y1+200, x2, y1+200, 2.0f, RED);
    Draw_AALine(screen, x2, y1, x2, y1+200, 2.0f, RED);
    Draw_AALine(screen, x1, y1, x1, y1+200, 2.0f, RED);
    
    io.printMessageValueAt("seconds:", clock.getSeconds(), 10, 20);
    io.printMessageValueAt("fps: ", fps, 10, 40);
    io.printMessageValueAt("X-pos:", int(sprites[0]->X()), 10, 60);
    io.printMessageValueAt("Y-pos:", int(sprites[0]->Y()), 10, 80);
    io.printMessageAt("A: Move Left", 10, 100);
    io.printMessageAt("S: Move Down", 10, 120);
    io.printMessageAt("D: Move Right", 10, 140);
    io.printMessageAt("W: Move Up", 10, 160);
    io.printMessageAt("B: Shooooooot", 10, 180);
}

bool Hud::is_start() {
    return SDL_GetTicks() < 3000 ; }

bool Hud::getF1() {return hud_f1;}
bool Hud::getF2() {return hud_f2;}
void Hud::setF1(bool s) { hud_f1 = s;}
void Hud::setF2(bool s) { hud_f2 = s;}
   
void Hud::update() {
}

void Hud::draw_pool(SDL_Surface* screen, const Uint32 RED, Pool& pool){
    SDL_Rect rect = {650, 10, 200, 110};
    SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 178, 102));
    SDL_Rect dest = {650, 10, 200, 110};
    SDL_BlitSurface(screen, &rect, screen, &dest);

    Sint16 y1 = 10;
    Sint16 x1 = 650, x2 = 850;
    //draw aaline
    Draw_AALine(screen, x1, y1, x2, y1, 2.0f, RED);
    Draw_AALine(screen, x1, y1+110, x2, y1+110, 2.0f, RED);
    Draw_AALine(screen, x2, y1, x2, y1+110, 2.0f, RED);
    Draw_AALine(screen, x1, y1, x1, y1+110, 2.0f, RED);
    
    io.printMessageAt("F2 toggle this", 660, 20);
    io.printMessageValueAt("Bullets Avaliable:", pool.get_bullet_number(), 660, 50);
    io.printMessageValueAt("Flying Bullets:", pool.get_inuse_number(), 660, 80);
}

void Hud::draw_boss_hp(SDL_Surface* screen, Boss* boss) {
    Uint16 hp_length = 4.5*(boss->getHp());

    io.printMessageValueAt("Boss HP:",boss->getHp(), 100, 445);
    SDL_Rect hp_bar = {200, 440, hp_length, 20};
    SDL_Rect hp_dest = {200, 440, hp_length, 20};
    SDL_FillRect(screen, &hp_bar, SDL_MapRGB(screen->format, 0xff, 0, 0));
    SDL_BlitSurface(screen, &hp_bar, screen, &hp_dest);

    Sint16 x1 = 200, x2 = 650, y1 = 440, y2 = 460;
    //draw hp bar rect;

    Draw_AALine(screen, x1, y1, x1, y2, 2.0f, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
    Draw_AALine(screen, x2, y1, x2, y2, 2.0f, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
    Draw_AALine(screen, x1, y1, x2, y1, 2.0f, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
    Draw_AALine(screen, x1, y2, x2, y2, 2.0f, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));
}

void Hud::draw_win(int k) {
    io.printMessageAt(" YOU WIN THIS ONE! ", 400, 230);
    io.printMessageAt(" Kill Boss Faster Next Time !", 400, 260 );
    io.printMessageValueAt(" YOUR SCORE: ", k, 400, 290);
}

