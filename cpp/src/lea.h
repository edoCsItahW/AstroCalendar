// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file lea.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 01:02
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef LEA_H
#define LEA_H
#pragma once

#include "src/ast.h"
#include "constant.h"
#include <functional>
#include <vector>

namespace astro::lea {
    double meanLongitude(double t);

    double ascendingNodeLongitude(double t);

    double meanAngleDistance(double t);

    double sunMeanAnomaly(double t);

    double moonMeanAnomaly(double t);

    double moonMeanLongitude(double t);

    double lambdaMercury(double t);

    double lambdaVenus(double t);

    double lambdaEarthMoon(double t);

    double lambdaMars(double t);

    double lambdaJupiter(double t);

    double lambdaSaturn(double t);

    double lambdaUranus(double t);

    double lambdaNeptune(double t);

    double generalPrecessionLongitude(double t);

    extern const std::vector<std::function<double(double)>> COEFFICIENTS_TABLE;

    long double calcOmega(double t, const std::vector<std::shared_ptr<reader::Literal>>& data);

    long double calcSeries(double t, const std::shared_ptr<Term>& term, const std::function<long double(long double)>& tragFunc);

    long double calcGeocentricDistance(double t, const reader::Data& data);

    long double calcTrueLongitude(double t, const reader::Data& data);

    long double calcTrueLatitude(double t, const reader::Data& data);

    GeoCoord<long double, long double, long double> lea406(double tdb_jd_C, const reader::Data& rData, const reader::Data& vData, const reader::Data& uData);

}  // namespace astro::lea


#endif  // LEA_H
