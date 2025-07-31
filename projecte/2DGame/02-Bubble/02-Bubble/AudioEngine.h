#ifndef _AUDIO_ENGINE_INCLUDE
#define _AUDIO_ENGINE_INCLUDE

#include <irrKlang.h>
#include <map>
#include <string>

class AudioEngine {
private:
    static AudioEngine* instance;
    irrklang::ISoundEngine* engine;
    std::map<std::string, irrklang::ISound*> sounds;

    AudioEngine();

public:
    static AudioEngine& getInstance();
    ~AudioEngine();

    bool init();
    void play2D(const std::string& filepath, bool loop = false, float volume = 1.0f);
    void stopAll();
    void setVolume(float volume);
};

#endif // _AUDIO_ENGINE_INCLUDE#pragma once
