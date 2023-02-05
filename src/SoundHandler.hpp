//
// Created by Anton on 03.02.2023.
//

#ifndef OPENGLPONG_SOUNDHANDLER_HPP
#define OPENGLPONG_SOUNDHANDLER_HPP

#include <iostream>
#include <filesystem>
#include "irrKlang.h"
#include "Utils.hpp"

class SoundHandler
{
public:
    static void Initialize(const std::string& soundFilesPath)
    {
#ifndef __MINGW32__
        SoundEngine = irrklang::createIrrKlangDevice();
        ASSERT(SoundEngine != nullptr, "Failed to create sound engine");
        
        for (const auto& file : std::filesystem::directory_iterator(soundFilesPath))
        {
            if (file.is_regular_file())
            {
                // the trick here is to force the sound engine to fully preload the sound before finishing the initialization by playing the sound with 0 volume
                // the "true" is supposedly responsible for doing exactly that (preloading the sound, not to play it on 0 volume), but it didn`t work for me
                auto sound = SoundEngine->addSoundSourceFromFile(file.path().string().c_str(), irrklang::ESM_AUTO_DETECT, true);
                
                sound->setDefaultVolume(0.0F);
                SoundEngine->play2D(sound);
                sound->setDefaultVolume(1.0F);
            }
        }
#endif
    }

    static void PlayAudio(const std::string& path, bool looped = false)
    {
#ifndef __MINGW32__
        SoundEngine->play2D(path.c_str());
#endif
    }
private:
    inline static irrklang::ISoundEngine * SoundEngine;
};

#endif //OPENGLPONG_SOUNDHANDLER_HPP
