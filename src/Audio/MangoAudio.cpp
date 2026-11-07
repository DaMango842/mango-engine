#include <Audio/MangoAudio.hpp>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL.h>

namespace mango_engine {
    namespace Audio {

        MangoAudio::MangoAudio() {
            // SDL3_mixer 初始化
            if (!MIX_Init()) {
                SDL_Log("MIX_Init failed: %s", SDL_GetError());
                return;
            }

            // 创建默认音频设备
            m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
            if (!m_mixer) {
                SDL_Log("Failed to create mixer: %s", SDL_GetError());
            }
        }

        MangoAudio::~MangoAudio() {
            if (m_mixer) {
                MIX_DestroyMixer(m_mixer);
            }
            MIX_Quit();
        }

        void MangoAudio::PlayOneShot(AudioBuffer::Ptr buffer, float volume) {
            if (!m_mixer || !buffer) return;

            // SDL3_mixer 的最简播放方式：直接播放
            // 注意：如果需要控制音量等，需要创建 Track
            MIX_PlayAudio(m_mixer, buffer->GetRaw());
        }
    }
}