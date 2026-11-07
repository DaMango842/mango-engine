#include "Loader/AudioLoader.hpp"
#include "Loader/SDLAudioLoader.hpp"
#include <SDL3/SDL.h>

namespace mango_engine {
    namespace Loader {
        std::unique_ptr<IAudioLoader> AudioLoader::s_loader = nullptr;

        void AudioLoader::Init(MIX_Mixer* mixer) {
            // 这里决定了底层到底用谁。以后想换，只改这一行
            s_loader = std::make_unique<SDLAudioLoader>(mixer);
        }

        Audio::AudioBuffer::Ptr AudioLoader::Load(const std::string& filePath) {
            if (!s_loader) {
                SDL_Log("AudioLoader not initialized!");
                return nullptr;
            }
            if (s_loader->CanLoad(filePath)) {
                return s_loader->Load(filePath);
            }
            SDL_Log("Unsupported audio format: %s", filePath.c_str());
            return nullptr;
        }
    }
}