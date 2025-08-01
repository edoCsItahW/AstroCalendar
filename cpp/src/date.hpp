// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file date.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 14:00
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DATE_HPP
#define DATE_HPP
#pragma once

#include "date.h"
#include "value.h"

namespace astro {
    // JulianDay: operator+ with Value
    template<typename U, typename T>
    constexpr JulianDay JulianDay::operator+(const Val<T, U>& duration) const {
        auto days = duration.template to<Day>();
        return {value_ + days.value(), scale_};
    }

    // JulianDay: operator- with Value
    template<typename U, typename T>
    constexpr JulianDay JulianDay::operator-(const Val<T, U>& duration) const {
        auto days = duration.template to<Day>();
        return {value_ - days.value(), scale_};
    }

    template<typename T>
    constexpr JulianDay JulianDay::operator*(const T& factor) const {
        if constexpr (is_julian_day_v<T>)
            return {value_ * factor.value(), scale_};
        else
            return {value_ * factor, scale_};
    }

    template<typename U, typename T>
    constexpr auto JulianDay::operator*(const Val<U, T>& factor) const {
        return factor.template to<Day>().value() * Val<Day, T>(value_);
    }

    template<typename T>
    constexpr JulianDay JulianDay::operator/(const T& factor) const {
        if constexpr (is_julian_day_v<T>)
            return {value_ / factor.value(), scale_};
        else
            return {value_ / factor, scale_};
    }

    template<typename U, typename T>
    constexpr auto JulianDay::operator/(const Val<U, T>& factor) const {
        return factor.template to<Day>().value() / Val<Day, T>(value_);
    }

    // DateTime: make with JulianDay
    template<TimeScale S>
    constexpr DateTime DateTime::make(const JulianDay& jd) {
        return make(jd, S);
    }

    template<typename U, typename T>
    constexpr auto operator*(const Val<U, T>& value, const JulianDay& jd) {
        return jd * value;
    }

    template<typename U, typename T>
    constexpr auto operator/(const Val<U, T>& value, const JulianDay& jd) {
        return jd / value;
    }

}  // namespace astro

#endif  // DATE_HPP
