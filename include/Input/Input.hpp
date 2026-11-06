#pragma once
#include <Input/KeyCode.hpp>
#include <Input/KeyAction.hpp>
#include <array>

namespace mango_engine {
    namespace Input {
        class Input {
        public:
            // 你想要的接口
            static void SetKeyState(KeyCode key, KeyAction action);

            static bool IsKeyDown(KeyCode key);     // 当前是否处于按下状态
            static bool IsKeyPressed(KeyCode key);  // 这一帧刚刚按下
            static bool IsKeyReleased(KeyCode key); // 这一帧刚刚松开

            // 每帧末尾调用，清理上一帧状态
            static void EndFrame();

            // 供 Application 调用的底层转换接口
            static KeyCode ConvertSDLKey(int sdlKey);

        private:
            // 用枚举的 Count 大小来开数组 (确保你在 KeyCode 末尾加了 Count)
            static constexpr int KeyCount = static_cast<int>(KeyCode::Count);
            
            // 用三个布尔数组记录状态，查表法，性能极高
            static std::array<bool, KeyCount> s_CurrentState;
            static std::array<bool, KeyCount> s_PreviousState;
        };
    }
}