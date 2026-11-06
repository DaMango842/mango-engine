#pragma once

#include <Utils/Vector2.hpp>

namespace mango_engine {
    namespace Physics {
        class AABB {
        public:
            AABB(const Utils::Vector2& min, const Utils::Vector2& max)
                : m_min(min), m_max(max) {}

            const Utils::Vector2& GetMin() const { return m_min; }
            const Utils::Vector2& GetMax() const { return m_max; }

            bool Intersects(const AABB& other) const {
                return (m_min.x <= other.m_max.x && m_max.x >= other.m_min.x) &&
                       (m_min.y <= other.m_max.y && m_max.y >= other.m_min.y);
            }

        private:
            Utils::Vector2 m_min;
            Utils::Vector2 m_max;
        };
    }
}