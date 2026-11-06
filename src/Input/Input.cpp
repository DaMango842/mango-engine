#include <Input/Input.hpp>
#include <SDL3/SDL.h>

namespace mango_engine {
    namespace Input {

        std::array<bool, Input::KeyCount> Input::s_CurrentState{};
        std::array<bool, Input::KeyCount> Input::s_PreviousState{};

        // 极简转换，利用 SDL3 的 ASCII 特性
        KeyCode Input::ConvertSDLKey(int sdlKey) {
            // 字母键
            if (sdlKey >= SDLK_A && sdlKey <= SDLK_Z) {
                return static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (sdlKey - SDLK_A));
            }
            // 数字键
            if (sdlKey >= SDLK_0 && sdlKey <= SDLK_9) {
                return static_cast<KeyCode>(static_cast<int>(KeyCode::Num0) + (sdlKey - SDLK_0));
            }
            
            // 只需要写你真正用得到的特殊键
            switch (sdlKey) {
                case SDLK_ESCAPE: return KeyCode::Escape;
                case SDLK_SPACE:  return KeyCode::Space;
                case SDLK_RETURN: return KeyCode::Enter;
                case SDLK_UP:     return KeyCode::Up;
                case SDLK_DOWN:   return KeyCode::Down;
                case SDLK_LEFT:   return KeyCode::Left;
                case SDLK_RIGHT:  return KeyCode::Right;
                default:          return KeyCode::Unknown;
            }
        }

        void Input::SetKeyState(KeyCode key, KeyAction action) {
            if (key == KeyCode::Unknown) return;
            
            int idx = static_cast<int>(key);
            if (action == KeyAction::Press) {
                s_CurrentState[idx] = true;
            } else if (action == KeyAction::Release) {
                s_CurrentState[idx] = false;
            }
        }

        void Input::Update() {
            s_PreviousState = s_CurrentState; // 帧末备份
        }

        bool Input::IsKeyDown(KeyCode key) {
            return s_CurrentState[static_cast<int>(key)];
        }

        bool Input::IsKeyPressed(KeyCode key) {
            int idx = static_cast<int>(key);
            // 当前按着，且上一帧没按
            return s_CurrentState[idx] && !s_PreviousState[idx]; 
        }

        bool Input::IsKeyReleased(KeyCode key) {
            int idx = static_cast<int>(key);
            // 当前没按，且上一帧按着
            return !s_CurrentState[idx] && s_PreviousState[idx];
        }
    }
}