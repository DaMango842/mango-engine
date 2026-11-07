#pragma once
#include <Audio/AudioBuffer.hpp>
#include <string>

namespace mango_engine {
    namespace Loader {
        class IAudioLoader {
        public:
            virtual ~IAudioLoader() = default;
            
            // 判断该加载器是否能处理这个后缀名（比如 .wav, .mp3）
            virtual bool CanLoad(const std::string& filePath) const = 0;
            
            // 注意：返回智能指针！
            virtual Audio::AudioBuffer::Ptr Load(const std::string& filePath) = 0;
        };
    }
}