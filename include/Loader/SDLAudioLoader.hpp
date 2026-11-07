#pragma once
#include "IAudioLoader.hpp"
#include <algorithm>

struct MIX_Mixer;

namespace mango_engine {
    namespace Loader {
        class SDLAudioLoader : public IAudioLoader {
        public:
            // 依赖注入：把大总管的 Mixer 指针传进来
            explicit SDLAudioLoader(MIX_Mixer* mixer) : m_mixer(mixer) {}

            bool CanLoad(const std::string& filePath) const override {
                // 简单粗暴地检查后缀名，SDL3_mixer 默认支持这些
                if (filePath.length() < 4) return false;
                std::string ext = filePath.substr(filePath.find_last_of('.') + 1);
                // 转小写（防呆设计，防止 .WAV 和 .wav 不认）
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                
                return ext == "wav" || ext == "mp3" || ext == "ogg" || ext == "flac";
            }

            Audio::AudioBuffer::Ptr Load(const std::string& filePath) override;

        private:
            MIX_Mixer* m_mixer = nullptr;
        };
    }
}