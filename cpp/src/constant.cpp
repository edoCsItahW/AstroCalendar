// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file constant.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 11:48
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "constant.h"
#include <cmath>
#include <numbers>
#include <sstream>

namespace astro {
    constexpr double LIGHT_SPEED = 2.99792458e8;

    constexpr double AU = 149597870700.0;

    JulianDay DateTime::toJulianDay() const {
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

    DateTime JulianDay::toDateTime() const {
        int Z    = std::floor(value);
        double F = value - Z;

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

        return {year, month, day, hour, minute, second, scale};
    }

    JulianDay JulianDay::to(TimeScale scale) const { return {convert(value, this->scale, scale), scale}; }

    namespace cvt {
        double leapSecond(const double t) {
            auto t_year = JulianDay(t, UTC).toDateTime().year;  // 只需要年份，所以哪个时间尺度都可以

            if (t_year < -500) {
                // u = (y - 1820) / 100
                auto u = (t_year - 1820) / 100.0;

                // ΔT = -20 + 32 * u^2
                return -20 + 32 * std::pow(u, 2);
            }

            if (t_year < 500) {
                // u = y / 100
                auto u = t_year / 100.0;

                // ΔT = 10583.6 - 1014.41 * u + 33.78311 * u^2 - 5.952053 * u^3 -
                // 0.1798452 * u ^ 4 + 0.022174192 * u ^ 5 + 0.0090316521 * u ^ 6
                return 10583.6 - 1014.41 * u + 33.78311 * std::pow(u, 2) - 5.952053 * std::pow(u, 3) - 0.1798452 * std::pow(u, 4) + 0.022174192 * std::pow(u, 5) + 0.0090316521 * std::pow(u, 6);
            }

            if (t_year < 1600) {
                // u = (y - 1000) / 100
                auto u = (t_year - 1000) / 100.0;

                // ΔT = 1574.2 - 556.01 * u + 71.23472 * u^2 + 0.319781 * u^3 -
                // 0.8503463 * u ^ 4 - 0.005050998 * u ^ 5 + 0.0083572073 * u ^ 6
                return 1574.2 - 556.01 * u + 71.23472 * std::pow(u, 2) + 0.319781 * std::pow(u, 3) - 0.8503463 * std::pow(u, 4) - 0.005050998 * std::pow(u, 5) + 0.0083572073 * std::pow(u, 6);
            }

            if (t_year < 1700) {
                // u = y - 1600
                auto u = t_year - 1600;

                // ΔT = 120 - 0.9808 * u - 0.01532 * u^2 + u^3 / 7129
                return 120 - 0.9808 * u - 0.01532 * std::pow(u, 2) + std::pow(u, 3) / 7129.0;
            }

            if (t_year < 1800) {
                // u = y - 1700
                auto u = t_year - 1700;

                // ΔT = 8.83 + 0.1603 * u - 0.0059285 * u^2 + 0.00013336 * u^3 - u^4 /
                // 1174000
                return 8.83 + 0.1603 * u - 0.0059285 * std::pow(u, 2) + 0.00013336 * std::pow(u, 3) - std::pow(u, 4) / 1174000.0;
            }

            if (t_year < 1860) {
                // u = y - 1800
                auto u = t_year - 1800;

                // ΔT = 13.72 - 0.332447 * u + 0.0068612 * u^2 + 0.0041116 * u^3 -
                // 0.00037436 * u^4 + 0.0000121272 * u^5 - 0.0000001699 * u^6 +
                // 0.000000000875 * u^7
                return 13.72 - 0.332447 * u + 0.0068612 * std::pow(u, 2) + 0.0041116 * std::pow(u, 3) - 0.00037436 * std::pow(u, 4) + 0.0000121272 * std::pow(u, 5) - 0.0000001699 * std::pow(u, 6)
                     + 0.000000000875 * std::pow(u, 7);
            }

            if (t_year < 1900) {
                // u = y - 1860
                auto u = t_year - 1860;

                // ΔT = 7.62 + 0.5737 * t - 0.251754 * t^2 + 0.01680668 * t^3
                // -0.0004473624 * t ^ 4 + t ^ 5 / 233174
                return 7.62 + 0.5737 * u - 0.251754 * std::pow(u, 2) + 0.01680668 * std::pow(u, 3) - 0.0004473624 * std::pow(u, 4) + std::pow(u, 5) / 233174.0;
            }

            if (t_year < 1920) {
                // u = y - 1900
                auto u = t_year - 1900;

                // ΔT = -2.79 + 1.494119 * u - 0.0598939 * u^2 + 0.0061966 * u^3 -
                // 0.000197 * u^4
                return -2.79 + 1.494119 * u - 0.0598939 * std::pow(u, 2) + 0.0061966 * std::pow(u, 3) - 0.000197 * std::pow(u, 4);
            }

            if (t_year < 1941) {
                // u = y - 1920
                auto u = t_year - 1920;

                // ΔT = 21.20 + 0.84493 * u - 0.076100 * u^2 + 0.0020936 * u^3
                return 21.20 + 0.84493 * u - 0.076100 * std::pow(u, 2) + 0.0020936 * std::pow(u, 3);
            }

            if (t_year < 1961) {
                // u = y - 1950
                auto u = t_year - 1950;

                // ΔT = 29.07 + 0.407 * u - u^2 / 233 + u^3 / 2547
                return 29.07 + 0.407 * u - std::pow(u, 2) / 233.0 + std::pow(u, 3) / 2547.0;
            }

            if (t_year < 1986) {
                // u = y - 1975
                auto u = t_year - 1975;

                // ΔT = 45.45 + 1.067 * u - u^2 / 260 - u^3 / 718
                return 45.45 + 1.067 * u - std::pow(u, 2) / 260.0 - std::pow(u, 3) / 718.0;
            }

            if (t_year < 2005) {
                // u = y - 2000
                auto u = t_year - 2000;

                // ΔT = 63.86 + 0.3345 * u - 0.060374 * u^2 + 0.0017275 * u^3 +
                // 0.000651814 * u^4 + 0.00002373599 * u^5
                return 63.86 + 0.3345 * u - 0.060374 * std::pow(u, 2) + 0.0017275 * std::pow(u, 3) + 0.000651814 * std::pow(u, 4) + 0.00002373599 * std::pow(u, 5);
            }

            if (t_year < 2050) {
                // u = y - 2000
                auto u = t_year - 2000;

                // ΔT = 62.92 + 0.32217 * u + 0.005589 * u^2
                return 62.92 + 0.32217 * u + 0.005589 * std::pow(u, 2);
            }

            if (t_year < 2150) {
                // u = (y - 2000) / 100
                auto u = (t_year - 1820) / 100.0;

                // ΔT = -20 + 32 * ((y-1820)/100)^2 - 0.5628 * (2150 - y)
                return -20 + 32 * std::pow(u, 2) - 0.5628 * (2150 - t_year);
            }

            // u = (y - 1820) / 100
            auto u = (t_year - 1820) / 100.0;

            // ΔT = -20 + 32 * u^2
            return -20.0 + 32 * std::pow(u, 2);
        }

        double calcG(const double t) {
            auto century = (t - 2451545.0) / 36525.0;

            return (357.528 + 35'999.05 * century) * std::numbers::pi / 180.0;
        }

        double diffDelta(const double t) {
            auto g = calcG(t);

            auto diff = 35'999.05 * std::numbers::pi / 180.0;

            return diff * 0.001658 * std::cos(g + 0.0167 * std::sin(g)) * (1 + 0.0167 * std::cos(g));
        }

        double equationDelta(const double t) {
            auto g = calcG(t);

            return 0.001658 * std::sin(g + 0.0167 * std::sin(g));
        }

        double TDB2TT(double t) {
            while (true) {
                const auto delta = equationDelta(t) / diffDelta(t);

                t -= delta;

                if (std::abs(delta) < 1e-12) break;
            }

            return t;
        }

        double TT2TDB(const double t) { return t + equationDelta(t); }

        double TAI2TT(const double t) { return t + 32.184; }

        double TT2TAI(const double t) { return t - 32.184; }

        double UTC2TAI(const double t) { return t + leapSecond(t); }

        double TAI2UTC(const double t) { return t - leapSecond(t); }

        double UTC2TT(const double t) { return TAI2TT(UTC2TAI(t)); }

        double TAI2TDB(const double t) { return TT2TDB(TAI2TT(t)); }

        double UTC2TDB(const double t) { return TT2TDB(TAI2TT(UTC2TAI(t))); }

        double TDB2TAI(const double t) { return TT2TAI(TDB2TT(t)); }

        double TT2UTC(const double t) { return TAI2UTC(TT2TAI(t)); }

        double TDB2UTC(const double t) { return TAI2UTC(TT2TAI(TDB2TT(t))); }

        double toTT(const double t, TimeScale from) {
            switch (from) {
                case TT: return t;
                case TDB: return TDB2TT(t);
                case TAI: return TAI2TT(t);
                case UTC: return UTC2TT(t);
                default: return t;
            }
        }

        double toTDB(const double t, TimeScale from) {
            switch (from) {
                case TDB: return t;
                case TT: return TT2TDB(t);
                case TAI: return TAI2TDB(t);
                case UTC: return UTC2TDB(t);
                default: return t;
            }
        }

        double toTAI(const double t, TimeScale from) {
            switch (from) {
                case TAI: return t;
                case TT: return TT2TAI(t);
                case TDB: return TDB2TAI(t);
                case UTC: return UTC2TAI(t);
                default: return t;
            }
        }

        double toUTC(const double t, TimeScale from) {
            switch (from) {
                case UTC: return t;
                case TT: return TT2UTC(t);
                case TDB: return TDB2UTC(t);
                case TAI: return TAI2UTC(t);
                default: return t;
            }
        }
    }  // namespace cvt

    std::string LunarDate::toString() const {
        std::ostringstream oss;

        oss << "LunarDate(" << year << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second << ")";

        return oss.str();
    }

    double convert(const double t, TimeScale from, TimeScale to) {
        if (from == to) return t;

        switch (to) {
            case TT: return cvt::toTT(t, from);
            case TDB: return cvt::toTDB(t, from);
            case TAI: return cvt::toTAI(t, from);
            case UTC: return cvt::toUTC(t, from);
            default: return t;
        }
    }

    constexpr JulianDay J2000 = JulianDay(2451545.0, TT);

    double julianCentury(const JulianDay& jd) { return (jd.to(TT).value - J2000.value) / 36525.0; }

    double julianCentury(const JulianDay& jd, TimeScale scale) { return (jd.to(scale).value - J2000.to(scale).value) / 36525.0; }

    constexpr std::array<Term, 24> TERM_TABLE = {Term::StartOfSprint, Term::RainWater, Term::InsectsAwaken, Term::SpringEquinox,  Term::ClearAndBright, Term::GrainRain,
                                                 Term::StartOfSummer, Term::GrainFull, Term::GrainInEar,    Term::SummerSolstice, Term::MinorHeat,      Term::MajorHeat,
                                                 Term::StartOfAutumn, Term::EndOfHeat, Term::WhiteDew,      Term::AutumnEquinox,  Term::ColdDew,        Term::FrostFalls,
                                                 Term::StartOfWinter, Term::MinorSnow, Term::MajorSnow,     Term::WinterSolstice, Term::MinorCold,      Term::MajorCold};


}  // namespace astro
