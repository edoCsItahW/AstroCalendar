// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file calender.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/13 00:51
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CALENDER_HPP
#define CALENDER_HPP
#pragma once

#include "utils.h"

namespace astro {
    template<typename SolarFunc, typename MoonFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult> && coordinateCalcFunc<MoonFunc, double, moonAppCoordResult>
    double findNewMoonMoment(double tdb_jd_C, const SolarFunc& solarCoord, const MoonFunc& moonCoord) {
        auto newMoonEqu = [&](double t) { return moonCoord(t).longitude - solarCoord(t).longitude; };

        return brent(newMoonEqu, tdb_jd_C - 10, tdb_jd_C + 10, 1e-8, 10);
    }

    template<typename SolarFunc, typename MoonFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult> && coordinateCalcFunc<MoonFunc, double, moonAppCoordResult>
    double findNextNewMoon(double tdb_jd_C, const SolarFunc& solarCoord, const MoonFunc& moonCoord) {
        auto newMoonEqu = [&](double t) { return moonCoord(t).longitude - solarCoord(t).longitude; };

        return findRootForward(newMoonEqu, tdb_jd_C, MEAN_LUNAR_MONTH, 1e-8);
    }

    template<typename SolarFunc, typename MoonFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult> && coordinateCalcFunc<MoonFunc, double, moonAppCoordResult>
    double findPrevNewMoon(double tdb_jd_C, const SolarFunc& solarCoord, const MoonFunc& moonCoord) {
        auto newMoonEqu = [&](double t) { return moonCoord(t).longitude - solarCoord(t).longitude; };

        return findRootBackward(newMoonEqu, tdb_jd_C, MEAN_LUNAR_MONTH, 1e-8);
    }

    template<typename SolarFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult>
    double findSolarTerm(double tdb_jd_C, Term longitude, const SolarFunc& solarCoord) {
        auto termEqu = [&](double t) { return solarCoord(t).longitude - static_cast<int>(longitude); };

        return brent(termEqu, tdb_jd_C - 30, tdb_jd_C + 30, 1e-6, 100);
    }

    template<typename SolarFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult>
    double findSolarTermForward(double tdb_jd_C, Term longitude, const SolarFunc& solarCoord) {
        auto termEqu = [&](double t) { return solarCoord(t).longitude - static_cast<int>(longitude); };

        return findRootForward(termEqu, tdb_jd_C, MEAN_LUNAR_MONTH / 2, 1e-6);
    }

    template<typename SolarFunc>
        requires coordinateCalcFunc<SolarFunc, double, solarAppCoordResult>
    double findSolarTermBackward(double tdb_jd_C, Term longitude, const SolarFunc& solarCoord) {
        auto termEqu = [&](double t) { return solarCoord(t).longitude - static_cast<int>(longitude); };

        return findRootBackward(termEqu, tdb_jd_C, MEAN_LUNAR_MONTH / 2, 1e-6);
    }

}  // namespace astro

#endif  // CALENDER_HPP
