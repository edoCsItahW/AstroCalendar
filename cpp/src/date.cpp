// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file date.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 14:00
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "date.h"

namespace astro {
    extern constexpr JulianDay convert(const JulianDay& jd, TimeScale target);

    // JulianDay: JulianDay constructor
    constexpr JulianDay::JulianDay(const double value, const TimeScale scale)
        : value_(value)
        , scale_(scale) {}

    // JulianDay: time scale conversion
    constexpr JulianDay JulianDay::to(const TimeScale target) const {
        if (scale_ == target) return *this;
        return convert(*this, target);
    }

    // JulianDay: operator+ with JulianDay
    constexpr JulianDay JulianDay::operator+(const JulianDay& other) const { return {value_ + other.value_, scale_}; }

    // JulianDay: operator- with JulianDay
    constexpr JulianDay JulianDay::operator-(const JulianDay& other) const { return {value_ - other.value_, scale_}; }

    // JulianDay: toDateTime
    constexpr DateTime JulianDay::toDateTime() const { return DateTime::make(to(TT), scale_); }

    // JulianDay: get original scale
    constexpr TimeScale JulianDay::scale() const { return scale_; }

    // JulianDay: get value
    constexpr double JulianDay::value() const { return value_; }

    // DateTime: DateTime make with JulianDay and optional TimeScale
    constexpr DateTime DateTime::make(const JulianDay& jd, std::optional<TimeScale> scale) {
        scale = scale.value_or(jd.scale_);  // 无需加0.5

        int Z    = std::floor(jd.value_);
        double F = jd.value_ - Z;

        int A = Z < 2299161 ? Z : static_cast<int>((Z - 1867216.25) / 36524.25);
        int B = Z + 1 + A - A / 4;

        int C = B + 1524;
        int D = std::floor((C - 122.1) / 365.25);
        int E = std::floor(365.25 * D);
        int G = std::floor((C - E) / 30.6001);

        int day   = C - E + F - std::floor(30.6001 * G);
        int month = G - (G < 13.5 ? 1 : 13);
        int year  = D - (month > 2.5 ? 4716 : 4715);

        double h      = F * 24.0;
        int hour      = static_cast<int>(h);
        double m      = (h - hour) * 60.0;
        int minute    = static_cast<int>(m);
        double second = (m - minute) * 60.0;

        return {year, month, day, hour, minute, second, scale.value()};
    }

    // DateTime: toJulianDay
    constexpr JulianDay DateTime::toJulianDay() const {
        // 处理公元前年份情况
        int y = year < 0 ? year + 1 : year;
        int m = month;
        int d = day;

        if (m <= 2) {
            y--;
            m += 12;
        }

        // 格里高利历修正（1582年10月5日至1582年10月14日）
        int A = y / 100;
        int B = y < 1582 || (y == 1582 && m < 10) || (y == 1582 && m == 10 && d < 15) ? 0 : 2 - A + A / 4;

        // 计算儒略日数
        double jd = std::floor(365.25 * (y + 4716)) + std::floor(30.6001 * (m + 1)) + d + B - 1524.5;

        return {jd + (hour + minute / 60.0 + second / 3600.0) / 24.0, scale};
    }
}  // namespace astro
