#include <iostream>
#include <string>
#include <iomanip>
#include "multisprite.h"
#include "player.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "collisionStrategy.h"
#include "sound.h"

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }
}


Manager::Manager() :
    k(0),
    god_mode(false),
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  pool(Pool::getInstance()),
  screen( io.getScreen() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  top("top", Gamedata::getInstance().getXmlInt("top/factor") ),
  bot("bot", Gamedata::getInstance().getXmlInt("bot/factor") ),
  mount("mount", Gamedata::getInstance().getXmlInt("mount/factor") ),
  viewport( Viewport::getInstance() ),
  hud(Hud::getInstance()),
  boss(),
  sprites(),
  currentSprite(0),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);
  sprites.push_back( new Player("player") );
  boss = new Boss("Boss");
  for (unsigned int i=0 ; i < 10; i++)
  {
      sprites.push_back(new Sprite("b"));
      sprites.back()->X(rand()%4000);
      sprites.back()->Y(rand()%50-50);
      sprites.back()->velocityX(rand()%200+100);
      sprites.back()->velocityY(rand()%50+100);
  }
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw(Uint32 RED) const {
  world.draw();
  mount.drawFloat(0, 300);
  pool.draw_b();
  for (unsigned i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }
  top.drawFloat(0, 0);
  bot.drawFloat(0, 400);
  viewport.draw();
  if( clock.getSeconds() < 4 )
  {
      io.printMessageAt( "Find And Kill The BOSS!", 400, 240);
  }
  //draw hud;
  if(hud.is_start() || hud.getF1()) {
      hud.draw_line(screen, RED, clock.getFps(), sprites);
  }
  if(hud.getF2())  {
  hud.draw_pool(screen, RED, pool);
  }

  boss->draw();

  hud.draw_boss_hp(screen, boss);
  if(boss->boss_killed() )
  { hud.draw_win(k); }

  SDL_Flip(screen);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::update() {
  ++(clock);
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    //switchSprite();
  }
  pool.update_b(ticks, sprites, boss);

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    if(i != 0 )
    // collision
    {   if  ((!(static_cast<Sprite*>(sprites[0])->if_exploding()))&&(!(static_cast<Sprite*>(sprites[i])->if_exploding()))) {
         if (static_cast<Sprite*>(sprites[i])->if_collision(sprites[0]))
             {   
             sprites[i]->explode();
             if(!god_mode) {
             sprites[0]->explode();
             } 
             sprites[i]->X(-100);
             sprites[i]->Y(240);
             }
        }
    }
    sprites[i]->update(ticks);
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  mount.update();
  boss->update(ticks);
  world.update();
  top.update();
  bot.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  const Uint32 RED = SDL_MapRGB( screen->format, 0xff, 0,0);
  SDLSound sound;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      sprites[0]->move(keystate);
      
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        // god mode-------- leftCtrl + G
        if ( keystate[SDLK_LCTRL]&& keystate[SDLK_g] )
        {
            god_mode = (!god_mode);
            if(god_mode)
            std::cout << "GOD__MODE__IS__ACTIVATED !" << std::endl;
            else
                std::cout << " Exiting _ God _ Mode " << std::endl;
        }
        //exit key ---- Q   /  ESC
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        //F1 F2 --- HUD CONTROL
        if( keystate[SDLK_F1] ) {
            hud.setF1(!hud.getF1());
        }
        if ( keystate[SDLK_F2] ) {
            hud.setF2(!hud.getF2());
        }
        //shoot key ----- B
        if ( keystate[SDLK_b] ) {
            if( !((static_cast<Sprite*>(sprites[0])->if_exploding())))
            pool.shoot(sprites[0]);
        }
        // pause key ---- P
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        // reset function
        if( keystate[SDLK_r] )
        {
            sound.startMusic();
            clock.reset();
            boss->reset();
            sprites[0]->X(300);
            sprites[0]->Y(240);
            for (unsigned int i = 1; i<10; i++)
            {
                sprites[i]->X(rand()%4000);
                sprites[i]->Y(rand()%50+50);
                sprites[i]->velocityX(rand()%200+100);
                sprites[i]->velocityY(rand()%50+100);
            }
        }
        // make video key----F4
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    update();
    
    if( (boss->boss_killed())&&(k==0)) {
      sound.winMusic();
      k = clock.getSeconds();
     }
    
    draw(RED);
    //update();
  }
}
