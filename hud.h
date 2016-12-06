//HUD CLASS
//display hud with help info & control settings
//header file
//ChunpengShao
#include <SDL.h>
#include <vector>
#include "drawable.h"
#include "aaline.h"
#include "ioManager.h"
#include "clock.h"
#include "pool.h"
#include <SDL.h>
#include <cstdlib>
#include "boss.h"

class Hud {
    public:
        Hud();
        static Hud& getInstance();
        void draw() const;
        //void draw_hpbar() const;
        void draw_line(SDL_Surface*, const Uint32, int, std::vector<Drawable*> ) const;
        void draw_pool(SDL_Surface*, const Uint32, Pool&);
        void draw_boss_hp(SDL_Surface*, Boss*) ;
        void draw_win(int);
        bool is_start();
        bool getF1();
        bool getF2();
        void setF1(bool);
        void setF2(bool);

        void update();

    private:
        const IOManager& io;
        const Clock& clock;
        bool hud_life;
        bool hud_f1;
        bool hud_f2;
};

