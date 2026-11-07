#include "Loader/SDLAudioLoader.hpp"
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL.h>

namespace mango_engine {
    namespace Loader {

        Audio::AudioBuffer::Ptr SDLAudioLoader::Load(const std::string& filePath) {
            if (!m_mixer) {
                SDL_Log("Mixer is null, cannot load audio: %s", filePath.c_str());
                return nullptr;
            }

            // 第三个参数 true 表示预解码，防止第一次播放时卡顿
            MIX_Audio* rawAudio = MIX_LoadAudio(m_mixer, filePath.c_str(), true);
            if (!rawAudio) {
                SDL_Log("Failed to load audio '%s': %s", filePath.c_str(), SDL_GetError());
                return nullptr;
            }

            // 之前定义的 AudioBuffer 构造函数是 public 的，可以直接 new
            return std::make_shared<Audio::AudioBuffer>(rawAudio);
        }
    }
}