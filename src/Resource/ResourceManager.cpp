#include <Resource/ResourceManager.hpp>
#include <Loader/AudioLoader.hpp>
#include <SDL3/SDL.h>


namespace mango_engine {
    namespace Resource {

        ResourceManager::ResourceManager(SDL_Renderer* renderer, MIX_Mixer* mixer)
            : m_renderer(renderer), m_mixer(mixer) {
        }

        Texture::Ptr ResourceManager::GetTexture(const std::string& path) {
            // 1. 去缓存里找
            auto it = m_textureCache.find(path);
            if (it != m_textureCache.end()) {
                return it->second; // 找到了，直接返回！
            }

            // 2. 没找到，去硬盘加载
            Texture::Ptr tex = Texture::LoadFromFile(m_renderer, path);
            if (tex) {
                m_textureCache[path] = tex; // 塞进缓存
            }
            return tex;
        }

        Font::Ptr ResourceManager::GetFont(const std::string& path, int size) {
            // 字体要拼一个带字号的 Key
            std::string key = path + "_" + std::to_string(size);

            auto it = m_fontCache.find(key);
            if (it != m_fontCache.end()) {
                return it->second;
            }

            Font::Ptr font = std::make_shared<Font>(path, size);
            m_fontCache[key] = font;
            return font;
        }

        Audio::AudioBuffer::Ptr ResourceManager::GetAudio(const std::string& path) {
            auto it = m_audioCache.find(path);
            if (it != m_audioCache.end()) {
                return it->second;
            }

            // 注意：这里调用你之前写的 AudioLoader 工厂或直接加载
            // 假设你保留了 AudioLoader
            auto audio = Loader::AudioLoader::Load(path); 
            if (audio) {
                m_audioCache[path] = audio;
            }
            return audio;
        }

        void ResourceManager::ClearCache() {
            m_textureCache.clear();
            m_fontCache.clear();
            m_audioCache.clear();
            SDL_Log("ResourceManager cache cleared.");
        }
    }
}