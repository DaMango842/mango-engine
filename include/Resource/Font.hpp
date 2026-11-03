// include/Font/Font.hpp
#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <stdexcept>
#include <memory>

namespace mango_engine {
	namespace Resource {
		struct TTF_FontDeleter {
			void operator()(TTF_Font* font) const {
				if (font) TTF_CloseFont(font);
			}
		};

		class Font {
		public:
			Font(const std::string& fontPath, int size = 20);
			~Font() = default;

			TTF_Font* get() const { return m_font.get(); }

		private:
			std::unique_ptr<TTF_Font, TTF_FontDeleter> m_font{nullptr};
		};
	}
}