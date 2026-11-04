#include "Renderer/SDLRenderer.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


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
            const std::string& fontPath, int fontSize) {
            if (!m_renderer || text.empty()) return;

            std::string fontKey = fontPath + "_" + std::to_string(fontSize);

            if (m_fontCache.find(fontKey) == m_fontCache.end()) {
                m_fontCache[fontKey] = std::make_shared<Resource::Font>(fontPath, fontSize);
            }
            auto font = m_fontCache[fontKey];

            std::string textKey = text + "_" + std::to_string(color.r) + std::to_string(color.g) + 
                                std::to_string(color.b) + "_" + fontKey;
            
            SDL_Texture* texture = nullptr;
            float texW = 0, texH = 0;

            if (m_textCache.find(textKey) == m_textCache.end()) {
                SDL_Color sdlColor = { 
                    static_cast<Uint8>(color.r * 255), static_cast<Uint8>(color.g * 255), 
                    static_cast<Uint8>(color.b * 255), static_cast<Uint8>(color.a * 255) 
                };

                SDL_Surface* surface = TTF_RenderText_Blended(font->GetRaw(), text.c_str(), 0, sdlColor);
                if (surface) {
                    texture = SDL_CreateTextureFromSurface(m_renderer, surface);
                    texW = static_cast<float>(surface->w);
                    texH = static_cast<float>(surface->h);
                    SDL_DestroySurface(surface);

                    m_textCache[textKey] = texture;
                } else {
                    SDL_Log("Failed to render text surface: %s", SDL_GetError());
                    return;
                }
            } else {
                texture = m_textCache[textKey];
                if (!SDL_GetTextureSize(texture, &texW, &texH)) {
                    SDL_Log("Failed to get texture size: %s", SDL_GetError());
                    return;
                }
            }

            SDL_FRect dstRect = { position.x, position.y, texW, texH };
            SDL_RenderTexture(m_renderer, texture, nullptr, &dstRect);   
        }

        void SDLRenderer::drawStrokeText(const std::string& text, const Utils::Vector2& position,
            const Color& fillColor, const Color& strokeColor,
            const std::string& fontPath, int fontSize, float strokeWidth) {
            if (!m_renderer || text.empty()) return;

            std::string fontKey = fontPath + "_" + std::to_string(fontSize);
            if (m_fontCache.find(fontKey) == m_fontCache.end()) {
                m_fontCache[fontKey] = std::make_shared<Resource::Font>(fontPath, fontSize);
            }
            auto font = m_fontCache[fontKey];

            TTF_SetFontOutline(font->GetRaw(), static_cast<int>(strokeWidth));
            SDL_Color sdlStrokeColor = {            
                static_cast<Uint8>(strokeColor.r * 255), static_cast<Uint8>(strokeColor.g * 255),
                static_cast<Uint8>(strokeColor.b * 255), static_cast<Uint8>(strokeColor.a * 255)
            };
            SDL_Surface* strokeSurface = TTF_RenderText_Blended(font->GetRaw(), text.c_str(), 0, sdlStrokeColor);

            TTF_SetFontOutline(font->GetRaw(), 0);

            SDL_Color sdlFillColor = {
                static_cast<Uint8>(fillColor.r * 255), static_cast<Uint8>(fillColor.g * 255),
                static_cast<Uint8>(fillColor.b * 255), static_cast<Uint8>(fillColor.a * 255)
            };
            SDL_Surface* fillSurface = TTF_RenderText_Blended(font->GetRaw(), text.c_str(), 0, sdlFillColor);

            if (strokeSurface && fillSurface) {
                SDL_Texture* strokeTex = SDL_CreateTextureFromSurface(m_renderer, strokeSurface);
                SDL_Texture* fillTex = SDL_CreateTextureFromSurface(m_renderer, fillSurface);
                
                float strokeW = static_cast<float>(strokeSurface->w);
                float strokeH = static_cast<float>(strokeSurface->h);
                float fillW = static_cast<float>(fillSurface->w);
                float fillH = static_cast<float>(fillSurface->h);

                float offsetX = (strokeW - fillW) / 2.0f;
                float offsetY = (strokeH - fillH) / 2.0f;

                SDL_FRect strokeRect = { position.x, position.y, strokeW, strokeH };
                SDL_FRect fillRect = { position.x + offsetX, position.y + offsetY, fillW, fillH };

                SDL_RenderTexture(m_renderer, strokeTex, nullptr, &strokeRect);
                SDL_RenderTexture(m_renderer, fillTex, nullptr, &fillRect);

                SDL_DestroyTexture(strokeTex);
                SDL_DestroyTexture(fillTex);
            } else {
                SDL_Log("Failed to render text surfaces: %s", SDL_GetError());
            }

            if (strokeSurface) SDL_DestroySurface(strokeSurface);
            if (fillSurface) SDL_DestroySurface(fillSurface);

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