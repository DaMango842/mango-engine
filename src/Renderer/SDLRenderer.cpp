#include "Renderer/SDLRenderer.hpp"
#include <SDL3/SDL.h>
#include <vector>
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

            setDrawColor(color);
            
            SDL_FRect rect = { position.x, position.y, size.x, size.y };
            SDL_RenderFillRect(m_renderer, &rect);
        }

        void SDLRenderer::drawCircle(const Utils::Vector2& center, float radius, const Color& color) {
            if (!m_renderer || radius <= 0.f) return;

            setDrawColor(color);
            
            const int segments = 48;
            std::vector<SDL_Vertex> vert;
            
            for (int i = 0; i < segments; ++i) {
                float theta = 2.0f * 3.1415926f * float(i) / float(segments);
                float x = radius * cosf(theta);
                float y = radius * sinf(theta);
                SDL_Vertex vertex;
                vertex.position.x = center.x + x;
                vertex.position.y = center.y + y;
                vertex.color.r = static_cast<Uint8>(color.r * 255);
                vertex.color.g = static_cast<Uint8>(color.g * 255);
                vertex.color.b = static_cast<Uint8>(color.b * 255);
                vertex.color.a = static_cast<Uint8>(color.a * 255);
                vert.push_back(vertex);
            }

            SDL_RenderGeometry(m_renderer, nullptr, vert.data(), vert.size(), nullptr, 0);

        }

        void SDLRenderer::drawLine(const Utils::Vector2& start, const Utils::Vector2& end, const Color& color) {
            if (!m_renderer) return;
            setDrawColor(color);
            SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
        }
        
        void SDLRenderer::drawTexture(const Resource::Texture::Ptr& texture, const Utils::Vector2& position,
            const Utils::Vector2& scale, float rotation, const SDL_FRect* clip) {
                if (!m_renderer || !texture) return;

                SDL_FRect dstRect;
                dstRect.x = position.x;
                dstRect.y = position.y;

                if (clip) {
                    dstRect.w = clip->w * scale.x;
                    dstRect.h = clip->h * scale.y;
                } else {
                    dstRect.w = texture->GetWidth() * scale.x;
                    dstRect.h = texture->GetHeight() * scale.y;
                }

                if (rotation != 0.0f) {
                    SDL_FPoint center = { static_cast<int>(dstRect.w / 2), static_cast<int>(dstRect.h / 2) };
                    SDL_RenderTextureRotated(m_renderer, texture->GetRaw(), clip, &dstRect, rotation, &center, SDL_FLIP_NONE);
                } else {
                    SDL_RenderTexture(m_renderer, texture->GetRaw(), clip, &dstRect); 
                }
        }

        void SDLRenderer::drawText(const std::string& text, const Utils::Vector2& position, const Color& color,
            const std::string& font, int fontSize) {
            
        }

        void SDLRenderer::drawStrokeText(const std::string& text, const Utils::Vector2& position,
            const Color& fillColor, const Color& strokeColor,
            const std::string& font, int fontSize, float strokeWidth) {
            
        }

        void SDLRenderer::setDrawColor(const Color& color) {
            if (!m_renderer) return;
            SDL_SetRenderDrawColor(m_renderer, 
                static_cast<Uint8>(color.r * 255), 
                static_cast<Uint8>(color.g * 255), 
                static_cast<Uint8>(color.b * 255), 
                static_cast<Uint8>(color.a * 255));
        }

    } // namespace Renderer
} // namespace mango_engine