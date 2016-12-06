#include <vector>
#include <list>
#include "chunk.h"
#include "player.h"

class Player;
class ExplodingPlayer : public Player {
public:
  ExplodingPlayer(const Player& s);
  ~ExplodingPlayer();

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int);
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here

  std::vector<SDL_Surface*> surfaces; // Each chunk has a Surface
  std::vector<Frame*> frames; // Each chunk has a Frame

  ExplodingPlayer(const ExplodingPlayer&); // Explicit disallow (Item 6)
  ExplodingPlayer& operator=(const ExplodingPlayer&); // (Item 6)
};
