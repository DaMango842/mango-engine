#include "Renderer/SDLRenderer.hpp"
#include <SDL3/SDL.h>
#include <iostream>

namespace mango_engine {
    namespace Renderer {

        bool SDLRenderer::init(void* windowHandle) {
            if (!windowHandle) return false;

            SDL_Window* window = static_cast<SDL_Window*>(windowHandle);

            m_renderer = SDL_CreateRenderer(window, nullptr);
            if (!m_renderer) {
                std::cerr << "Failed to create SDL Renderer: " << SDL_GetError() << std::endl;
                return false;
            }
            return true;
        }

        void SDLRenderer::shutdown() {
            if (m_renderer) {
                SDL_DestroyRenderer(m_renderer);
                m_renderer = nullptr;
            }
        }

        void SDLRenderer::beginFrame() {
            // 可以在这里设置渲染目标、清除缓冲区等
        }

        void SDLRenderer::endFrame() {
            SDL_RenderPresent(m_renderer);
        }

        void SDLRenderer::clear(const Color& color) {
            if (!m_renderer) return;
            SDL_SetRenderDrawColor(m_renderer, 
                static_cast<Uint8>(color.r * 255), 
                static_cast<Uint8>(color.g * 255), 
                static_cast<Uint8>(color.b * 255), 
                static_cast<Uint8>(color.a * 255));
            SDL_RenderClear(m_renderer);
        }



        void SDLRenderer::drawRect(const Utils::Vector2& position, const Utils::Vector2& size,
            const Color& color, float rotation) {
            if (!m_renderer) return;

            SDL_SetRenderDrawColor(m_renderer, 
                static_cast<Uint8>(color.r * 255), 
                static_cast<Uint8>(color.g * 255), 
                static_cast<Uint8>(color.b * 255), 
                static_cast<Uint8>(color.a * 255));
            
            SDL_FRect rect = { position.x, position.y, size.x, size.y };
            SDL_RenderFillRect(m_renderer, &rect);
        }

        void SDLRenderer::drawCircle(const Utils::Vector2& center, float radius, const Color& color) {
            
        }

        void SDLRenderer::drawLine(const Utils::Vector2& start, const Utils::Vector2& end, const Color& color) {
            if (!m_renderer) return;
            SDL_SetRenderDrawColor(m_renderer, 
                static_cast<Uint8>(color.r * 255), 
                static_cast<Uint8>(color.g * 255), 
                static_cast<Uint8>(color.b * 255), 
                static_cast<Uint8>(color.a * 255));
            SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
        }
        
        void SDLRenderer::drawTexture(const std::string& texturePath,
            const Utils::Vector2& position, const Utils::Vector2& scale, float rotation) {
            
        }

        void SDLRenderer::drawText(const std::string& text, const Utils::Vector2& position, const Color& color,
            const std::string& font, int fontSize) {
            
        }

        void SDLRenderer::drawStrokeText(const std::string& text, const Utils::Vector2& position,
            const Color& fillColor, const Color& strokeColor,
            const std::string& font, int fontSize, float strokeWidth) {
            
        }

    } // namespace Renderer
} // namespace MangoEngine