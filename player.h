#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include "drawable.h"

class ExplodingPlayer;
class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player& s);
  Player& operator=(const Player&);

  virtual ~Player() { } 
  
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return m_frames[currentFrame]; 
  }
  void explode();
  void move(Uint8*);

protected:
  ExplodingPlayer* explosion;
  const std::vector<Frame *> m_frames;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
#endif
