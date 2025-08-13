// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file constant.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 11:48
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CONSTANT_H
#define CONSTANT_H
#pragma once

#include <array>
#include <string>

namespace astro {
    template<typename R, typename U, typename V>
    struct GeocentricCoordinate {
        R geocentricDistance;
        V longitude;
        U latitude;
    };

    template<typename R, typename U, typename V>
    using GeoCoord = GeocentricCoordinate<R, U, V>;

    extern const double LIGHT_SPEED;

    extern const double AU;

    enum TimeScale { UTC, TAI, TT, TDB };

    struct DateTime;

    struct JulianDay {
        double value;

        TimeScale scale;

        [[nodiscard]] DateTime toDateTime() const;

        [[nodiscard]] JulianDay to(TimeScale scale) const;
    };

    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        double second;

        TimeScale scale;

        [[nodiscard]] JulianDay toJulianDay() const;
    };

    struct LunarDate {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        double second;

        bool isLeap;

        std::string toString() const;
    };

    double convert(double t, TimeScale from, TimeScale to);

    extern const JulianDay J2000;

    double julianCentury(const JulianDay& jd);

    double julianCentury(const JulianDay& jd, TimeScale scale);

    enum class Term {
        ///< 立春
        StartOfSprint = 0,
        ///< 雨水
        RainWater = 15,
        ///< 惊蛰
        InsectsAwaken = 30,
        ///< 春分
        SpringEquinox = 45,
        ///< 清明
        ClearAndBright = 60,
        ///< 谷雨
        GrainRain = 75,
        ///< 立夏
        StartOfSummer = 90,
        ///< 小满
        GrainFull = 105,
        ///< 芒种
        GrainInEar = 120,
        ///< 夏至
        SummerSolstice = 135,
        ///< 小暑
        MinorHeat = 150,
        ///< 大暑
        MajorHeat = 165,
        ///< 立秋
        StartOfAutumn = 180,
        ///< 处暑
        EndOfHeat = 195,
        ///< 白露
        WhiteDew = 210,
        ///< 秋分
        AutumnEquinox = 225,
        ///< 寒露
        ColdDew = 240,
        ///< 霜降
        FrostFalls = 255,
        ///< 立冬
        StartOfWinter = 270,
        ///< 小雪
        MinorSnow = 285,
        ///< 大雪
        MajorSnow = 300,
        ///< 冬至
        WinterSolstice = 315,
        ///< 小寒
        MinorCold = 330,
        ///< 大寒
        MajorCold = 345,
    };

    extern const std::array<Term, 24> TERM_TABLE;
}  // namespace astro

#endif  // CONSTANT_H
