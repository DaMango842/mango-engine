// include/Utils/Matrix3x3.hpp
#pragma once
#include <cmath>
#include "Utils/Vector3.hpp" // 依赖 Vector3

namespace mango_engine {
    namespace Utils {

        struct Matrix3x3 {
            // 列主序存储（Column-major）：m[0], m[1], m[2] 是三列
            // 即矩阵为：
            // [ m[0].x  m[1].x  m[2].x ]
            // [ m[0].y  m[1].y  m[2].y ]
            // [ m[0].z  m[1].z  m[2].z ]
            Vector3 m[3];

            // --- 默认构造：单位矩阵 ---
            Matrix3x3() {
                m[0] = { 1.0f, 0.0f, 0.0f };
                m[1] = { 0.0f, 1.0f, 0.0f };
                m[2] = { 0.0f, 0.0f, 1.0f };
            }

            // --- 自定义构造：传入三列 ---
            Matrix3x3(const Vector3& col0, const Vector3& col1, const Vector3& col2)
                : m{ col0, col1, col2 } {
            }

            // --- 静态工厂方法 ---

            static Matrix3x3 identity() {
                return Matrix3x3();
            }

            // 绕 X 轴旋转（angle in radians）
            static Matrix3x3 rotationX(float angle) {
                float c = std::cos(angle);
                float s = std::sin(angle);
                return Matrix3x3(
                    { 1, 0, 0 },
                    { 0, c, s },
                    { 0, -s, c }
                );
            }

            // 绕 Y 轴旋转
            static Matrix3x3 rotationY(float angle) {
                float c = std::cos(angle);
                float s = std::sin(angle);
                return Matrix3x3(
                    { c, 0, -s },
                    { 0, 1, 0 },
                    { s, 0, c }
                );
            }

            // 绕 Z 轴旋转
            static Matrix3x3 rotationZ(float angle) {
                float c = std::cos(angle);
                float s = std::sin(angle);
                return Matrix3x3(
                    { c, s, 0 },
                    { -s, c, 0 },
                    { 0, 0, 1 }
                );
            }

            // 绕任意轴旋转（Rodrigues' formula）
            static Matrix3x3 rotationAxis(const Vector3& axis, float angle) {
                Vector3 a = axis.normalized();
                float x = a.x, y = a.y, z = a.z;
                float c = std::cos(angle);
                float s = std::sin(angle);
                float omc = 1.0f - c;

                return Matrix3x3(
                    { c + x * x * omc,     x * y * omc + z * s,   x * z * omc - y * s },
                    { x * y * omc - z * s,   c + y * y * omc,     y * z * omc + x * s },
                    { x * z * omc + y * s,   y * z * omc - x * s,   c + z * z * omc }
                );
            }

            // 缩放矩阵
            static Matrix3x3 scale(float sx, float sy, float sz) {
                return Matrix3x3(
                    { sx, 0, 0 },
                    { 0, sy, 0 },
                    { 0, 0, sz }
                );
            }

            static Matrix3x3 scale(const Vector3& s) {
                return scale(s.x, s.y, s.z);
            }

            // --- 运算符重载 ---

            // 矩阵 × 向量
            Vector3 operator*(const Vector3& v) const {
                return m[0] * v.x + m[1] * v.y + m[2] * v.z;
            }

            // 矩阵 × 矩阵
            Matrix3x3 operator*(const Matrix3x3& other) const {
                return Matrix3x3(
                    (*this) * other.m[0],
                    (*this) * other.m[1],
                    (*this) * other.m[2]
                );
            }

            Matrix3x3& operator*=(const Matrix3x3& other) {
                *this = *this * other;
                return *this;
            }

            // --- 方法 ---

            Matrix3x3 transpose() const {
                return Matrix3x3(
                    { m[0].x, m[1].x, m[2].x },
                    { m[0].y, m[1].y, m[2].y },
                    { m[0].z, m[1].z, m[2].z }
                );
            }

            float determinant() const {
                return m[0].x * (m[1].y * m[2].z - m[1].z * m[2].y)
                    - m[0].y * (m[1].x * m[2].z - m[1].z * m[2].x)
                    + m[0].z * (m[1].x * m[2].y - m[1].y * m[2].x);
            }

            Matrix3x3 inverse() const {
                float det = determinant();
                if (std::abs(det) < 1e-6f) {
                    return identity(); // 或抛异常
                }
                float invDet = 1.0f / det;

                Vector3 i0(
                    (m[1].y * m[2].z - m[1].z * m[2].y) * invDet,
                    (m[1].z * m[2].x - m[1].x * m[2].z) * invDet,
                    (m[1].x * m[2].y - m[1].y * m[2].x) * invDet
                );
                Vector3 i1(
                    (m[2].y * m[0].z - m[2].z * m[0].y) * invDet,
                    (m[2].z * m[0].x - m[2].x * m[0].z) * invDet,
                    (m[2].x * m[0].y - m[2].y * m[0].x) * invDet
                );
                Vector3 i2(
                    (m[0].y * m[1].z - m[0].z * m[1].y) * invDet,
                    (m[0].z * m[1].x - m[0].x * m[1].z) * invDet,
                    (m[0].x * m[1].y - m[0].y * m[1].x) * invDet
                );

                return Matrix3x3(i0, i1, i2); // 注意：这是逆矩阵的列
            }

            // 快速元素访问（row, col）
            float& at(int row, int col) { return m[col][row]; }
            float at(int row, int col) const { return m[col][row]; }
        };

    } // namespace Utils
} // namespace MangoEngine