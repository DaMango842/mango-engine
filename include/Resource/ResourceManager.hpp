#pragma once

#include <unordered_map>
#include <string>
#include <memory>

// 前向声明底层依赖
struct SDL_Renderer;
struct MIX_Mixer;

// 引入你的资源类型
#include <Resource/Texture.hpp>
#include <Resource/Font.hpp>
#include <Audio/AudioBuffer.hpp>

namespace mango_engine {
    namespace Resource {

        class ResourceManager {
        public:
            // 初始化时传入底层的渲染器和音频设备（让管理器拥有加载的能力）
            ResourceManager(SDL_Renderer* renderer, MIX_Mixer* mixer);
            ~ResourceManager() = default;

            // 禁止拷贝（管理器应该是全局唯一的，或者作为单例）
            ResourceManager(const ResourceManager&) = delete;
            ResourceManager& operator=(const ResourceManager&) = delete;

            // --- 核心加载接口 ---
            Texture::Ptr GetTexture(const std::string& path);
            Font::Ptr GetFont(const std::string& path, int size);
            Audio::AudioBuffer::Ptr GetAudio(const std::string& path);

            // 清理未被任何对象使用的资源（可选，C++ 的 shared_ptr 其实会自动清理，但这里可以强制清空缓存）
            void ClearCache();

        private:
            SDL_Renderer* m_renderer = nullptr;
            MIX_Mixer* m_mixer = nullptr;

            // 缓存表（Key 为文件路径）
            std::unordered_map<std::string, Texture::Ptr> m_textureCache;
            
            // 字体需要特殊的 Key，因为同一个字体文件可能有不同字号（例如 "font.ttf_16"）
            std::unordered_map<std::string, Font::Ptr> m_fontCache;
            
            std::unordered_map<std::string, Audio::AudioBuffer::Ptr> m_audioCache;
        };
    }
}