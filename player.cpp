#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "expPlayer.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
            if(velocityX()>=0){
    currentFrame = (currentFrame+1) %( numberOfFrames/2);}
            else if(velocityX()<0){
    currentFrame = (currentFrame+1) % (numberOfFrames/2) +numberOfFrames/2;}
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  m_frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("WORLD_WIDTH")),
  worldHeight(Gamedata::getInstance().getXmlInt("WORLD_HEIGHT")),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(m_frames[0]->getWidth()),
  frameHeight(m_frames[0]->getHeight())
{ }

Player::Player(const Player& s) :
  Drawable(s),
  explosion(NULL),
  m_frames(s.m_frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

Player& Player::operator=(const Player& rhs) {
    Drawable::operator=(rhs);
    explosion = rhs.explosion;
    //m_frames = rhs.m_frames;
    frameWidth = rhs.frameWidth;
    frameHeight = rhs.frameHeight;
    worldWidth = rhs.worldWidth;
    worldHeight = rhs.worldHeight;
    return *this;
}
void Player::explode(){
    if( explosion ) return;
    explosion = new ExplodingPlayer(*this);
}

void Player::move(Uint8* keystate) {
        if( keystate[SDLK_a] )
        {
            this->velocityX(-250);
        }
        if( keystate[SDLK_s] )
        {
            this->velocityY(200);
        }
        if( keystate[SDLK_w] )
        {
            this->velocityY(-200);
        }
        if( keystate[SDLK_d] )
        {
            this->velocityX(300);
        }
        if( keystate[SDLK_a] && keystate[SDLK_d] )
            this->velocityX(0);
        if( keystate[SDLK_w] && keystate[SDLK_s] )
            this->velocityY(0);

        if( !keystate[SDLK_a] && !keystate[SDLK_d] )
            this->velocityX(0);
        if( !keystate[SDLK_w] && !keystate[SDLK_s] )
            this->velocityY(0);
}

void Player::draw() const { 
  if( explosion ){
      explosion->draw();
      return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  m_frames[currentFrame]->draw(x, y);
}

void Player::update(Uint32 ticks) { 

  if( explosion ) {
      explosion->update(ticks);
      if(explosion->chunkCount() == 0) {
          delete explosion;
          explosion = NULL;
      }
      return;
  }
  advanceFrame(ticks);

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
