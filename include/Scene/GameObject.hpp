// include/Scene/GameObject.hpp
#pragma once
#include <Utils/Vector2.hpp>
#include <Renderer/IRenderer.hpp>
#include <SDL3/SDL.h>

namespace mango_engine {
    namespace Scene {
        class GameObject {
        public:
            GameObject(const Utils::Vector2& pos, const Utils::Vector2& sz, const Renderer::Color& col, float spd)
                : m_position(pos), m_size(sz), m_color(col), speed(spd) {}
            
            void Update(float deltaTime, const bool* keyStates) {
                if (keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP]) m_position.y -= speed * deltaTime;
                if (keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN]) m_position.y += speed * deltaTime;
                if (keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT]) m_position.x -= speed * deltaTime;
                if (keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT]) m_position.x += speed * deltaTime;
            }

            void Draw(Renderer::IRenderer& renderer) {
                renderer.drawRect(m_position, m_size, m_color);
            }

        private:
            Utils::Vector2 m_position;
            Utils::Vector2 m_size;
            Renderer::Color m_color;
            float speed; // 每秒移动的像素数
        };
    }
}