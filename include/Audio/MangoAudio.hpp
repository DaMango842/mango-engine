#pragma once
#include <Audio/AudioBuffer.hpp>

struct MIX_Mixer;

namespace mango_engine {
    namespace Audio {

        class MangoAudio {
        public:
            MangoAudio();
            ~MangoAudio();

            // 禁用拷贝
            MangoAudio(const MangoAudio&) = delete;
            MangoAudio& operator=(const MangoAudio&) = delete;

            // 基础播放接口
            void PlayOneShot(AudioBuffer::Ptr buffer, float volume = 1.0f);
            
            // 获取底层 Mixer 给 Loader 用
            MIX_Mixer* GetMixer() const { return m_mixer; }

        private:
            MIX_Mixer* m_mixer = nullptr;
        };
    }
}