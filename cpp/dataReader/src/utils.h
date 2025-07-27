// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/05/29 17:08
 *
 * @if zh
 * @brief 定义一些工具函数或类
 *
 * @elseif en
 * @brief Defines some utility functions or classes.
 *
 * @endif
 *
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <cassert>
#include <string>
#include <type_traits>

/**
 * @if zh
 *
 * @brief 将枚举值转换为字符串表示（编译期）
 *
 * @tparam V 自动推导的枚举值
 * @return 枚举值的字符串视图
 *
 * @par 实现细节:
 * - 使用编译器特定宏获取类型信息字符串
 * - GCC/Clang: 解析__PRETTY_FUNCTION__
 * - MSVC: 解析__FUNCSIG__
 * - 移除命名空间前缀和模板杂讯
 *
 * @note 要求枚举值在编译期已知
 *
 * **示例**
 *
 *
 * @elseif en
 * @brief Converts an enum value to its string representation (compile-time)
 *
 * @tparam V Auto-deduced enum value
 * @return String view of the enum value
 *
 * @par Implementation details:
 * - Uses compiler-specific macros to get type info string
 * - GCC/Clang: Parses __PRETTY_FUNCTION__
 * - MSVC: Parses __FUNCSIG__
 * - Removes namespace prefixes and template noise
 *
 * @note Requires the enum value to be known at compile-time
 *
 * **Example**
 *
 *
 * @endif
 *
 * @code {.cpp}
 * enum class Color { Red = 0, Green, Blue };
 * constexpr auto str = enumToStr<Color::Blue>();
 * @endcode
 */
template<auto V>
constexpr auto enumToStr();

/**
 * @if zh
 *
 * @brief 递归计算枚举类型的最大值
 *
 * @tparam T 枚举类型
 * @tparam N 当前测试的枚举整数值
 * @return 枚举有效值的最大索引
 *
 * @par 终止条件:
 * - 当enumToStr返回的字符串包含")"时停止递归
 * - 表示超出有效枚举范围
 *
 * @warning 递归深度受枚举最大值限制
 *
 *
 * @elseif en
 *
 * @brief Recursively calculates the maximum value of an enum type
 *
 * @tparam T Enum type
 * @tparam N Current integer value being tested
 * @return Maximum valid index of the enum
 *
 * @par Termination condition:
 * - Stops recursion when enumToStr returns a string containing ")"
 * - Indicates exceeding valid enum range
 *
 * @warning Recursion depth limited by max enum value
 *
 *
 * @endif
 */
template<typename T, std::size_t N = 0>
constexpr auto enumMax();

/**
 * @if zh
 *
 * @brief 运行时枚举值转字符串
 *
 * @tparam T 枚举类型（通过concept约束）
 * @param value 运行时枚举值
 * @return 对应的字符串视图
 *
 * @par 工作原理:
 * 1. 使用enumMax确定枚举范围
 * 2. 预生成所有枚举值的字符串数组
 * 3. 通过整数值索引返回对应字符串
 *
 * @note 要求枚举值从0开始连续
 *
 * **示例**
 *
 *
 * @elseif en
 *
 * @brief Converts runtime enum value to string
 *
 * @tparam T Enum type (constrained by concept)
 * @param value Runtime enum value
 * @return Corresponding string view
 *
 * @par How it works:
 * 1. Uses enumMax to determine enum range
 * 2. Pre-generates string array for all enum values
 * 3. Returns string by integer value indexing
 *
 * @note Requires enum values to be contiguous starting from 0
 *
 * **Example**
 *
 * @endif
 *
 *
 * @code {.cpp}
 * Color c = Color::Red;
 * std::cout << enumToStr(c); // 输出"Red"
 * @endcode
 */
template<typename T>
    requires std::is_enum_v<T>
constexpr auto enumToStr(T value);

/**
 * @if zh
 *
 * @brief 字符是否为数字
 *
 * @param c 字符
 * @return 字符是否为数字
 *
 *
 * @elseif en
 *
 * @brief Check if a character is a digit
 *
 * @param c Character
 * @return Whether the character is a digit
 *
 *
 * @endif
 */
bool isDigit(char c);

/**
 * @if zh
 *
 * @brief 字符是否为字母
 *
 * @param c 字符
 * @return 字符是否为字母
 *
 *
 * @elseif en
 *
 * @brief Check if a character is a letter
 *
 * @param c Character
 * @return Whether the character is a letter
 *
 *
 * @endif
 */
bool isLetter(char c);

/**
 * @if zh
 *
 * @brief 字符是否为16进制数字
 *
 * @param c 字符
 * @return 字符是否为16进制数字
 *
 *
 * @elseif en
 *
 * @brief Check if a character is a hexadecimal digit
 *
 * @param c Character
 * @return Whether the character is a hexadecimal digit
 *
 *
 * @endif
 */
bool isHexDigit(char c);

/**
 * @if zh
 *
 * @brief 字符是否为空白字符
 * @details 空白字符包括空格、制表符、换行符、回车符
 *
 * @param c 字符
 * @return 字符是否为空白字符
 *
 *
 * @elseif en
 *
 * @brief Check if a character is a whitespace character
 * @details Whitespace characters include spaces, tabs, newlines, and carriage returns.
 *
 * @param c Character
 * @return Whether the character is a whitespace character
 *
 *
 * @endif
 */
bool isWhitespace(char c);

/**
 * @if zh
 *
 * @brief 字符是否为符合标识符的字符
 * @details 符合标识符的字符必须是字母、数字、下划线
 *
 * @param c 字符
 * @return 字符是否符合标识符的字符
 *
 *
 * @elseif en
 *
 * @brief Check if a character is a valid identifier character
 * @details A valid identifier character must be a letter, a digit, or an underscore.
 *
 * @param c Character
 * @return Whether the character is a valid identifier character
 *
 *
 * @endif
 */
bool isIdentifier(char c);

/**
 * @if zh
 *
 * @brief 将字符串转换为小写
 * @details 该函数会将字符串中的所有大写字母转换为小写字母
 *
 * @param str 字符串
 * @return 转换后的字符串
 *
 *
 * @elseif en
 *
 * @brief Convert a string to lowercase
 * @details This function converts all uppercase letters in the string to lowercase letters.
 *
 * @param str String
 * @return Converted string
 *
 *
 * @endif
 */
std::string toLowerCase(const std::string& str);

bool isUpperCase(const std::string& str);

bool isLowerCase(const std::string& str);

bool isCapitalized(const std::string& str);

#include "utils.hpp"

#endif  // UTILS_H
