// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file vsop.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/10 16:18
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "vsop.h"
#include "utils.h"
#include <format>
#include <numbers>

namespace astro::vsop {
    double lambdaMercury(const double t) { return 4.402608631669 + 26087.90314068555 * t; }

    double lambdaVenus(const double t) { return 3.176134461576 + 10213.28554743445 * t; }

    double lambdaEarthMoon(const double t) { return 1.753470369433 + 6283.075850353215 * t; }

    double lambdaMars(const double t) { return 6.203500014141 + 3340.612434145457 * t; }

    double lambdaVesta(const double t) { return 4.091360003050 + 1731.170452721855 * t; }

    double lambdaIris(const double t) { return 1.713740719173 + 1704.450855027201 * t; }

    double lambdaBamberga(const double t) { return 5.598641292287 + 1428.948917844273 * t; }

    double lambdaCeres(const double t) { return 2.805136360408 + 1364.756513629990 * t; }

    double lambdaPallas(const double t) { return 2.326989734620 + 1361.923207632842 * t; }

    double lambdaJupiter(const double t) { return 0.599546107035 + 529.6909615623250 * t; }

    double lambdaSaturn(const double t) { return 0.874018510107 + 213.2990861084880 * t; }

    double lambdaUranus(const double t) { return 5.481225395663 + 74.78165903077800 * t; }

    double lambdaNeptune(const double t) { return 5.311897933164 + 38.13297222612500 * t; }

    double lambdaPluto(const double t) { return 0.3595362285049309 * t; }

    double lambdaMoonD(const double t) { return 5.198466400630 + 77713.7714481804 * t; }

    double lambdaMoonF(const double t) { return 1.627905136020 + 84334.6615717837 * t; }

    double lambdaMoonL(const double t) { return 2.35555638750 + 83286.9142477147 * t; }

    extern const std::vector<std::function<double(double)>> LAMBDA_TABLE = {lambdaMercury,  lambdaVenus, lambdaEarthMoon, lambdaMars,    lambdaVesta,  lambdaIris,
                                                                            lambdaBamberga, lambdaCeres, lambdaPallas,    lambdaJupiter, lambdaSaturn, lambdaUranus,
                                                                            lambdaNeptune,  lambdaPluto, lambdaMoonD,     lambdaMoonF,   lambdaMoonL};

    double calcPhi(const double t, const std::vector<std::shared_ptr<reader::Literal>>& data) {
        double result{};

        for (std::size_t i{}; i < data.size(); ++i)
            result += std::visit(
                [&]<typename T>(T&& arg) -> double {
                    if constexpr (std::is_same_v<T, std::string>) {
                        std::cerr << "Waring: " << arg << " is not a valid term in the VSOP model. It will be ignored." << std::endl;
                        return std::stod(arg) * LAMBDA_TABLE[i](t);
                    } else
                        return arg * LAMBDA_TABLE[i](t);
                },
                data[i]->value()
            );

        return result;
    }

    double calcSeries(const double t, const std::shared_ptr<reader::Term>& term) {
        const auto phi = calcPhi(t, term->coefficients);

        return std::get<double>(term->sinMantissa->value()) * std::pow(10, std::get<int>(term->sinExponent->value())) * std::sin(phi)
             + std::get<double>(term->cosMantissa->value()) * std::pow(10, std::get<int>(term->cosExponent->value())) * std::cos(phi);
    }

    double calcEccentricity(const double k, const double h) { return std::sqrt(k * k + h * h); }

    double calcPerihelionLongitude(double k, double h) { return std::atan2(h, k); }

    double calcMeanAnomaly(double l, double perihelionLongitude) { return l - perihelionLongitude; }

    double calcOrbitInclination(double q, double p) { return 2 * std::asin(std::sqrt(q * q + p * p)); }

    double calcAscendingNodeLongitude(double q, double p) { return std::atan2(p, q); }

    double equationKepler(const double x, const double eccentricity, const double meanAnomaly) { return x - eccentricity * std::sin(x) - meanAnomaly; }

    double diffKepler(const double x, const double eccentricity) { return 1 - eccentricity * std::cos(x); }

    double calcKepler(double x, const double eccentricity, const double meanAnomaly) {
        while (true) {
            const auto delta = equationKepler(x, eccentricity, meanAnomaly) / diffKepler(x, eccentricity);

            x -= delta;

            if (std::abs(delta) < 1e-12) break;
        }

        return x;
    }

    double calcTrueAnomaly(double eccentricAnomaly, double eccentricity) { return 2 * std::atan(std::sqrt((1 + eccentricity) / (1 - eccentricity)) * std::tan(eccentricAnomaly / 2)); }

    double calcHeliocentricDistance(double a, double eccentricity, double trueAnomaly) { return a * (1 - eccentricity * eccentricity) / (1 + eccentricity * std::cos(trueAnomaly)); }

    template<typename T>
    std::tuple<T, T, T, T, T, T> calcCoefficents(double t, const reader::Data& data) {
        double series[6] = {0};

        for (const auto& table : data.tables) {
            auto key = std::get<int>(dynamic_cast<reader::Integer*>(table->header->fields[2].get())->value()) - 1;

            for (int i = 1; i < table->terms.size(); ++i) series[key] += binPow(t, i) * calcSeries(t, table->terms[i]);
        }

        return {series[0], series[1], series[2], series[3], series[4], series[5]};
    }

    template std::tuple<double, double, double, double, double, double> calcCoefficents(double t, const reader::Data& data);

    GeoCoord<double, double, double> vsop2013(double tdb_jd_C, const reader::Data& data) {
        if (std::abs(tdb_jd_C) > 100) throw std::invalid_argument(std::format("The time {} exceeds the supported range of Vsop2013.", tdb_jd_C));

        const auto [a, l, k, h, p, q] = calcCoefficents<double>(tdb_jd_C, data);
        // rangeCheck(a, 0.3, 40 * AU);
        // rangeCheck(l, 0.0, 2 * std::numbers::pi);
        // rangeCheck(k, -0.3, 0.3);
        // rangeCheck(h, -0.3, 0.3);
        // rangeCheck(p, -0.25, 0.25);
        // rangeCheck(q, -0.25, 0.25);

        // 偏心率(e)
        auto eccentricity = calcEccentricity(k, h);
        rangeCheck(eccentricity, 0.0, 0.5);

        // 近日点黄经(\Pi)
        auto perihelionLongitude = calcPerihelionLongitude(k, h);
        rangeCheck(perihelionLongitude, 0.0, 2 * std::numbers::pi);

        // 平近点角(M)
        auto meanAnomaly = calcMeanAnomaly(l, perihelionLongitude);
        rangeCheck(meanAnomaly, 0.0, 2 * std::numbers::pi);

        // 轨道倾角(i)
        auto orbitInclination = calcOrbitInclination(q, p);
        rangeCheck(orbitInclination, 0.0, 0.6);

        // 升交点黄经(\Omega)
        auto ascendingNodeLongitude = calcAscendingNodeLongitude(q, p);
        rangeCheck(ascendingNodeLongitude, 0.0, 2 * std::numbers::pi);

        // 偏近点角(E)
        auto eccentricAnomaly = calcKepler(meanAnomaly, eccentricity, meanAnomaly);
        rangeCheck(eccentricAnomaly, 0.0, 2 * std::numbers::pi);

        // 真近点角(\nu)
        auto trueAnomaly = calcTrueAnomaly(eccentricAnomaly, eccentricity);
        rangeCheck(trueAnomaly, 0.0, 2 * std::numbers::pi);

        // 日心距(r)
        auto heliocentricDistance = calcHeliocentricDistance(a, eccentricity, trueAnomaly);
        rangeCheck(heliocentricDistance, a * (1 - eccentricity), a * (1 + eccentricity));

        auto theta = perihelionLongitude - trueAnomaly;
        auto x     = heliocentricDistance * (std::cos(ascendingNodeLongitude) * std::cos(theta) - std::sin(ascendingNodeLongitude) * std::sin(theta) * std::cos(orbitInclination));
        auto y     = heliocentricDistance * (std::sin(ascendingNodeLongitude) * std::cos(theta) + std::cos(ascendingNodeLongitude) * std::sin(theta) * std::cos(orbitInclination));
        auto z     = heliocentricDistance * std::sin(theta) * std::sin(orbitInclination);

        auto V = std::atan2(-y, -x);
        rangeCheck(V, 0.0, 2 * std::numbers::pi);
        auto U = -std::asin(z / heliocentricDistance);
        rangeCheck(U, -orbitInclination, orbitInclination);

        return {heliocentricDistance, V, U};
    }

}  // namespace astro::vsop
