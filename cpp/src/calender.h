// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file calender.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 10:54
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CALENDER_H
#define CALENDER_H
#pragma once

#include "src/ast.h"
#include "constant.h"
#include <functional>

namespace astro {
    GeoCoord<double, double, double> solarApparentCoordinate(double tdb_jd_C, const reader::Data& data);

    using solarAppCoordResult = GeoCoord<double, double, double>;

    GeoCoord<long double, long double, long double> moonApparentCoordinate(double tdb_jd_C, const reader::Data& data, const reader::Data& rData, const reader::Data& vData, const reader::Data& uData);

    using moonAppCoordResult = GeoCoord<long double, long double, long double>;

    extern const double MEAN_LUNAR_MONTH;

    template<typename Func, typename A, typename R>
    concept coordinateCalcFunc = requires(Func f, A a) {
        { f(a) } -> std::same_as<R>;
    };

    template<typename SolarFunc, typename MoonFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult> && coordinateCalcFunc<MoonFunc, double, moonAppCoordResult>
    double findNewMoonMoment(double tdb_jd_C, const SolarFunc& solarCoord, const MoonFunc& moonCoord);

    template<typename SolarFunc, typename MoonFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult> && coordinateCalcFunc<MoonFunc, double, moonAppCoordResult>
    double findNextNewMoon(double tdb_jd_C, const SolarFunc& solarCoord, const MoonFunc& moonCoord);

    template<typename SolarFunc, typename MoonFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult> && coordinateCalcFunc<MoonFunc, double, moonAppCoordResult>
    double findPrevNewMoon(double tdb_jd_C, const SolarFunc& solarCoord, const MoonFunc& moonCoord);

    template<typename SolarFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult>
    double findSolarTerm(double tdb_jd_C, Term longitude, const SolarFunc& solarCoord);

    template<typename SolarFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult>
    double findSolarTermForward(double tdb_jd_C, Term longitude, const SolarFunc& solarCoord);

    template<typename SolarFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult>
    double findSolarTermBackward(double tdb_jd_C, Term longitude, const SolarFunc& solarCoord);

}  // namespace astro

#include "calender.hpp"

#endif  // CALENDER_H
