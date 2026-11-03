#include "Resource/Font.hpp"
#include <mutex>

namespace mango_engine {
	namespace Resource {
		Font::Font(const std::string& fontPath, int size) {
			static std::once_flag ttfInitFlag;
			std::call_once(ttfInitFlag, []() {
				if (!TTF_Init()) {
					throw std::runtime_error("Failed to initialize SDL_ttf: " + std::string(SDL_GetError()));
				}
			});

			TTF_Font* rawFont = TTF_OpenFont(fontPath.c_str(), size);
			if (!rawFont) {
				throw std::runtime_error("Failed to load font: " + fontPath + " - " + std::string(SDL_GetError()));
			}
			m_font.reset(rawFont);
		}
	}
}