#pragma once
#include <Utils/Vector2.hpp>

namespace mango_engine {
    namespace Scene {
        class Camera {
        public:
            Camera(float viewportW, float viewportH)
                : m_viewportWidth(viewportW), m_viewportHeight(viewportH) {
                // 初始位置：如果是用左上角坐标，一般从 (0,0) 开始
                m_position = { 0.0f, 0.0f }; 
            }

            // 跟随目标（传入目标世界坐标，平滑插值）
            void Follow(const Utils::Vector2& targetPos, float deltaTime, float smoothSpeed = 5.0f) {
                // 目标是让摄像机中心对准物体
                float targetX = targetPos.x - m_viewportWidth / 2.0f;
                float targetY = targetPos.y - m_viewportHeight / 2.0f;

                // 线性插值（Lerp），让镜头移动有一种“平滑跟拍”的阻尼感
                m_position.x += (targetX - m_position.x) * smoothSpeed * deltaTime;
                m_position.y += (targetY - m_position.y) * smoothSpeed * deltaTime;

                // （可选）边界限制：这里假设你的地图范围是 0 到 2000x2000
                // 为了防止镜头看到地图外面的黑边，可以把摄像机坐标限制住
                // m_position.x = std::clamp(m_position.x, 0.0f, 2000.0f - m_viewportWidth);
                // m_position.y = std::clamp(m_position.y, 0.0f, 2000.0f - m_viewportHeight);
            }

            // 获取摄像机的左上角坐标（给渲染器计算用）
            Utils::Vector2 GetPosition() const { return m_position; }

        private:
            Utils::Vector2 m_position; // 摄像机左上角在世界中的坐标
            float m_viewportWidth;
            float m_viewportHeight;
        };
    }
}