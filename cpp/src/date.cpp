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

    extern const JulianDay J2000;

    namespace cvt {
        using DegreeParCentury =
            Unit<merge_dimensions_t<DimensionList<Dimension<AngleTag, 1>>, dimension_power_t<-1, DimensionList<Dimension<TimeTag, 1>>>>, std::ratio_divide<Degree::ratio, Century::ratio>>;

        constexpr Val<Second, double> leapSecond(const JulianDay& jd) {
            auto dt = jd.toDateTime();

            if (dt.year < -500) {
                // u = (y - 1820) / 100
                auto u = (dt.year - 1820) / 100.0;

                // ΔT = -20 + 32 * u^2
                return -20 + 32 * std::pow(u, 2);
            }

            if (dt.year < 500) {
                // u = y / 100
                auto u = dt.year / 100.0;

                // ΔT = 10583.6 - 1014.41 * u + 33.78311 * u^2 - 5.952053 * u^3 -
                // 0.1798452 * u ^ 4 + 0.022174192 * u ^ 5 + 0.0090316521 * u ^ 6
                return 10583.6 - 1014.41 * u + 33.78311 * std::pow(u, 2) - 5.952053 * std::pow(u, 3) - 0.1798452 * std::pow(u, 4) + 0.022174192 * std::pow(u, 5) + 0.0090316521 * std::pow(u, 6);
            }

            if (dt.year < 1600) {
                // u = (y - 1000) / 100
                auto u = (dt.year - 1000) / 100.0;

                // ΔT = 1574.2 - 556.01 * u + 71.23472 * u^2 + 0.319781 * u^3 -
                // 0.8503463 * u ^ 4 - 0.005050998 * u ^ 5 + 0.0083572073 * u ^ 6
                return 1574.2 - 556.01 * u + 71.23472 * std::pow(u, 2) + 0.319781 * std::pow(u, 3) - 0.8503463 * std::pow(u, 4) - 0.005050998 * std::pow(u, 5) + 0.0083572073 * std::pow(u, 6);
            }

            if (dt.year < 1700) {
                // u = y - 1600
                auto u = dt.year - 1600;

                // ΔT = 120 - 0.9808 * u - 0.01532 * u^2 + u^3 / 7129
                return 120 - 0.9808 * u - 0.01532 * std::pow(u, 2) + std::pow(u, 3) / 7129.0;
            }

            if (dt.year < 1800) {
                // u = y - 1700
                auto u = dt.year - 1700;

                // ΔT = 8.83 + 0.1603 * u - 0.0059285 * u^2 + 0.00013336 * u^3 - u^4 /
                // 1174000
                return 8.83 + 0.1603 * u - 0.0059285 * std::pow(u, 2) + 0.00013336 * std::pow(u, 3) - std::pow(u, 4) / 1174000.0;
            }

            if (dt.year < 1860) {
                // u = y - 1800
                auto u = dt.year - 1800;

                // ΔT = 13.72 - 0.332447 * u + 0.0068612 * u^2 + 0.0041116 * u^3 -
                // 0.00037436 * u^4 + 0.0000121272 * u^5 - 0.0000001699 * u^6 +
                // 0.000000000875 * u^7
                return 13.72 - 0.332447 * u + 0.0068612 * std::pow(u, 2) + 0.0041116 * std::pow(u, 3) - 0.00037436 * std::pow(u, 4) + 0.0000121272 * std::pow(u, 5) - 0.0000001699 * std::pow(u, 6)
                     + 0.000000000875 * std::pow(u, 7);
            }

            if (dt.year < 1900) {
                // u = y - 1860
                auto u = dt.year - 1860;

                // ΔT = 7.62 + 0.5737 * t - 0.251754 * t^2 + 0.01680668 * t^3
                // -0.0004473624 * t ^ 4 + t ^ 5 / 233174
                return 7.62 + 0.5737 * u - 0.251754 * std::pow(u, 2) + 0.01680668 * std::pow(u, 3) - 0.0004473624 * std::pow(u, 4) + std::pow(u, 5) / 233174.0;
            }

            if (dt.year < 1920) {
                // u = y - 1900
                auto u = dt.year - 1900;

                // ΔT = -2.79 + 1.494119 * u - 0.0598939 * u^2 + 0.0061966 * u^3 -
                // 0.000197 * u^4
                return -2.79 + 1.494119 * u - 0.0598939 * std::pow(u, 2) + 0.0061966 * std::pow(u, 3) - 0.000197 * std::pow(u, 4);
            }

            if (dt.year < 1941) {
                // u = y - 1920
                auto u = dt.year - 1920;

                // ΔT = 21.20 + 0.84493 * u - 0.076100 * u^2 + 0.0020936 * u^3
                return 21.20 + 0.84493 * u - 0.076100 * std::pow(u, 2) + 0.0020936 * std::pow(u, 3);
            }

            if (dt.year < 1961) {
                // u = y - 1950
                auto u = dt.year - 1950;

                // ΔT = 29.07 + 0.407 * u - u^2 / 233 + u^3 / 2547
                return 29.07 + 0.407 * u - std::pow(u, 2) / 233.0 + std::pow(u, 3) / 2547.0;
            }

            if (dt.year < 1986) {
                // u = y - 1975
                auto u = dt.year - 1975;

                // ΔT = 45.45 + 1.067 * u - u^2 / 260 - u^3 / 718
                return 45.45 + 1.067 * u - std::pow(u, 2) / 260.0 - std::pow(u, 3) / 718.0;
            }

            if (dt.year < 2005) {
                // u = y - 2000
                auto u = dt.year - 2000;

                // ΔT = 63.86 + 0.3345 * u - 0.060374 * u^2 + 0.0017275 * u^3 +
                // 0.000651814 * u^4 + 0.00002373599 * u^5
                return 63.86 + 0.3345 * u - 0.060374 * std::pow(u, 2) + 0.0017275 * std::pow(u, 3) + 0.000651814 * std::pow(u, 4) + 0.00002373599 * std::pow(u, 5);
            }

            if (dt.year < 2050) {
                // u = y - 2000
                auto u = dt.year - 2000;

                // ΔT = 62.92 + 0.32217 * u + 0.005589 * u^2
                return 62.92 + 0.32217 * u + 0.005589 * std::pow(u, 2);
            }

            if (dt.year < 2150) {
                // u = (y - 2000) / 100
                auto u = (dt.year - 1820) / 100.0;

                // ΔT = -20 + 32 * ((y-1820)/100)^2 - 0.5628 * (2150 - y)
                return -20 + 32 * std::pow(u, 2) - 0.5628 * (2150 - dt.year);
            }

            // u = (y - 1820) / 100
            auto u = (dt.year - 1820) / 100.0;

            // ΔT = -20 + 32 * u^2
            return -20.0 + 32 * std::pow(u, 2);
        }

        constexpr auto calcG(const JulianDay& jd) {
            auto century = (jd - J2000).value() / 36525.0;

            return std::numbers::pi * (357.528 + 35'999.05 * century) / 180.0;
        }

        constexpr auto diffG(const JulianDay& jd) {
            auto g = calcG(jd);

            auto gDiff = std::numbers::pi * 35999.05 / 180.0;

            return gDiff * 0.001658 * std::cos(g + 0.0167 * std::sin(g)) * (1 + 0.0167 * std::cos(g));
        }

        constexpr auto delta(const JulianDay& jd) {
            auto g = calcG(jd);

            return 0.001658_tS * std::sin(g + 0.0167 * std::sin(g));
        }

        constexpr JulianDay TDB2TT(const JulianDay& jd) {
            // 假设jd_0 = jd_tdb
            auto prev = jd;
            auto diff = delta(prev);

            while (math::abs(diff) < 1e-12) {
                prev = prev - diff;
                diff = delta(prev);
            }

            return prev;
        }

        constexpr JulianDay TT2TDB(const JulianDay& jd) { return jd + delta(jd); }

        constexpr JulianDay TAI2TT(const JulianDay& jd) { return jd + 32.184_tS; }

        constexpr JulianDay TT2TAI(const JulianDay& jd) { return jd - 32.184_tS; }

        constexpr JulianDay UTC2TAI(const JulianDay& jd) { return jd + leapSecond(jd); }

        constexpr JulianDay TAI2UTC(const JulianDay& jd) { return jd - leapSecond(jd); }

        constexpr JulianDay UTC2TT(const JulianDay& jd) { return TAI2TT(UTC2TAI(jd)); }

        constexpr JulianDay TAI2TDB(const JulianDay& jd) { return TT2TDB(TAI2TT(jd)); }

        constexpr JulianDay UTC2TDB(const JulianDay& jd) { return TT2TDB(TAI2TT(UTC2TAI(jd))); }

        constexpr JulianDay TDB2TAI(const JulianDay& jd) { return TT2TAI(TDB2TT(jd)); }

        constexpr JulianDay TT2UTC(const JulianDay& jd) { return TAI2UTC(TT2TAI(jd)); }

        constexpr JulianDay TDB2UTC(const JulianDay& jd) { return TAI2UTC(TT2TAI(TDB2TT(jd))); }

        constexpr JulianDay toTT(const JulianDay& jd) {
            switch (jd.scale()) {
                case TT: return jd;
                case TDB: return TDB2TT(jd);
                case TAI: return TAI2TT(jd);
                case UTC: return UTC2TT(jd);
                default: return jd;
            }
        }

        constexpr JulianDay toTDB(const JulianDay& jd) {
            switch (jd.scale()) {
                case TDB: return jd;
                case TT: return TT2TDB(jd);
                case TAI: return TAI2TDB(jd);
                case UTC: return UTC2TDB(jd);
                default: return jd;
            }
        }

        constexpr JulianDay toTAI(const JulianDay& jd) {
            switch (jd.scale()) {
                case TAI: return jd;
                case TT: return TT2TAI(jd);
                case TDB: return TDB2TAI(jd);
                case UTC: return UTC2TAI(jd);
                default: return jd;
            }
        }

        constexpr JulianDay toUTC(const JulianDay& jd) {
            switch (jd.scale()) {
                case UTC: return jd;
                case TT: return TT2UTC(jd);
                case TDB: return TDB2UTC(jd);
                case TAI: return TAI2UTC(jd);
                default: return jd;
            }
        }
    }  // namespace cvt

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
