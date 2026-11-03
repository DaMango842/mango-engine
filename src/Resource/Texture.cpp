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
        
    }
}