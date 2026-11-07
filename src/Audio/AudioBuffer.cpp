// src/Audio/AudioBuffer.cpp
#include <Audio/AudioBuffer.hpp>
#include <SDL3_mixer/SDL_mixer.h>

namespace mango_engine {
    namespace Audio {

        // 实现智能指针的自定义删除器
        void AudioBufferDeleter::operator()(MIX_Audio* audio) const {
            if (audio) {
                MIX_DestroyAudio(audio);
            }
        }

        // 实现构造函数
        AudioBuffer::AudioBuffer(MIX_Audio* rawAudio) : m_audio(rawAudio) {
            // 如果还有其他初始化逻辑，可以写在这里
        }
    }
}