// Renderer/SDLRenderer.hpp
#pragma once
#include "IRenderer.hpp"
#include <memory>

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

            void drawTexture(const std::string& texturePath, 
                const Utils::Vector2& position, const Utils::Vector2& scale = { 1, 1 }, float rotation = 0.0f) override;

            void drawRect(const Utils::Vector2& position, const Utils::Vector2& size,
                const Color& color, float rotation = 0.0f) override;

            void drawCircle(const Utils::Vector2& center, float radius, const Color& color) override;

            void drawLine(const Utils::Vector2& start, const Utils::Vector2& end, const Color& color) override;

            void drawText(const std::string& text, const Utils::Vector2& position,
                const Color& color, const std::string& font, int fontSize = 16) override;

            void drawStrokeText(const std::string& text, const Utils::Vector2& position,
                const Color& fillColor, const Color& strokeColor,
                const std::string& font, int fontSize = 16, float strokeWidth = 1.0f) override;
            
        private:
            SDL_Renderer* m_renderer = nullptr; 
        };
    }
}