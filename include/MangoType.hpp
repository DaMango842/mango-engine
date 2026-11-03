// include/MangoType.hpp
#pragma once

// 这个头文件作为全局类型定义的集中の地，包含了引擎中常用的基础类型和工具类
// 但请勿使用 `using namespace MangoEngine::xxx` 在任意文件中，以免污染全局命名空间
// 除非你想体验因为其他库定义了同名且相同功能造成的冲突 :)

#include <Utils/Vector2.hpp>
#include <Utils/Vector3.hpp>
#include <Utils/Vector4.hpp>
#include <Utils/Matrix2x2.hpp>
#include <Utils/Matrix3x3.hpp>
#include <Utils/Matrix4x4.hpp>


using MangoVector2 = mango_engine::Utils::Vector2;
using MangoVector3 = mango_engine::Utils::Vector3;
using MangoVector4 = mango_engine::Utils::Vector4;
using MangoMatrix2x2 = mango_engine::Utils::Matrix2x2;
using MangoMatrix3x3 = mango_engine::Utils::Matrix3x3;
using MangoMatrix4x4 = mango_engine::Utils::Matrix4x4;