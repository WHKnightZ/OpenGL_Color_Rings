#include "main.h"

void Init_Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Sound_Tick = Mix_LoadWAV("Sounds/Tick.wav");
        Sound_Explode = Mix_LoadWAV("Sounds/Explode.wav");
        Sound_Game_Over = Mix_LoadWAV("Sounds/Game_Over.wav");
    }
}
