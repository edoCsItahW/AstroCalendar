// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file date.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 14:00
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DATE_H
#define DATE_H
#pragma once

#include "units.h"
#include "value.h"
#include <optional>

namespace astro {
    struct DateTime;

    class JulianDay {
    public:
        using unit = Day;

        constexpr JulianDay(double value, TimeScale scale);

        [[nodiscard]] constexpr JulianDay to(TimeScale target) const;

        template<typename U, typename T>
        constexpr JulianDay operator+(const Val<T, U>& duration) const;

        template<typename U, typename T>
        constexpr JulianDay operator-(const Val<T, U>& duration) const;

        constexpr JulianDay operator+(const JulianDay& other) const;

        constexpr JulianDay operator-(const JulianDay& other) const;

        /** @brief 儒略日乘以一个数或儒略日
         *
         * @details 允许标量和儒略日
         *
         * @tparam T 乘数类型
         * @param factor 被乘数
         * @return 乘积（JulianDay）
         */
        template<typename T>
        constexpr JulianDay operator*(const T& factor) const;

        /** @brief 儒略日与时间相乘
         * @tparam U 目标量的量纲
         * @tparam T 目标量的值类型
         * @param factor 被乘数
         * @return 乘积（目标量类型）
         */
        template<typename U, typename T>
        constexpr auto operator*(const Val<U, T>& factor) const;

        /** @brief 儒略日除以一个数或儒略日
         *
         * @details 允许标量和儒略日
         *
         * @tparam T 除数类型
         * @param factor 除数
         * @return 商（JulianDay）
         */
        template<typename T>
        constexpr JulianDay operator/(const T& factor) const;

        /** @brief 儒略日与时间相除
         *
         * @tparam U 目标量的量纲
         * @tparam T 目标量的值类型
         * @param factor 被除数
         * @return 商（目标量类型）
         */
        template<typename U, typename T>
        constexpr auto operator/(const Val<U, T>& factor) const;

        [[nodiscard]] constexpr DateTime toDateTime() const;

        [[nodiscard]] constexpr TimeScale scale() const;

        [[nodiscard]] constexpr double value() const;

    private:
        double value_;
        TimeScale scale_;

        friend struct DateTime;
    };

    template<typename T>
    inline constexpr bool is_julian_day_v = std::is_same_v<T, JulianDay>;

    template<TimeScale S>
    constexpr JulianDay convert(const JulianDay& jd);

    constexpr JulianDay convert(const JulianDay& jd, TimeScale target);

    struct DateTime {
        int year      = 0;
        int month     = 0;
        int day       = 0;
        int hour      = 0;
        int minute    = 0;
        double second = 0.0;

        TimeScale scale = UTC;

        template<TimeScale S>
        constexpr static DateTime make(const JulianDay& jd);

        constexpr static DateTime make(const JulianDay& jd, std::optional<TimeScale> scale = std::nullopt);

        [[nodiscard]] constexpr JulianDay toJulianDay() const;
    };

    template<typename U, typename T>
    constexpr auto operator*(const Val<U, T>& value, const JulianDay& jd);

    template<typename U, typename T>
    constexpr auto operator/(const Val<U, T>& value, const JulianDay& jd);

    template<typename T>
    requires numerical<T>
    constexpr JulianDay operator*(T factor, const JulianDay& jd);

    template<typename T>
    requires numerical<T>
    constexpr JulianDay operator/(T factor, const JulianDay& jd);


}  // namespace astro

#include "date.hpp"

#endif  // DATE_H
