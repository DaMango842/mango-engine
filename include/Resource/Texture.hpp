#pragma once

#include <SDL3_image/SDL_image.h>

#include <string>
#include <stdexcept>
#include <memory>


namespace mango_engine {
    namespace Resource {
        struct SDL_TextureDeleter {
            void operator()(SDL_Texture* texture) const {
                if (texture) SDL_DestroyTexture(texture);
            }
        };

        class Texture {
        public:
            using Ptr = std::shared_ptr<Texture>;

            Texture(SDL_Texture* texture, int width, int height);
            ~Texture() = default;

            Texture(const Texture&) = delete;
            Texture& operator=(const Texture&) = delete;
            Texture(Texture&&) = default;
            Texture& operator=(Texture&&) = default;

            int GetWidth() const { return m_width; }
            int GetHeight() const { return m_height; }

            SDL_Texture* GetRaw() const { return m_texture.get(); }

        private:
            std::unique_ptr<SDL_Texture, SDL_TextureDeleter> m_texture{nullptr};
            int m_width{0};
            int m_height{0};
        };
    }
}