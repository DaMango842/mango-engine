#pragma once
#include "Loader/IAudioLoader.hpp"
#include <memory>

struct MIX_Mixer;

namespace mango_engine {
    namespace Loader {
        class AudioLoader {
        public:
            // 初始化引擎底层的默认加载器
            static void Init(MIX_Mixer* mixer);

            // 统一的对外的加载入口，隐藏具体是哪个 Loader 在干活
            static Audio::AudioBuffer::Ptr Load(const std::string& filePath);

        private:
            static std::unique_ptr<IAudioLoader> s_loader;
        };
    }
}