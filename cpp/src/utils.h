// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for
// commercial purposes is prohibited without the author's permission. If you have any
// questions or require permission, please contact the author: 2207150234@st.sziit.edu.cn

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

#include <array>
#include <cstddef>
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

template<typename T, typename U = double>
concept numerical = std::is_arithmetic_v<T> || std::is_convertible_v<T, U>;

template<char... Cs>
constexpr auto parseCharSeq();

template<double V, double F, bool I, char... Cs>
struct str_type_impl;

template<double V, double F, bool I>
struct str_type_impl<V, F, I> {
    using type = std::conditional_t<I, double, int>;

    static constexpr type value = I ? V : static_cast<int>(V);
};

template<double V, double F, bool I, char C, char... Cs>
struct str_type_impl<V, F, I, C, Cs...> {
private:
    static constexpr bool is_digit = C >= '0' && C <= '9';
    static constexpr bool is_dot   = C == '.';

    static constexpr double factor  = is_digit && I ? F * 0.1 : F;
    static constexpr double current = is_dot ? V : is_digit ? (I ? V + (C - '0') * factor : V * 10 + (C - '0')) : V;

    static constexpr bool is_float = I || is_dot;

    using next = str_type_impl<current, factor, is_float, Cs...>;

public:
    using type = typename next::type;

    static constexpr auto value = next::value;
};

template<char... Cs>
struct str_type : str_type_impl<0.0, 1.0, false, Cs...> {};

template<char... Cs>
using str_type_t = typename str_type<Cs...>::type;

template<char... Cs>
inline constexpr auto str_type_v = str_type<Cs...>::value;

template<char... Cs>
using char_seq_t = decltype(parseCharSeq<Cs...>());

template<typename T, typename... Ts>
struct contains_type : std::disjunction<std::is_same<T, Ts>...> {};

template<typename T, typename... Ts>
constexpr bool contains_type_v = contains_type<T, Ts...>::value;

template<auto E>
    requires std::is_enum_v<std::remove_cvref_t<decltype(E)>>
constexpr auto enum_to_int_v = static_cast<std::underlying_type_t<decltype(E)>>(E);

template<typename... Arrays>
constexpr auto concatenate_arrays(Arrays&&... arrays) {
    // 计算总大小
    constexpr std::size_t total_size = (0 + ... + std::tuple_size_v<std::decay_t<Arrays>>);
    using value_type                 = std::common_type_t<typename std::decay_t<Arrays>::value_type...>;
    std::array<value_type, total_size> result{};

    std::size_t index = 0;
    // 使用 lambda + 折叠表达式复制数据
    auto copy = [&](const auto& arr) {
        for (const auto& element : arr) result[index++] = element;
    };
    (copy(arrays), ...);  // 依次处理每个数组

    return result;
}

template<numerical T, T X, T Y>
struct Max_impl {
    static constexpr T value = X > Y ? X : Y;
};

template<auto X, auto Y>
struct Max {
    static constexpr auto value = Max_impl<decltype(X), X, Y>::value;
};

template<auto X, auto Y>
inline constexpr auto max_v = Max<X, Y>::value;

#include "utils.hpp"

#endif  // UTILS_H
