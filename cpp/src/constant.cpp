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
 * @date 2025/07/26 16:25
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "constant.h"

namespace astro {
    const JulianDay J2000{2451545.0, UTC};

    namespace cvt {
        using DegreeParCentury =
            Unit<merge_dimensions_t<DimensionList<Dimension<AngleTag, 1>>, dimension_power_t<-1, DimensionList<Dimension<TimeTag, 1>>>>, std::ratio_divide<Degree::ratio, Century::ratio>>;

        constexpr Val<Second, double> leapSecond(const JulianDay& jd) {
            auto dt = jd.toDateTime();

            if (dt.year < -500) {
                // u = (y - 1820) / 100
                auto u = (dt.year - 1820) / 100.0;

                // ΔT = -20 + 32 * u^2
                return -20 + 32 * std::pow(u, 2);
            }

            if (dt.year < 500) {
                // u = y / 100
                auto u = dt.year / 100.0;

                // ΔT = 10583.6 - 1014.41 * u + 33.78311 * u^2 - 5.952053 * u^3 -
                // 0.1798452 * u ^ 4 + 0.022174192 * u ^ 5 + 0.0090316521 * u ^ 6
                return 10583.6 - 1014.41 * u + 33.78311 * std::pow(u, 2) - 5.952053 * std::pow(u, 3) - 0.1798452 * std::pow(u, 4) + 0.022174192 * std::pow(u, 5) + 0.0090316521 * std::pow(u, 6);
            }

            if (dt.year < 1600) {
                // u = (y - 1000) / 100
                auto u = (dt.year - 1000) / 100.0;

                // ΔT = 1574.2 - 556.01 * u + 71.23472 * u^2 + 0.319781 * u^3 -
                // 0.8503463 * u ^ 4 - 0.005050998 * u ^ 5 + 0.0083572073 * u ^ 6
                return 1574.2 - 556.01 * u + 71.23472 * std::pow(u, 2) + 0.319781 * std::pow(u, 3) - 0.8503463 * std::pow(u, 4) - 0.005050998 * std::pow(u, 5) + 0.0083572073 * std::pow(u, 6);
            }

            if (dt.year < 1700) {
                // u = y - 1600
                auto u = dt.year - 1600;

                // ΔT = 120 - 0.9808 * u - 0.01532 * u^2 + u^3 / 7129
                return 120 - 0.9808 * u - 0.01532 * std::pow(u, 2) + std::pow(u, 3) / 7129.0;
            }

            if (dt.year < 1800) {
                // u = y - 1700
                auto u = dt.year - 1700;

                // ΔT = 8.83 + 0.1603 * u - 0.0059285 * u^2 + 0.00013336 * u^3 - u^4 /
                // 1174000
                return 8.83 + 0.1603 * u - 0.0059285 * std::pow(u, 2) + 0.00013336 * std::pow(u, 3) - std::pow(u, 4) / 1174000.0;
            }

            if (dt.year < 1860) {
                // u = y - 1800
                auto u = dt.year - 1800;

                // ΔT = 13.72 - 0.332447 * u + 0.0068612 * u^2 + 0.0041116 * u^3 -
                // 0.00037436 * u^4 + 0.0000121272 * u^5 - 0.0000001699 * u^6 +
                // 0.000000000875 * u^7
                return 13.72 - 0.332447 * u + 0.0068612 * std::pow(u, 2) + 0.0041116 * std::pow(u, 3) - 0.00037436 * std::pow(u, 4) + 0.0000121272 * std::pow(u, 5) - 0.0000001699 * std::pow(u, 6)
                     + 0.000000000875 * std::pow(u, 7);
            }

            if (dt.year < 1900) {
                // u = y - 1860
                auto u = dt.year - 1860;

                // ΔT = 7.62 + 0.5737 * t - 0.251754 * t^2 + 0.01680668 * t^3
                // -0.0004473624 * t ^ 4 + t ^ 5 / 233174
                return 7.62 + 0.5737 * u - 0.251754 * std::pow(u, 2) + 0.01680668 * std::pow(u, 3) - 0.0004473624 * std::pow(u, 4) + std::pow(u, 5) / 233174.0;
            }

            if (dt.year < 1920) {
                // u = y - 1900
                auto u = dt.year - 1900;

                // ΔT = -2.79 + 1.494119 * u - 0.0598939 * u^2 + 0.0061966 * u^3 -
                // 0.000197 * u^4
                return -2.79 + 1.494119 * u - 0.0598939 * std::pow(u, 2) + 0.0061966 * std::pow(u, 3) - 0.000197 * std::pow(u, 4);
            }

            if (dt.year < 1941) {
                // u = y - 1920
                auto u = dt.year - 1920;

                // ΔT = 21.20 + 0.84493 * u - 0.076100 * u^2 + 0.0020936 * u^3
                return 21.20 + 0.84493 * u - 0.076100 * std::pow(u, 2) + 0.0020936 * std::pow(u, 3);
            }

            if (dt.year < 1961) {
                // u = y - 1950
                auto u = dt.year - 1950;

                // ΔT = 29.07 + 0.407 * u - u^2 / 233 + u^3 / 2547
                return 29.07 + 0.407 * u - std::pow(u, 2) / 233.0 + std::pow(u, 3) / 2547.0;
            }

            if (dt.year < 1986) {
                // u = y - 1975
                auto u = dt.year - 1975;

                // ΔT = 45.45 + 1.067 * u - u^2 / 260 - u^3 / 718
                return 45.45 + 1.067 * u - std::pow(u, 2) / 260.0 - std::pow(u, 3) / 718.0;
            }

            if (dt.year < 2005) {
                // u = y - 2000
                auto u = dt.year - 2000;

                // ΔT = 63.86 + 0.3345 * u - 0.060374 * u^2 + 0.0017275 * u^3 +
                // 0.000651814 * u^4 + 0.00002373599 * u^5
                return 63.86 + 0.3345 * u - 0.060374 * std::pow(u, 2) + 0.0017275 * std::pow(u, 3) + 0.000651814 * std::pow(u, 4) + 0.00002373599 * std::pow(u, 5);
            }

            if (dt.year < 2050) {
                // u = y - 2000
                auto u = dt.year - 2000;

                // ΔT = 62.92 + 0.32217 * u + 0.005589 * u^2
                return 62.92 + 0.32217 * u + 0.005589 * std::pow(u, 2);
            }

            if (dt.year < 2150) {
                // u = (y - 2000) / 100
                auto u = (dt.year - 1820) / 100.0;

                // ΔT = -20 + 32 * ((y-1820)/100)^2 - 0.5628 * (2150 - y)
                return -20 + 32 * std::pow(u, 2) - 0.5628 * (2150 - dt.year);
            }

            // u = (y - 1820) / 100
            auto u = (dt.year - 1820) / 100.0;

            // ΔT = -20 + 32 * u^2
            return -20.0 + 32 * std::pow(u, 2);
        }

        constexpr auto calcG(const JulianDay& jd) {
            auto diff = jd - J2000;

            auto century = value<Day>(diff.value()).to<Century>();

            return (357.528_aD + value<DegreeParCentury>(35'999.05) * century).to<Radian>();
        }

        constexpr auto diffG(const JulianDay& jd) {
            auto g = calcG(jd);

            auto gDiff = 35999.05_aD .to<Radian>();

            return gDiff * 0.001658_tS * math::cos(g + 0.0167_aR * math::sin(g)) * (1 + 0.0167 * math::cos(g));
        }

        constexpr auto delta(const JulianDay& jd) {
            auto g = calcG(jd);

            return 0.001658_tS * math::sin(g + 0.0167_aR * math::sin(g));
        }

        constexpr JulianDay TDB2TT(const JulianDay& jd) {
            // 假设jd_0 = jd_tdb
            auto prev = jd;
            auto diff = delta(prev);

            while (math::abs(diff) < 1e-12) {
                prev = prev - diff;
                diff = delta(prev);
            }

            return prev;
        }

        constexpr JulianDay TT2TDB(const JulianDay& jd) { return jd + delta(jd); }

        constexpr JulianDay TAI2TT(const JulianDay& jd) { return jd + 32.184_tS; }

        constexpr JulianDay TT2TAI(const JulianDay& jd) { return jd - 32.184_tS; }

        constexpr JulianDay UTC2TAI(const JulianDay& jd) { return jd + leapSecond(jd); }

        constexpr JulianDay TAI2UTC(const JulianDay& jd) { return jd - leapSecond(jd); }

        constexpr JulianDay UTC2TT(const JulianDay& jd) { return TAI2TT(UTC2TAI(jd)); }

        constexpr JulianDay TAI2TDB(const JulianDay& jd) { return TT2TDB(TAI2TT(jd)); }

        constexpr JulianDay UTC2TDB(const JulianDay& jd) { return TT2TDB(TAI2TT(UTC2TAI(jd))); }

        constexpr JulianDay TDB2TAI(const JulianDay& jd) { return TT2TAI(TDB2TT(jd)); }

        constexpr JulianDay TT2UTC(const JulianDay& jd) { return TAI2UTC(TT2TAI(jd)); }

        constexpr JulianDay TDB2UTC(const JulianDay& jd) { return TAI2UTC(TT2TAI(TDB2TT(jd))); }

        constexpr JulianDay toTT(const JulianDay& jd) {
            switch (jd.scale()) {
                case TT: return jd;
                case TDB: return TDB2TT(jd);
                case TAI: return TAI2TT(jd);
                case UTC: return UTC2TT(jd);
                default: return jd;
            }
        }

        constexpr JulianDay toTDB(const JulianDay& jd) {
            switch (jd.scale()) {
                case TDB: return jd;
                case TT: return TT2TDB(jd);
                case TAI: return TAI2TDB(jd);
                case UTC: return UTC2TDB(jd);
                default: return jd;
            }
        }

        constexpr JulianDay toTAI(const JulianDay& jd) {
            switch (jd.scale()) {
                case TAI: return jd;
                case TT: return TT2TAI(jd);
                case TDB: return TDB2TAI(jd);
                case UTC: return UTC2TAI(jd);
                default: return jd;
            }
        }

        constexpr JulianDay toUTC(const JulianDay& jd) {
            switch (jd.scale()) {
                case UTC: return jd;
                case TT: return TT2UTC(jd);
                case TDB: return TDB2UTC(jd);
                case TAI: return TAI2UTC(jd);
                default: return jd;
            }
        }
    }  // namespace cvt

    constexpr JulianDay convert(const JulianDay& jd, const TimeScale target) {
        if (jd.scale() == target) return jd;

        switch (target) {
            case TT: return cvt::toTT(jd);
            case TDB: return cvt::toTDB(jd);
            case TAI: return cvt::toTAI(jd);
            case UTC: return cvt::toUTC(jd);
            default: return jd;
        }
    }

    namespace vsop2013 {
        double lambdaMercury(double t) { return 4.402608631669 + 26087.90314068555 * t; }

        double lambdaVenus(double t) { return 3.176134461576 + 10213.28554743445 * t; }

        double lambdaEarthMoon(double t) { return 1.753470369433 + 6283.075850353215 * t; }

        double lambdaMars(double t) { return 6.203500014141 + 3340.612434145457 * t; }

        double lambdaVesta(double t) { return 4.091360003050 + 1731.170452721855 * t; }

        double lambdaIris(double t) { return 1.713740719173 + 1704.450855027201 * t; }

        double lambdaBamberga(double t) { return 5.598641292287 + 1428.948917844273 * t; }

        double lambdaCeres(double t) { return 2.805136360408 + 1364.756513629990 * t; }

        double lambdaPallas(double t) { return 2.326989734620 + 1361.923207632842 * t; }

        double lambdaJupiter(double t) { return 0.599546107035 + 529.6909615623250 * t; }

        double lambdaSaturn(double t) { return 0.874018510107 + 213.2990861084880 * t; }

        double lambdaUranus(double t) { return 5.481225395663 + 74.78165903077800 * t; }

        double lambdaNeptune(double t) { return 5.311897933164 + 38.13297222612500 * t; }

        double lambdaPluto(double t) { return 0.3595362285049309 * t; }

        double lambdaMoonD(double t) { return 5.198466400630 + 77713.7714481804 * t; }

        double lambdaMoonF(double t) { return 1.627905136020 + 84334.6615717837 * t; }

        double lambdaMoonL(double t) { return 2.35555638750 + 83286.9142477147 * t; }

    }  // namespace vsop2013

}  // namespace astro
