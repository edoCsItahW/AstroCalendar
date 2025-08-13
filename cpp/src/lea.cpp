// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file lea.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 01:02
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "lea.h"
#include "utils.h"
#include <cmath>

namespace astro::lea {
    double meanLongitude(double t) { return 218.31664563 + (173256437.2370470 * t - 527.90 * std::pow(t, 2) + 6.6655 * std::pow(t, 3) - 0.5522 * std::pow(t, 4)) / 3600; }

    double ascendingNodeLongitude(double t) { return 125.04455501 - (696791.937631 * t - 636.02 * std::pow(t, 2) - 7.625 * std::pow(t, 3) + 0.3586 * std::pow(t, 4)) / 3600; }

    double meanAngleDistance(double t) { return 297.85019547 + (16029616012.090 * t - 637.06 * std::pow(t, 2) + 6.593 * std::pow(t, 3) - 0.3169 * std::pow(t, 4)) / 3600; }

    double sunMeanAnomaly(double t) { return 357.52910918 + (1295965810.481 * t - 55.32 * std::pow(t, 2) + 0.136 * std::pow(t, 3) - 0.1149 * std::pow(t, 4)) / 3600; }

    double moonMeanAnomaly(double t) { return 134.96340251 + (17179159232.178 * t + 3187.92 * std::pow(t, 2) + 51.635 * std::pow(t, 3) - 2.4470 * std::pow(t, 4)) / 3600; }

    double moonMeanLongitude(double t) { return 93.27209062 + (17395272628.478 * t - 1275.12 * std::pow(t, 2) - 1.037 * std::pow(t, 3) + 0.0417 * std::pow(t, 4)) / 3600; }

    double lambdaMercury(double t) { return 252.25090552 + (5381016286.88982 * t - 1.92789 * std::pow(t, 2) + 0.00639 * std::pow(t, 3)) / 3600; }

    double lambdaVenus(double t) { return 181.97980085 + (2106641364.33548 * t + 0.59381 * std::pow(t, 2) - 0.00627 * std::pow(t, 3)) / 3600; }

    double lambdaEarthMoon(double t) { return 100.46645683 + (1295977422.83429 * t - 2.04411 * std::pow(t, 2) - 0.00523 * std::pow(t, 3)) / 3600; }

    double lambdaMars(double t) { return 355.43299958 + (689050774.93988 * t + 0.94264 * std::pow(t, 2) - 0.01043 * std::pow(t, 3)) / 3600; }

    double lambdaJupiter(double t) { return 34.35151874 + (109256603.77991 * t - 30.60378 * std::pow(t, 2) + 0.05706 * std::pow(t, 3) + 0.04667 * std::pow(t, 4)) / 3600; }

    double lambdaSaturn(double t) { return 50.07744430 + (43996098.55732 * t + 75.61614 * std::pow(t, 2) - 0.16618 * std::pow(t, 3) - 0.11484 * std::pow(t, 4)) / 3600; }

    double lambdaUranus(double t) { return 314.05500511 + (15424811.93933 * t - 1.75083 * std::pow(t, 2) + 0.02156 * std::pow(t, 3)) / 3600; }

    double lambdaNeptune(double t) { return 304.34866548 + (78655032.20744 * t + 0.21103 * std::pow(t, 2) - 0.00895 * std::pow(t, 3)) / 3600; }

    double generalPrecessionLongitude(double t) { return (50288.200 * t + 111.202 * std::pow(t, 2) + 0.0773 * std::pow(t, 3) - 0.2353 * std::pow(t, 4)) / 3600; }

    const std::vector<std::function<double(double)>> COEFFICIENTS_TABLE = {
        ascendingNodeLongitude, meanAngleDistance, sunMeanAnomaly, moonMeanAnomaly, moonMeanLongitude, lambdaMercury, lambdaVenus,
        lambdaEarthMoon,        lambdaMars,        lambdaJupiter,  lambdaSaturn,    lambdaUranus,      lambdaNeptune, generalPrecessionLongitude
    };

    long double calcOmega(double t, const std::vector<std::shared_ptr<reader::Literal>>& data) {
        long double result{};

        for (std::size_t i{}; i < data.size(); ++i)
            result += std::visit(
                [&]<typename T>(T&& arg) -> double {
                    if constexpr (std::is_same_v<T, std::string>) {
                        std::cerr << "Waring: " << arg << " is not a valid term in the LEA model. It will be ignored." << std::endl;
                        return std::stod(arg) * COEFFICIENTS_TABLE[i](t);
                    } else
                        return arg * COEFFICIENTS_TABLE[i](t);
                },
                data[i]->value()
            );

        return result;
    }

    long double calcSeries(double t, const std::shared_ptr<reader::Term>& term, const std::function<long double(long double)>& tragFunc) {
        long double result{};

        auto omega = calcOmega(t, term->coefficients);

        for (std::size_t i{}; i < term->amplitudes.size(); ++i) result += std::get<double>(term->amplitudes[i]->value()) * tragFunc(omega + std::get<double>(term->phases[i]->value()));

        return result;
    }

    long double calcGeocentricDistance(double t, const reader::Data& data) {
        long double result{};

        for (const auto& term : data.terms) result += calcSeries(t, term, [](long double x) { return std::cos(x); });

        return result;
    }

    long double calcTrueLongitude(double t, const reader::Data& data) {
        long double result{};

        for (const auto& term : data.terms) result += calcSeries(t, term, [](long double x) { return std::sin(x); });

        return meanLongitude(t) + result;
    }

    long double calcTrueLatitude(double t, const reader::Data& data) {
        long double result{};

        for (const auto& term : data.terms) result += calcSeries(t, term, [](long double x) { return std::sin(x); });

        return result;
    }

    GeoCoord<long double, long double, long double> lea406(double tdb_jd_C, const reader::Data& rData, const reader::Data& vData, const reader::Data& uData) {
        return {calcGeocentricDistance(tdb_jd_C, rData), calcTrueLongitude(tdb_jd_C, vData), calcTrueLatitude(tdb_jd_C, uData)};
    }

}  // namespace astro::lea
