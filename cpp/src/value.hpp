// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file value.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 14:24
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VALUE_HPP
#define VALUE_HPP
#pragma once

#include "dimension.h"
#include <ratio>

namespace astro {  // Val: constructor

    template<typename U, typename T>
    constexpr Val<U, T>::Val(T v)
        : value_(v) {}

    // Val: get original value
    template<typename U, typename T>
    constexpr T Val<U, T>::value() const {
        return value_;
    }

    // Val: unit conversion
    template<typename U, typename T>
    template<typename U2>
    constexpr Val<U2, T> Val<U, T>::to() const {
        using R = std::ratio_divide<typename U::ratio, typename U2::ratio>;

        T value = value_ * static_cast<T>(R::num) / static_cast<T>(R::den);

        return Val<U2, T>(value);
    }

    // Val: operator* with no unit (scalar, dimensionless)
    template<typename U, typename T>
    template<typename S>
        requires numerical<S>
    constexpr Val<U, T> Val<U, T>::operator*(S scalar) const {
        return Val(value_ * scalar);
    }

    // Val: operator/ with no unit (scalar, dimensionless)
    template<typename U, typename T>
    template<typename S>
        requires numerical<S>
    constexpr Val<U, T> Val<U, T>::operator/(S scalar) const {
        return Val(value_ / scalar);
    }

    // Val: operator+ with different unit
    template<typename U, typename T>
    template<typename O, typename V>
    constexpr auto Val<U, T>::operator+(const Val<O, V>& other) const {
        static_assert(std::is_same_v<dimension_power_t<1, typename U::dimension>, dimension_power_t<1, typename O::dimension>>, "Addition of different dimensions is not allowed.");

        auto converted = other.template to<U>();
        return Val<U, std::common_type_t<T, V>>(value_ + converted.value());
    }

    // Val: operator- with different unit
    template<typename U, typename T>
    template<typename O, typename V>
    constexpr auto Val<U, T>::operator-(const Val<O, V>& other) const {
        static_assert(std::is_same_v<dimension_power_t<1, typename U::dimension>, dimension_power_t<1, typename O::dimension>>, "Subtraction of different dimensions is not allowed.");

        auto converted = other.template to<U>();

        return Val<U, std::common_type_t<T, V>>(value_ - converted.value());
    }

    // Val: operator* with different unit
    template<typename U, typename T>
    template<typename O, typename V>
    constexpr auto Val<U, T>::operator*(const Val<O, V>& other) const {
        using D = merge_dimensions_t<typename U::dimension, typename O::dimension>;

        using R = std::ratio_multiply<typename U::ratio, typename O::ratio>;

        return Val<Unit<D, R>, std::common_type_t<T, V>>(value_ * other.value());
    }

    // Val: operator/ with different unit
    template<typename U, typename T>
    template<typename O, typename V>
    constexpr auto Val<U, T>::operator/(const Val<O, V>& other) const {
        using D = merge_dimensions_t<typename U::dimension, dimension_power_t<-1, typename O::dimension>>;

        using R = std::ratio_divide<typename U::ratio, typename O::ratio>;

        return Val<Unit<D, R>, std::common_type_t<T, V>>(value_ / other.value());
    }

    template<typename U, typename T>
    template<typename O>
        requires numerical<O>
    constexpr bool Val<U, T>::operator<(O other) const {
        return value_ < other;
    }

    template<typename U, typename T>
    template<typename O>
        requires numerical<O>
    constexpr bool Val<U, T>::operator>(O other) const {
        return value_ > other;
    }

    // Val: operator- negation
    template<typename U, typename T>
    constexpr Val<U, T> Val<U, T>::operator-() const {
        return Val(-value_);
    }

    namespace math {
        // pow
        template<int N, typename U, typename T>
        constexpr auto pow(const Val<U, T>& value) {
            if constexpr (N == 0) {
                // 零次幂返回无量纲的单位值 1
                return Val<Unit<Dimensionless, std::ratio<1>>, T>(1);
            } else {
                using D = dimension_power_t<N, typename U::dimension>;
                using R = ratio_power_t<typename U::ratio, N>;
                T v     = math::pow(value.value(), N);
                return Val<Unit<D, R>, T>(v);
            }
        }

        // sqrt
        template<typename U, typename T>
        constexpr auto sqrt(const Val<U, T>& value) {
            using D = dimension_power_t<1 / 2, typename U::dimension>;

            using R = ratio_power_t<typename U::ratio, 1 / 2>;

            T v = std::sqrt(value.value());

            return Val<Unit<D, R>, T>(v);
        }

        template<typename U, typename T>
        constexpr Val<U, T> abs(const Val<U, T>& value) {
            return Val<U, T>(std::abs(value.value()));
        }

        template<typename U, typename T>
        constexpr T sin(const Val<U, T>& angle) {
            return std::sin(angle.value());
        }

        template<typename U, typename T>
        constexpr T cos(const Val<U, T>& angle) {
            return std::cos(angle.value());
        }

        template<typename T>
            requires numerical<T>
        constexpr T pow(T base, int exponent) {
            return std::pow(base, exponent);
        }
    }  // namespace math

    template<typename S, typename U, typename T>
        requires numerical<S>
    constexpr auto operator*(S scalar, const Val<U, T>& other) {
        return other * scalar;
    }

    template<typename S, typename U, typename T>
        requires numerical<S>
    constexpr auto operator/(S scalar, const Val<U, T>& other) {
        return other / scalar;
    }

    template<typename U, typename T>
    requires numerical<T>
    constexpr Val<U, T> value(const T& value) {
        return Val<U, T>(value);
    }
}  // namespace astro

#endif  // VALUE_HPP
