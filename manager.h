#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "pool.h"
#include "boss.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  int k;
  bool god_mode;
  const bool env;
  const IOManager& io;
  Clock& clock;
  Pool& pool;

  SDL_Surface * const screen;
  World world;
  World top;
  World bot;
  World mount;

  Viewport& viewport;

  Hud& hud;

  Boss* boss;

  std::vector<Drawable*> sprites;
  int currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw(Uint32) const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
