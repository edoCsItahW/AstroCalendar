// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file calender.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 10:54
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "calender.h"
#include "constant.h"
#include "lea.h"
#include "utils.h"
#include "vsop.h"
#include <cmath>
#include <numbers>

namespace astro {
    GeoCoord<double, double, double> solarApparentCoordinate(double tdb_jd_C, const reader::Data& data) {
        auto trueCoords = vsop::vsop2013(tdb_jd_C, data);

        // 光行时修正
        auto tau = trueCoords.geocentricDistance / LIGHT_SPEED;

        auto travelTimeCorrection = vsop::vsop2013(tdb_jd_C - tau, data);

        const auto [a, l, k, h, p, q]  = vsop::calcCoefficents<double>(tdb_jd_C, data);
        const auto perihelionLongitude = vsop::calcPerihelionLongitude(k, h);

        // 光行差修正
        const auto K = 20.49552;

        auto aberrationFunc = [&](double appLong) -> double {
            double delta = -K * std::cos((appLong - perihelionLongitude) * std::numbers::pi_v<double> / 180) / 3600;

            return travelTimeCorrection.longitude + delta - appLong;
        };

        auto initLong          = travelTimeCorrection.longitude;
        auto apparentLongitude = brent(aberrationFunc, initLong - 0.1, initLong + 0.1, 1e-8, 10);

        auto apparentLatitude =
            -K * std::sin(travelTimeCorrection.latitude * std::numbers::pi_v<double> / 180) * std::sin((apparentLongitude - perihelionLongitude) * std::numbers::pi_v<double> / 180) / 3600
            + travelTimeCorrection.latitude;

        return {travelTimeCorrection.geocentricDistance, apparentLongitude, apparentLatitude};
    }

    GeoCoord<long double, long double, long double> moonApparentCoordinate(double tdb_jd_C, const reader::Data& data, const reader::Data& rData, const reader::Data& vData, const reader::Data& uData) {
        auto moonTrueCoords = lea::lea406(tdb_jd_C, rData, vData, uData);

        auto solarAppLong = solarApparentCoordinate(tdb_jd_C, data).longitude;

        // 光行时修正
        auto tau = moonTrueCoords.geocentricDistance / LIGHT_SPEED;

        auto travelTimeCorrection = lea::lea406(tdb_jd_C - tau, rData, vData, uData);

        // 光行差修正
        const auto k = 20.49552;

        auto deltaV = k * std::cos(travelTimeCorrection.longitude - solarAppLong) / std::cos(travelTimeCorrection.latitude) / 3600;
        auto deltaU = k * std::sin(travelTimeCorrection.latitude) * std::sin(travelTimeCorrection.longitude - solarAppLong) / 3600;

        return {travelTimeCorrection.geocentricDistance, travelTimeCorrection.longitude + deltaV, travelTimeCorrection.latitude + deltaU};
    }

    constexpr double MEAN_LUNAR_MONTH = 29.530588853;

}  // namespace astro
