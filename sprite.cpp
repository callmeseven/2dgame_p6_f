#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  explosion(NULL),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("WORLD_WIDTH")),
  worldHeight(Gamedata::getInstance().getXmlInt("WORLD_HEIGHT"))
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel):
  Drawable(n, pos, vel), 
  explosion(NULL),
  frame( FrameFactory::getInstance().getFrame(n) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("WORLD_WIDTH")),
  worldHeight(Gamedata::getInstance().getXmlInt("WORLD_HEIGHT"))
{ }

Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Frame* frm):
  Drawable(n, pos, vel), 
  explosion(NULL),
  frame( frm ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("WORLD_WIDTH")),
  worldHeight(Gamedata::getInstance().getXmlInt("WORLD_HEIGHT"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  explosion(NULL),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("WORLD_WIDTH")),
  worldHeight(Gamedata::getInstance().getXmlInt("WORLD_HEIGHT"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs){
    Drawable::operator=( rhs );
    explosion = rhs.explosion;
    frame = rhs.frame;
    frameWidth = rhs.frameWidth;
    frameHeight = rhs.frameHeight;
    worldWidth = rhs.worldWidth;
    worldHeight = rhs.worldHeight;
    return *this;
}

void Sprite::explode(){
    if ( explosion ) return;
    explosion = new ExplodingSprite(*this);
}

bool Sprite::if_collision( Drawable* obj ) {
    int COL_DISTANCE = this->getFrame()->getWidth()/2 + obj->getFrame()->getWidth()/2;
    float x1 = this->X() + this->getFrame()->getWidth()/2;
    float x2 = obj->X() + obj->getFrame()->getWidth()/2;
    float y1 = this->Y() + this->getFrame()->getHeight()/2;
    float y2 = obj->Y() + obj->getFrame()->getHeight()/2;

    return hypot((x2-x1), (y2-y1)) < COL_DISTANCE;
}

bool Sprite::if_exploding() {
    if (explosion!=NULL){return true;}
    return false;
}


void Sprite::draw() const { 
    if(explosion){
        explosion->draw();
        return;
    }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Sprite::update(Uint32 ticks) { 
    if(explosion) {
        explosion->update(ticks);
        if(explosion->chunkCount() == 0){
            delete explosion;
            explosion = NULL;
            }
        return;
    }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}
