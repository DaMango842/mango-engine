// Renderer/SDLRenderer.hpp
#pragma once
#include "IRenderer.hpp"
#include <memory>
#include <unordered_map>
#include <Resource/Font.hpp>

// 前向声明 SDL_Renderer
struct SDL_Renderer;

namespace mango_engine {
    namespace Renderer {

        class SDLRenderer : public IRenderer {
        public:
            SDLRenderer() = default;
            ~SDLRenderer() override = default;

            bool init(void* windowHandle) override;
            void shutdown() override;

            void beginFrame() override;
            void endFrame() override;

            void clear(const Color& color) override;

            void drawTexture(const Resource::Texture::Ptr& texture, const Utils::Vector2& position,
                const Utils::Vector2& scale = { 1, 1 }, float rotation = 0.0f, const SDL_FRect* clip = nullptr) override;

            void drawRect(const Utils::Vector2& position, const Utils::Vector2& size,
                const Color& color, float rotation = 0.0f) override;

            void drawCircle(const Utils::Vector2& center, float radius, const Color& color) override;

            void drawLine(const Utils::Vector2& start, const Utils::Vector2& end, const Color& color) override;

            void drawText(const std::string& text, const Utils::Vector2& position,
                const Color& color, const std::string& fontPath, int fontSize = 16) override;

            void drawStrokeText(const std::string& text, const Utils::Vector2& position,
                const Color& fillColor, const Color& strokeColor,
                const std::string& fontPath, int fontSize = 16, float strokeWidth = 1.0f) override;

            
            SDL_Renderer* GetRaw() const { return m_renderer; }
            
        private:
            void setDrawColor(const Color& color);
        private:
            SDL_Renderer* m_renderer{nullptr};
            
            // 字体缓存
            std::unordered_map<std::string, std::shared_ptr<Resource::Font>> m_fontCache;
            // 文字纹理缓存
            std::unordered_map<std::string, SDL_Texture*> m_textCache;
            
        };
    }
}