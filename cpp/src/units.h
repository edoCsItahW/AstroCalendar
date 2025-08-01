// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file units.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 14:02
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UNITS_H
#define UNITS_H
#pragma once

#include "utils.h"
#include "value.h"

namespace astro {
    enum TimeScale { UTC, TAI, TT, TDB };

    struct AngleTag {};

    struct DistanceTag {};

    struct TimeTag {};

    // angle unit
    using Radian = Unit<DimensionList<Dimension<AngleTag, 1>>, std::ratio<1>>;

    using Degree = Unit<DimensionList<Dimension<AngleTag, 1>>, std::ratio<314159265, 18000000000>>;

    using Arcminute = Unit<DimensionList<Dimension<AngleTag, 1>>, std::ratio<1, 60>>;

    using Arcsecond = Unit<DimensionList<Dimension<AngleTag, 1>>, std::ratio<1, 3600>>;

    // distance unit
    using Meter = Unit<DimensionList<Dimension<DistanceTag, 1>>, std::ratio<1>>;

    using Kilometer = Unit<DimensionList<Dimension<DistanceTag, 1>>, std::ratio<1000>>;

    using AU = Unit<DimensionList<Dimension<DistanceTag, 1>>, std::ratio<149597870700>>;

    // time unit
    using Second = Unit<DimensionList<Dimension<TimeTag, 1>>, std::ratio<1>>;

    using Minute = Unit<DimensionList<Dimension<TimeTag, 1>>, std::ratio<60>>;

    using Hour = Unit<DimensionList<Dimension<TimeTag, 1>>, std::ratio<3600>>;

    using Day = Unit<DimensionList<Dimension<TimeTag, 1>>, std::ratio<86400>>;

    using Year = Unit<DimensionList<Dimension<TimeTag, 1>>, std::ratio<31557600>>;

    using Century = Unit<DimensionList<Dimension<TimeTag, 1>>, std::ratio<3155760000>>;

    template<char... Cs>
    constexpr Val<Degree, str_type_t<Cs...>> operator""_aD();

    template<char... Cs>
    constexpr Val<Radian, str_type_t<Cs...>> operator""_aR();

    template<char... Cs>
    constexpr Val<Arcminute, str_type_t<Cs...>> operator""_aAM();

    template<char... Cs>
    constexpr Val<Arcsecond, str_type_t<Cs...>> operator""_aAS();

    template<char... Cs>
    constexpr Val<Meter, str_type_t<Cs...>> operator""_dM();

    template<char... Cs>
    constexpr Val<Kilometer, str_type_t<Cs...>> operator""_dK();

    template<char... Cs>
    constexpr Val<AU, str_type_t<Cs...>> operator""_dAU();

    template<char... Cs>
    constexpr auto operator""_tY() -> Val<Year, str_type_t<Cs...>>;

    template<char... Cs>
    constexpr Val<Day, str_type_t<Cs...>> operator""_tD();

    template<char... Cs>
    constexpr Val<Second, str_type_t<Cs...>> operator""_tS();

    template<char... Cs>
    constexpr Val<Century, str_type_t<Cs...>> operator""_tC();

}  // namespace astro

#include "units.hpp"

#endif  // UNITS_H
