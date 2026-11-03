// MangoPtr SDL 兼容层
// 注: 本兼容层只支持SDL3，并且不包含SDL2的兼容性支持
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <Utils/MangoPtr.hpp>

// SDL 资源的自定义删除器
struct SDL_Window_Deleter {
	void operator()(SDL_Window* ptr) const {
		if (ptr) {
			SDL_DestroyWindow(ptr);
		}
	}
};

struct SDL_Renderer_Deleter {
	void operator()(SDL_Renderer* ptr) const {
		if (ptr) {
			SDL_DestroyRenderer(ptr);
		}
	}
};

struct SDL_Texture_Deleter {
	void operator()(SDL_Texture* ptr) const {
		if (ptr) {
			SDL_DestroyTexture(ptr);
		}
	}
};

// SDL3_ttf
struct SDL_Font_Deleter {
	void operator()(TTF_Font* ptr) const {
		if (ptr) {
			TTF_CloseFont(ptr);
		}
	}
};

using MangoSDLWindow = MangoPtr<SDL_Window, SDL_Window_Deleter>;
using MangoSDLRenderer = MangoPtr<SDL_Renderer, SDL_Renderer_Deleter>;
using MangoSDLTexture = MangoPtr<SDL_Texture, SDL_Texture_Deleter>;
using MangoSDLFont = MangoPtr<TTF_Font, SDL_Font_Deleter>;

MangoSDLWindow make_sdl_window(const char* title, 
	int width, int height, 
	Uint32 flags) {
	SDL_Window* window = SDL_CreateWindow(title, width, height, flags);
	if (!window) {
		throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
	}
	return MangoSDLWindow(window);
}

MangoSDLRenderer make_sdl_renderer(
	SDL_Window* window, 
	const char* name = nullptr) {
	SDL_Renderer* renderer = SDL_CreateRenderer(window, name);
	if (!renderer) {
		throw std::runtime_error("Failed to create SDL Renderer: " + std::string(SDL_GetError()));
	}
	return MangoSDLRenderer(renderer);
}

MangoSDLTexture make_sdl_texture(
	SDL_Renderer* renderer, 
	const SDL_PixelFormat& format, 
	const SDL_TextureAccess access,
	int w, int h) {
	SDL_Texture* texture = SDL_CreateTexture(renderer, format, access, w, h);
	if (!texture) {
		throw std::runtime_error("Failed to create SDL Texture: " + std::string(SDL_GetError()));
	}
	return MangoSDLTexture(texture);
}

MangoSDLFont make_sdl_font(const char* file, int ptsize) {
	TTF_Font* font = TTF_OpenFont(file, ptsize);
	if (!font) {
		throw std::runtime_error("Failed to open TTF Font: " + std::string(SDL_GetError()));
	}
	return MangoSDLFont(font);
}