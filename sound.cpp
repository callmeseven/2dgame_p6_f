#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
#include <SDL/SDL_mixer.h>
using std::string;

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
  Mix_HaltMusic();
  Mix_FreeMusic(music);

  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}

SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME/4), 
  currentSound(-1), 
  music(NULL),
  winner(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  channels() 
{
  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS("sound/bgm.mp3");
  if (!music) throw string("Couldn't load bgm.mp3")+Mix_GetError();

  startMusic();
  winner = Mix_LoadMUS("sound/output.wav");
  if (!winner) throw string("bad output.wav ")+Mix_GetError();

}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::winMusic() {
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(winner, 1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

