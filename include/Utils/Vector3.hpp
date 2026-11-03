// include/Utils/Vector3.hpp
#pragma once
#include <cmath>
#include <algorithm> // for std::clamp
#include <stdexcept>

namespace mango_engine {
    namespace Utils {
        struct Vector3 {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;

            // --- 构造函数 ---
            Vector3() = default;
            Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

            // --- 运算符重载 ---
            Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
            Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
            Vector3 operator*(float s) const { return { x * s, y * s, z * s }; }
            Vector3 operator/(float s) const {
                // 可选：加断言或 epsilon 检查
                return { x / s, y / s, z / s };
            }

            Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
            Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
            Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
            Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }

            bool operator==(const Vector3& v) const {
                constexpr float eps = 1e-5f;
                return std::abs(x - v.x) < eps &&
                    std::abs(y - v.y) < eps &&
                    std::abs(z - v.z) < eps;
            }

			// --- 下标访问 ---
            float& operator[](size_t i) {
                if (i == 0) return x;
                if (i == 1) return y;
                if (i == 2) return z;
                throw std::out_of_range("Vector3 index out of range");
			}

            const float& operator[](size_t i) const {
                if (i == 0) return x;
                if (i == 1) return y;
                if (i == 2) return z;
                throw std::out_of_range("Vector3 index out of range");
			}

            // --- 工具方法 ---
            float magnitude() const {
                return std::sqrt(x * x + y * y + z * z);
            }

            float sqrMagnitude() const {
                return x * x + y * y + z * z;
            }

            void normalize() {
                float len = magnitude();
                if (len > 0.0f) {
                    x /= len;
                    y /= len;
                    z /= len;
                }
            }

            Vector3 normalized() const {
                Vector3 v(*this);
                v.normalize();
                return v;
            }

            static Vector3 lerp(const Vector3& a, const Vector3& b, float t) {
                return a + (b - a) * t;
            }

            float distance(const Vector3& other) const {
                return (*this - other).magnitude();
            }

            float dot(const Vector3& other) const {
                return x * other.x + y * other.y + z * other.z;
            }

            // 叉积（仅 3D 有）
            Vector3 cross(const Vector3& other) const {
                return {
                    y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x
                };
            }

            // 与另一个向量的夹角（弧度）
            float angle(const Vector3& other) const {
                float len = magnitude() * other.magnitude();
                if (len <= 0.0f) return 0.0f;
                float cosTheta = dot(other) / len;
                cosTheta = std::clamp(cosTheta, -1.0f, 1.0f);
                return std::acos(cosTheta);
            }

            // 零向量常量（可选）
            static const Vector3 zero;
            static const Vector3 one;
            static const Vector3 up;
            static const Vector3 forward;
        };
    } // namespace Utils
} // namespace MangoEngine