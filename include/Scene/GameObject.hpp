// include/Scene/GameObject.hpp
#pragma once
#include <Utils/Vector2.hpp>
#include <Renderer/IRenderer.hpp>
#include <Resource/Texture.hpp>
#include <Scene/Animator.hpp>
#include <SDL3/SDL.h>
#include <string>

namespace mango_engine {
    namespace Scene {

        // 【新增】定义四个方向
        enum class Direction {
            Down,
            Left,
            Right,
            Up
        };

        class GameObject {
        public:
            GameObject(const Utils::Vector2& pos, const Utils::Vector2& sz, const Renderer::Color& col, float spd)
                : m_position(pos), m_size(sz), m_color(col), speed(spd) 
            {
                // 【关键】根据方向注册动画 (假设 4x4 排版)
                // 朝下: 0-3 帧
                m_animator.AddAnimationClip(AnimationClip("walk_down", 0, 3, 0.15f, true));
                m_animator.AddAnimationClip(AnimationClip("idle_down", 0, 0, 0.1f, true));
                
                // 朝左: 4-7 帧
                m_animator.AddAnimationClip(AnimationClip("walk_left", 4, 7, 0.15f, true));
                m_animator.AddAnimationClip(AnimationClip("idle_left", 4, 4, 0.1f, true));

                // 朝右: 8-11 帧
                m_animator.AddAnimationClip(AnimationClip("walk_right", 8, 11, 0.15f, true));
                m_animator.AddAnimationClip(AnimationClip("idle_right", 8, 8, 0.1f, true));

                // 朝上: 12-15 帧
                m_animator.AddAnimationClip(AnimationClip("walk_up", 12, 15, 0.15f, true));
                m_animator.AddAnimationClip(AnimationClip("idle_up", 12, 12, 0.1f, true));
                
                m_animator.PlayAnimation("idle_down"); // 初始朝下站着
            }
            
            void Update(float deltaTime, const bool* keyStates) {
                bool isMoving = false;

                // 优先级判定：先判断垂直，再判断水平，防止同时按出对角线导致方向闪烁
                if (isPlayer) { // 只有玩家对象才响应输入
                    if (keyStates[SDL_SCANCODE_W] || keyStates[SDL_SCANCODE_UP]) {
                        m_position.y -= speed * deltaTime; 
                        m_direction = Direction::Up; 
                        isMoving = true;
                    } 
                    else if (keyStates[SDL_SCANCODE_S] || keyStates[SDL_SCANCODE_DOWN]) {
                        m_position.y += speed * deltaTime; 
                        m_direction = Direction::Down; 
                        isMoving = true;
                    }

                    if (keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT]) {
                        m_position.x -= speed * deltaTime; 
                        m_direction = Direction::Left; 
                        isMoving = true;
                    } 
                    else if (keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT]) {
                        m_position.x += speed * deltaTime; 
                        m_direction = Direction::Right; 
                        isMoving = true;
                    }
                }

                // 【核心】根据移动状态 + 当前方向，拼接动画名字
                std::string action = isMoving ? "walk_" : "idle_";
                std::string dirStr = "";
                
                switch (m_direction) {
                    case Direction::Up:    dirStr = "up"; break;
                    case Direction::Down:  dirStr = "down"; break;
                    case Direction::Left:  dirStr = "left"; break;
                    case Direction::Right: dirStr = "right"; break;
                }

                m_animator.PlayAnimation(action + dirStr); // 比如 "walk_up", "idle_left"
                m_animator.Update(deltaTime);
            }

            void Draw(Renderer::IRenderer& renderer) {
                if (m_texture) {
                    float frameW = static_cast<float>(m_texture->GetWidth()) / columns;
                    float frameH = static_cast<float>(m_texture->GetHeight()) / rows;

                    int currentFrame = m_animator.GetCurrentFrame();
                    int col = currentFrame % columns;
                    int row = currentFrame / columns;

                    SDL_FRect clip = { col * frameW, row * frameH, frameW, frameH };

                    renderer.drawTexture(m_texture, m_position, { 1.0f, 1.0f }, 0.0f, &clip);
                } else {
                    renderer.drawRect(m_position, m_size, m_color);
                }
            }
        
        public:
            void SetTexture(const Resource::Texture::Ptr& tex) { m_texture = tex; }
            Resource::Texture::Ptr GetTexture() const { return m_texture; }
            
            
        public:
            bool isPlayer = false; // 【新增】标记是否为玩家对象
            
        private:
            Resource::Texture::Ptr m_texture{nullptr};
            Utils::Vector2 m_position;
            Utils::Vector2 m_size;
            Renderer::Color m_color;
            float speed; 

            Animator m_animator;
            Direction m_direction = Direction::Down; // 【新增】记录当前朝向，默认朝下

            int columns{4};
            int rows{4};

            
        };
    }
}