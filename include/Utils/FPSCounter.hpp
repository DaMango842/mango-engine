#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace mango_engine {
    namespace Utils {
        class FPSCounter {
        public:
            FPSCounter() {
                // 获取初始频率和计数器
                m_freq = SDL_GetPerformanceFrequency();
                m_lastTime = SDL_GetPerformanceCounter();
            }

            void Update() {
                Uint64 currentTime = SDL_GetPerformanceCounter();
                m_frameCount++;

                // 计算两次更新之间的时间（秒）
                float elapsed = (float)(currentTime - m_lastTime) / (float)m_freq;

                // 如果超过 1 秒（或者 0.5 秒），更新一次 FPS 显示
                if (elapsed >= 0.5f) {
                    m_fps = (float)m_frameCount / elapsed;
                    m_frameCount = 0;
                    m_lastTime = currentTime;
                    
                    // 把浮点数转成字符串，保留 1 位小数
                    m_displayString = "FPS: " + std::to_string(static_cast<int>(m_fps));
                }
            }

            const std::string& GetDisplayString() const { return m_displayString; }

        private:
            Uint64 m_freq;
            Uint64 m_lastTime;
            int m_frameCount = 0;
            float m_fps = 0.0f;
            std::string m_displayString = "FPS: 0";
        };
    }
}