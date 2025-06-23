#include "game/MusicSystem.h"
#include "dolphin/dtk.h"

MusicSystem gMusicSystem;

void MusicSystem::LoadMusic(void) {
    
}

void MusicSystem::PlayMusic(void) {

}

void MusicSystem::SetMusicVolume(u8 volume) {
    DTKSetVolume(volume, volume);
    m_Volume = volume;
}
