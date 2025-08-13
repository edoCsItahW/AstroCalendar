// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file vsop.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/10 16:18
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef VSOP_H
#define VSOP_H
#pragma once

#include "src/ast.h"
#include "constant.h"
#include <functional>
#include <vector>

namespace astro::vsop {
    double lambdaMercury(double t);

    double lambdaVenus(double t);

    double lambdaEarthMoon(double t);

    double lambdaMars(double t);

    double lambdaVesta(double t);

    double lambdaIris(double t);

    double lambdaBamberga(double t);

    double lambdaCeres(double t);

    double lambdaPallas(double t);

    double lambdaJupiter(double t);

    double lambdaSaturn(double t);

    double lambdaUranus(double t);

    double lambdaNeptune(double t);

    double lambdaPluto(double t);

    double lambdaMoonD(double t);

    double lambdaMoonF(double t);

    double lambdaMoonL(double t);

    extern const std::vector<std::function<double(double)>> LAMBDA_TABLE;

    double calcPhi(double t, const std::vector<std::shared_ptr<reader::Literal>>& data);

    double calcSeries(double t, const std::shared_ptr<Term>& term);

    GeoCoord<double, double, double> vsop2013(double tdb_jd_C, const reader::Data& data);

    double calcEccentricity(double k, double h);

    double calcPerihelionLongitude(double k, double h);

    double calcMeanAnomaly(double l, double perihelionLongitude);

    double calcOrbitInclination(double q, double p);

    double calcAscendingNodeLongitude(double q, double p);

    double calcKepler(double x, double eccentricity, double meanAnomaly);

    double equationKepler(double x, double eccentricity, double meanAnomaly);

    double diffKepler(double x, double eccentricity);

    double calcTrueAnomaly(double eccentricAnomaly, double eccentricity);

    double calcHeliocentricDistance(double a, double eccentricity, double trueAnomaly);

    template<typename T>
    extern std::tuple<T, T, T, T, T, T> calcCoefficents(double t, const reader::Data& data);
}  // namespace astro::vsop


#endif  // VSOP_H
