#include "Resource/Texture.hpp"
#include <mutex>

namespace mango_engine {
    namespace Resource {

        Texture::Texture(SDL_Texture* texture, int width, int height)
            : m_texture(texture), m_width(width), m_height(height) {
            if (!texture) {
                throw std::runtime_error("Failed to create texture: " + std::string(SDL_GetError()));
            }
        }

        Texture::Ptr Texture::LoadFromFile(SDL_Renderer* renderer, const std::string& path) {
            if (!renderer) return nullptr;

            SDL_Surface* surface = IMG_Load(path.c_str());
            if (!surface) {
                SDL_Log("Failed to load image: '%s': %s", path.c_str(), SDL_GetError());
                return nullptr;
            }

            SDL_Texture* rawTexture = SDL_CreateTextureFromSurface(renderer, surface);
            int w = surface->w;
            int h = surface->h;

            SDL_DestroySurface(surface);

            if (!rawTexture) {
                SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
                return nullptr;
            }

            return std::make_shared<Texture>(rawTexture, w, h);
        }

    }
}