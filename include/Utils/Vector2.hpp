// include/Utils/Vector2.hpp
#pragma once
#include <cmath>
#include <algorithm> // for std::clamp
#include <stdexcept>

namespace mango_engine {
    namespace Utils {
        struct Vector2 {
            float x = 0.0f;
            float y = 0.0f;

            Vector2() = default;
            Vector2(float x, float y) : x(x), y(y) {}

            // --- 运算符 ---
            Vector2 operator+(const Vector2& v) const { return { x + v.x, y + v.y }; }
            Vector2 operator-(const Vector2& v) const { return { x - v.x, y - v.y }; }
            Vector2 operator*(float s) const { return { x * s, y * s }; }
            Vector2 operator/(float s) const { return { x / s, y / s }; }

            Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
			Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
			Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
			Vector2& operator/=(float s) { x /= s; y /= s; return *this; }

            bool operator==(const Vector2& v) const {
                return std::abs(x - v.x) < 1e-5f && std::abs(y - v.y) < 1e-5f;
            }

			// --- 下标访问 ---
            float& operator[](size_t i) {
                if (i == 0) return x;
				if (i == 1) return y;
				throw std::out_of_range("Vector2 index out of range");
			}

            const float& operator[](size_t i) const {
				if (i == 0) return x;
				if (i == 1) return y;
				throw std::out_of_range("Vector2 index out of range");
			}


            // --- 方法 ---
            float magnitude() const { return std::sqrt(x * x + y * y); }
            float sqrMagnitude() const { return x * x + y * y; }

            void normalize() {
                float len = magnitude();
                if (len > 0) { x /= len; y /= len; }
            }

            Vector2 normalized() const {
                Vector2 v(*this);
                v.normalize();
                return v;
            }

            static Vector2 lerp(const Vector2& a, const Vector2& b, float t) {
                return a + (b - a) * t;
            }

            float distance(const Vector2& other) const {
                return (*this - other).magnitude();
            }

            float dot(const Vector2& other) const {
                return x * other.x + y * other.y;
            }

            float cross(const Vector2& other) const {
                return x * other.y - y * other.x;
            }

            float angle(const Vector2& other) const {
                float len = magnitude() * other.magnitude();
                if (len <= 0) return 0.0f;
                float cosTheta = dot(other) / len;
                cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);
                return std::acos(cosTheta);
            }
        };
    } // namespace Utils
} // namespace MangoEngine