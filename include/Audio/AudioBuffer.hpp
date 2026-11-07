#pragma once
#include <memory>
#include <string>

struct MIX_Audio; // 前向声明

namespace mango_engine {
    namespace Audio {

        struct AudioBufferDeleter {
            void operator()(MIX_Audio* audio) const;
        };

        class AudioBuffer {
        public:
            using Ptr = std::shared_ptr<AudioBuffer>;

            explicit AudioBuffer(MIX_Audio* rawAudio);
            ~AudioBuffer() = default;

            // 禁止拷贝，允许移动
            AudioBuffer(const AudioBuffer&) = delete;
            AudioBuffer& operator=(const AudioBuffer&) = delete;

            MIX_Audio* GetRaw() const { return m_audio.get(); }

        private:
            std::unique_ptr<MIX_Audio, AudioBufferDeleter> m_audio;
        };
    }
}