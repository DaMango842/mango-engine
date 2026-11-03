// include/Utils/Matrix2x2.hpp
#pragma once
#include <cmath>
#include "Utils/Vector2.hpp" // 确保 Vector2 已定义

namespace mango_engine {
    namespace Utils {

        struct Matrix2x2 {
            // 列主序存储（Column-major）：m[0] = col0, m[1] = col1
            // 即：
            // [ m[0].x  m[1].x ]
            // [ m[0].y  m[1].y ]
            Vector2 m[2]; // 两列，每列是一个 Vector2

            // --- 默认构造：单位矩阵 ---
            Matrix2x2() {
                m[0] = { 1.0f, 0.0f }; // 第一列
                m[1] = { 0.0f, 1.0f }; // 第二列
            }

            // --- 自定义构造 ---
            Matrix2x2(const Vector2& col0, const Vector2& col1) : m{ col0, col1 } {}

            // --- 静态工厂方法 ---
            static Matrix2x2 identity() {
                return Matrix2x2();
            }

            // 创建旋转矩阵（angle in radians）
            static Matrix2x2 rotation(float angle) {
                float c = std::cos(angle);
                float s = std::sin(angle);
                return Matrix2x2(
                    { c, s },   // 第一列
                    { -s, c }   // 第二列
                );
            }

            // 创建缩放矩阵
            static Matrix2x2 scale(float sx, float sy) {
                return Matrix2x2(
                    { sx, 0.0f },
                    { 0.0f, sy }
                );
            }

            static Matrix2x2 scale(const Vector2& s) {
                return scale(s.x, s.y);
            }

            // --- 运算符重载 ---
            Vector2 operator*(const Vector2& v) const {
                // result = m[0] * v.x + m[1] * v.y
                return m[0] * v.x + m[1] * v.y;
            }

            Matrix2x2 operator*(const Matrix2x2& other) const {
                // 手动展开 2x2 矩阵乘法
                return Matrix2x2(
                    (*this) * other.m[0], // 第一列 = this * other.col0
                    (*this) * other.m[1]  // 第二列 = this * other.col1
                );
            }

            Matrix2x2& operator*=(const Matrix2x2& other) {
                *this = *this * other;
                return *this;
            }

            // --- 方法 ---
            Matrix2x2 transpose() const {
                return Matrix2x2(
                    { m[0].x, m[1].x },
                    { m[0].y, m[1].y }
                );
            }

            float determinant() const {
                return m[0].x * m[1].y - m[0].y * m[1].x;
            }

            // 求逆（仅当行列式 ≠ 0）
            Matrix2x2 inverse() const {
                float det = determinant();
                if (std::abs(det) < 1e-6f) {
                    return identity(); // 或抛异常，这里保守返回单位阵
                }
                float invDet = 1.0f / det;
                return Matrix2x2(
                    { m[1].y * invDet, -m[0].y * invDet },
                    { -m[1].x * invDet,  m[0].x * invDet }
                );
            }

            // 快速访问元素（可选）
            float& at(int row, int col) { return m[col][row]; } // 注意：col 是外层索引
            float at(int row, int col) const { return m[col][row]; }
        };

    } // namespace Utils
} // namespace MangoEngine


