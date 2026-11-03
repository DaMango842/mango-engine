// include/Utils/Matrix4x4.hpp
#pragma once
#include <cmath>
#include "Utils/Vector4.hpp" // 依赖 Vector4（也隐含 Vector3）

namespace mango_engine {
    namespace Utils {

        struct Matrix4x4 {
            // 列主序存储（Column-major）：m[0] ~ m[3] 是四列
            // 对应 OpenGL / GLM / Vulkan 的标准布局
            // 矩阵形式：
            // [ m[0].x  m[1].x  m[2].x  m[3].x ]
            // [ m[0].y  m[1].y  m[2].y  m[3].y ]
            // [ m[0].z  m[1].z  m[2].z  m[3].z ]
            // [ m[0].w  m[1].w  m[2].w  m[3].w ]
            Vector4 m[4];

            // --- 默认构造：单位矩阵 ---
            Matrix4x4() {
                m[0] = { 1.0f, 0.0f, 0.0f, 0.0f };
                m[1] = { 0.0f, 1.0f, 0.0f, 0.0f };
                m[2] = { 0.0f, 0.0f, 1.0f, 0.0f };
                m[3] = { 0.0f, 0.0f, 0.0f, 1.0f };
            }

            // --- 自定义构造：传入四列 ---
            Matrix4x4(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3)
                : m{ col0, col1, col2, col3 } {
            }

            // --- 静态工厂方法 ---

            static Matrix4x4 identity() {
                return Matrix4x4();
            }

            // 从 Matrix3x3 扩展（用于纯旋转/缩放）
            static Matrix4x4 fromMatrix3x3(const Matrix3x3& mat3) {
                return Matrix4x4(
                    { mat3.m[0].x, mat3.m[0].y, mat3.m[0].z, 0.0f },
                    { mat3.m[1].x, mat3.m[1].y, mat3.m[1].z, 0.0f },
                    { mat3.m[2].x, mat3.m[2].y, mat3.m[2].z, 0.0f },
                    { 0.0f, 0.0f, 0.0f, 1.0f }
                );
            }

            // 平移矩阵
            static Matrix4x4 translation(float x, float y, float z) {
                return Matrix4x4(
                    { 1, 0, 0, 0 },
                    { 0, 1, 0, 0 },
                    { 0, 0, 1, 0 },
                    { x, y, z, 1 }
                );
            }

            static Matrix4x4 translation(const Vector3& t) {
                return translation(t.x, t.y, t.z);
            }

            // 缩放矩阵
            static Matrix4x4 scale(float sx, float sy, float sz) {
                return Matrix4x4(
                    { sx, 0, 0, 0 },
                    { 0, sy, 0, 0 },
                    { 0, 0, sz, 0 },
                    { 0, 0, 0, 1 }
                );
            }

            static Matrix4x4 scale(const Vector3& s) {
                return scale(s.x, s.y, s.z);
            }

            // 绕 X/Y/Z 轴旋转（基于 Matrix3x3 扩展）
            static Matrix4x4 rotationX(float angle) {
                return fromMatrix3x3(Matrix3x3::rotationX(angle));
            }

            static Matrix4x4 rotationY(float angle) {
                return fromMatrix3x3(Matrix3x3::rotationY(angle));
            }

            static Matrix4x4 rotationZ(float angle) {
                return fromMatrix3x3(Matrix3x3::rotationZ(angle));
            }

            // 绕任意轴旋转
            static Matrix4x4 rotationAxis(const Vector3& axis, float angle) {
                return fromMatrix3x3(Matrix3x3::rotationAxis(axis, angle));
            }

            // 透视投影矩阵（OpenGL 右手系，near/far > 0）
            static Matrix4x4 perspective(float fovYRadians, float aspect, float near, float far) {
                float tanHalfFov = std::tan(fovYRadians * 0.5f);
                float range = far - near;

                return Matrix4x4(
                    { 1.0f / (aspect * tanHalfFov), 0, 0, 0 },
                    { 0, 1.0f / tanHalfFov, 0, 0 },
                    { 0, 0, -(far + near) / range, -1 },
                    { 0, 0, -2.0f * far * near / range, 0 }
                );
            }

            // 正交投影矩阵
            static Matrix4x4 ortho(float left, float right, float bottom, float top, float near, float far) {
                return Matrix4x4(
                    { 2.0f / (right - left), 0, 0, 0 },
                    { 0, 2.0f / (top - bottom), 0, 0 },
                    { 0, 0, -2.0f / (far - near), 0 },
                    { -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1 }
                );
            }

            // 视图矩阵（LookAt，右手坐标系）
            static Matrix4x4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
                Vector3 f = (center - eye).normalized();
                Vector3 s = f.cross(up).normalized();
                Vector3 u = s.cross(f);

                return Matrix4x4(
                    { s.x, u.x, -f.x, 0 },
                    { s.y, u.y, -f.y, 0 },
                    { s.z, u.z, -f.z, 0 },
                    { -s.dot(eye), -u.dot(eye), f.dot(eye), 1 }
                );
            }

            // --- 运算符重载 ---

            // 矩阵 × 向量（齐次坐标）
            Vector4 operator*(const Vector4& v) const {
                return m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w;
            }

            // 矩阵 × 矩阵
            Matrix4x4 operator*(const Matrix4x4& other) const {
                return Matrix4x4(
                    (*this) * other.m[0],
                    (*this) * other.m[1],
                    (*this) * other.m[2],
                    (*this) * other.m[3]
                );
            }

            Matrix4x4& operator*=(const Matrix4x4& other) {
                *this = *this * other;
                return *this;
            }

            // --- 方法 ---

            Matrix4x4 transpose() const {
                return Matrix4x4(
                    { m[0].x, m[1].x, m[2].x, m[3].x },
                    { m[0].y, m[1].y, m[2].y, m[3].y },
                    { m[0].z, m[1].z, m[2].z, m[3].z },
                    { m[0].w, m[1].w, m[2].w, m[3].w }
                );
            }

            // 快速元素访问
            float& at(int row, int col) { return m[col][row]; }
            float at(int row, int col) const { return m[col][row]; }

            // 提取平移分量（第4列的 xyz）
            Vector3 getTranslation() const {
                return { m[3].x, m[3].y, m[3].z };
            }

            // 构造仅含平移的矩阵
            static Matrix4x4 fromTranslation(const Vector3& t) {
                Matrix4x4 mat = identity();
                mat.m[3].x = t.x;
                mat.m[3].y = t.y;
                mat.m[3].z = t.z;
                return mat;
            }
        };

    } // namespace Utils
} // namespace mango_engine