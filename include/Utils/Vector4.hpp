// include/Utils/Vector4.hpp
#pragma once
#include <cmath>
#include <algorithm> // for std::clamp
#include <stdexcept>

#include "Utils/Vector3.hpp" // 确保 Vector3 已定义

namespace mango_engine {
    namespace Utils {
        struct Vector4 {
            float x = 0.0f;
            float y = 0.0f;
            float z = 0.0f;
            float w = 0.0f;

            // --- 构造函数 ---
            Vector4() = default;
            Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
            Vector4(const Vector3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {} // 从 Vector3 扩展
            Vector4(float value) : x(value), y(value), z(value), w(value) {}   // 标量构造（如 white = Vector4(1)）

            // --- 运算符重载 ---
            Vector4 operator+(const Vector4& v) const { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
            Vector4 operator-(const Vector4& v) const { return { x - v.x, y - v.y, z - v.z, w - v.w }; }
            Vector4 operator*(float s) const { return { x * s, y * s, z * s, w * s }; }
            Vector4 operator/(float s) const { return { x / s, y / s, z / s, w / s }; }

            Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
            Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
            Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
            Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }

            bool operator==(const Vector4& v) const {
                constexpr float eps = 1e-5f;
                return std::abs(x - v.x) < eps &&
                    std::abs(y - v.y) < eps &&
                    std::abs(z - v.z) < eps &&
                    std::abs(w - v.w) < eps;
            }

			// --- 下标访问 ---
            float& operator[](size_t i) {
                if (i == 0) return x;
                if (i == 1) return y;
                if (i == 2) return z;
                if (i == 3) return w;
                throw std::out_of_range("Vector4 index out of range");
			}

            const float& operator[](size_t i) const {
                if (i == 0) return x;
                if (i == 1) return y;
                if (i == 2) return z;
                if (i == 3) return w;
                throw std::out_of_range("Vector4 index out of range");
            }

            // --- 工具方法 ---
            float magnitude() const {
                return std::sqrt(x * x + y * y + z * z + w * w);
            }

            float sqrMagnitude() const {
                return x * x + y * y + z * z + w * w;
            }

            void normalize() {
                float len = magnitude();
                if (len > 0.0f) {
                    x /= len;
                    y /= len;
                    z /= len;
                    w /= len;
                }
            }

            Vector4 normalized() const {
                Vector4 v(*this);
                v.normalize();
                return v;
            }

            static Vector4 lerp(const Vector4& a, const Vector4& b, float t) {
                return a + (b - a) * t;
            }

            float distance(const Vector4& other) const {
                return (*this - other).magnitude();
            }

            float dot(const Vector4& other) const {
                return x * other.x + y * other.y + z * other.z + w * other.w;
            }

            // --- 颜色/分量便捷访问（可选但实用）---
            // 例如：color.r = 1.0f; 或 pos.xyz() 获取 Vector3
            float& r() { return x; } const float& r() const { return x; }
            float& g() { return y; } const float& g() const { return y; }
            float& b() { return z; } const float& b() const { return z; }
            float& a() { return w; } const float& a() const { return w; }

            // 转为 Vector3（丢弃 w）
            Vector3 xyz() const { return { x, y, z }; }
        };
    } // namespace Utils
} // namespace mango_engine