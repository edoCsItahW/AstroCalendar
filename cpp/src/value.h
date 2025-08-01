// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file value.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 13:27
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VALUE_H
#define VALUE_H
#pragma once

#include "dimension.h"
#include "utils.h"
#include <cmath>

namespace astro {

    /** @struct Val
     *
     * @brief 基于量纲的数值类型模板类
     *
     * @tparam U 单位类型
     * @tparam T 值类型
     */
    template<typename U, typename T>
    class Val {
    public:
        using value_type = U;
        using unit_type  = T;

        constexpr Val() = default;
        constexpr Val(T v);

        constexpr T value() const;

        template<typename U2>
        constexpr Val<U2, T> to() const;

        /** @brief 与标量相乘
         *
         * @details [类型不同 | 纬度不同 | 单位不同]
         *
         * @tparam S 标量类型
         * @param scalar 标量值
         * @return 乘积结果
         */
        template<typename S>
            requires numerical<S>
        constexpr Val operator*(S scalar) const;

        /** @brief 与标量相除
         *
         * @details [类型不同 | 纬度不同 | 单位不同]
         *
         * @tparam S 标量类型
         * @param scalar 标量值
         * @return 商
         */
        template<typename S>
            requires numerical<S>
        constexpr Val operator/(S scalar) const;

        /** @brief 相同纬度相加
         *
         * @details [类型相同 | 纬度相同 | 单位允许不同]
         *
         * @note 求和操作符允许对不同@b 单位 进行归一化并求和，但@b 纬度 必须相同。
         *
         * @tparam O 另一个单位类型
         * @tparam V 另一个值类型
         * @param other 另一个值
         * @return 求和结果
         */
        template<typename O, typename V>
        constexpr auto operator+(const Val<O, V>& other) const;

        /** @brief 相同纬度相减
         *
         * @details [类型相同 | 纬度相同 | 单位允许不同]
         *
         * @note 求差操作符允许对不同@b 单位 进行归一化并求差，但@b 纬度 必须相同。
         *
         * @tparam O 另一个单位类型
         * @tparam V 另一个值类型
         * @param other 另一个值
         * @return 求差结果
         */
        template<typename O, typename V>
        constexpr auto operator-(const Val<O, V>& other) const;

        /** @brief 相同类型相乘
         *
         * @details [类型相同 | 纬度允许不同 | 单位允许不同]
         *
         * @tparam O 另一个单位类型
         * @tparam V 另一个值类型
         * @param other 另一个值
         * @return 乘积结果
         */
        template<typename O, typename V>
        constexpr auto operator*(const Val<O, V>& other) const;

        /** @brief 相同类型相除
         *
         * @details [类型相同 | 纬度允许不同 | 单位允许不同]
         *
         * @tparam O 另一个单位类型
         * @tparam V 另一个值类型
         * @param other 另一个值
         * @return 商
         */
        template<typename O, typename V>
        constexpr auto operator/(const Val<O, V>& other) const;

        template<typename O>
            requires numerical<O>
        constexpr bool operator<(O other) const;

        template<typename O>
            requires numerical<O>
        constexpr bool operator>(O other) const;

        constexpr Val operator-() const;

    private:
        T value_;
    };

    template<typename T>
    inline constexpr bool is_value_v = false;

    template<typename U, typename T>
    inline constexpr bool is_value_v<Val<U, T>> = true;

    namespace math {
        template<typename T>
            requires numerical<T>
        constexpr T pow(T base, int exponent);

        template<int N, typename U, typename T>
        constexpr auto pow(const Val<U, T>& value);

        template<typename U, typename T>
        constexpr auto sqrt(const Val<U, T>& value);
    }  // namespace math

    template<typename S, typename U, typename T>
        requires numerical<S>
    constexpr auto operator*(S scalar, const Val<U, T>& other);

    template<typename S, typename U, typename T>
        requires numerical<S>
    constexpr auto operator/(S scalar, const Val<U, T>& other);

    template<typename U, typename T>
    requires numerical<T>
    constexpr Val<U, T> value(const T& value);
}  // namespace astro

#include "value.hpp"

#endif  // VALUE_H
