// include/Renderer/IRenderer.hpp
#pragma once
#include <Utils/Vector2.hpp>
#include <Resource/Texture.hpp>
#include <string>

namespace mango_engine {
    namespace Renderer {

        struct Color {
            float r = 1, g = 1, b = 1, a = 1;
        };

        class IRenderer {
        public:
            virtual ~IRenderer() = default;

            virtual bool init(void* windowHandle) = 0; // 接收 SDL_Window*
            virtual void shutdown() = 0;

            virtual void beginFrame() = 0;
            virtual void endFrame() = 0;

            virtual void clear(const Color& color) = 0;

            // 2D 渲染接口
            virtual void drawTexture(const Resource::Texture::Ptr& texture, const Utils::Vector2& position,
                const Utils::Vector2& scale = { 1, 1 }, float rotation = 0.0f, const SDL_FRect* clip = nullptr) = 0;

            virtual void drawRect(const Utils::Vector2& position, const Utils::Vector2& size,
                const Color& color, float rotation = 0.0f) = 0;

            virtual void drawCircle(const Utils::Vector2& center, float radius, const Color& color) = 0;

            virtual void drawLine(const Utils::Vector2& start, const Utils::Vector2& end, const Color& color) = 0;

            virtual void drawText(const std::string& text, const Utils::Vector2& position, const Color& color, 
                const std::string& font, int fontSize = 16) = 0;

            virtual void drawStrokeText(const std::string& text, const Utils::Vector2& position,
                const Color& fillColor, const Color& strokeColor,
                const std::string& font, int fontSize = 16, float strokeWidth = 1.0f) = 0;
            
        };
	} // namespace Renderer
} // namespace MangoEngine