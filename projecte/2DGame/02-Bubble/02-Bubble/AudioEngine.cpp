#include "AudioEngine.h"

AudioEngine* AudioEngine::instance = nullptr;

AudioEngine& AudioEngine::getInstance() {
    if (instance == nullptr) {
        instance = new AudioEngine();
    }
    return *instance;
}

AudioEngine::AudioEngine() {
    engine = nullptr;
    init();
}

AudioEngine::~AudioEngine() {
    stopAll();

    if (engine) {
        engine->drop();
        engine = nullptr;
    }

    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

bool AudioEngine::init() {
    engine = irrklang::createIrrKlangDevice();

    if (!engine) {
        return false;
    }

    return true;
}

void AudioEngine::play2D(const std::string& filepath, bool loop, float volume) {
    if (!engine) return;

    // Detener la música anterior si existe
    if (sounds.find(filepath) != sounds.end() && sounds[filepath]) {
        sounds[filepath]->stop();
        sounds[filepath]->drop();
        sounds.erase(filepath);
    }

    // Reproducir la nueva música
    irrklang::ISound* sound = engine->play2D(filepath.c_str(), loop, false, true);

    if (sound) {
        sound->setVolume(volume);
        sounds[filepath] = sound;
    }
}

void AudioEngine::stopAll() {
    if (!engine) return;

    for (auto& pair : sounds) {
        if (pair.second) {
            pair.second->stop();
            pair.second->drop();
        }
    }

    sounds.clear();
    engine->stopAllSounds();
}

void AudioEngine::setVolume(float volume) {
    if (!engine) return;

    for (auto& pair : sounds) {
        if (pair.second) {
            pair.second->setVolume(volume);
        }
    }
}
