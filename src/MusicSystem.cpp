#include "game/MusicSystem.h"
#include "dolphin/dtk.h"

void MusicSystem::LoadMusic(void) {
    
}

void MusicSystem::PlayMusic(void) {

}

void MusicSystem::SetMusicVolume(u8 volume) {
    DTKSetVolume(volume, volume);
    this->volume = volume;
}
