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
 * @date 2025/07/26 16:25
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef CONSTANT_H
#define CONSTANT_H
#pragma once

#include "utils.h"
#include <complex>
#include <type_traits>

namespace astro {

    /**
     * @brief
     *
     * @see https://baike.baidu.com/item/%E5%A4%A9%E6%96%87%E5%8D%95%E4%BD%8D/216723 天文单位
     * */
    extern const double AU;

    extern const double J2000;

    template<typename T, typename U = double>
    concept numerical = std::is_arithmetic_v<T> && std::is_convertible_v<T, U>;

    enum TimeScale { TT, UT, TDB, TDT, TAI, UTC, GST, JD };

    struct DateTime;

    struct Duration {
        double value;

        TimeScale scale;

        constexpr void convertTo(TimeScale targetScale);

        [[nodiscard]] constexpr Duration toScale(TimeScale targetScale) const;

        template<TimeScale S = UTC>
        [[nodiscard]] DateTime toDateTime() const;
    };

    struct DateTime {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        double second;

        TimeScale scale;

        constexpr void convertTo(TimeScale targetScale);

        [[nodiscard]] constexpr DateTime toScale(TimeScale targetScale) const;

        [[nodiscard]] double toJulianDay() const;
    };

    template<typename>
    struct convertible_to_time : std::false_type {};

    template<>
    struct convertible_to_time<DateTime> : std::true_type {};

    template<>
    struct convertible_to_time<Duration> : std::true_type {};

    template<typename T>
    inline constexpr bool convertible_to_time_v = convertible_to_time<T>::value;

    template<typename T>
    concept time_type = convertible_to_time_v<T>;

    template<TimeScale F = TDB, TimeScale T = TT>
    struct convert {
        static_assert(F != F, "Unsupported time scale conversion");
    };

    template<>
    struct convert<UTC, JD> {
        template<typename T = double>
            requires time_type<T>
        static constexpr auto value(T utc) -> T {
            if constexpr (
                std::is_same_v<T, Duration> ||
                std::is_same_v<T, DateTime> &&
                utc.scale == UTC
                )
                    return utc;

            else
                return convert<utc.scale, JD>::value(utc);
        }
    };

    constexpr void Duration::convertTo(TimeScale targetScale) {
        if (scale != targetScale)
            this = convert<>
    }


    namespace vsop2013 {
        /**
         * @if zh
         * @brief 计算水星的
         *
         * @tparam T
         * @param t
         * @return
         */
        template<typename T>
            requires numerical<T>
        double lambdaMercury(T t) {
            return 4.402608631669 + 26087.90314068555 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaVenus(T t) {
            return 3.176134461576 + 10213.28554743445 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaEarthMoon(T t) {
            return 1.753470369433 + 6283.075850353215 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaMars(T t) {
            return 6.203500014141 + 3340.612434145457 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaVesta(T t) {
            return 4.091360003050 + 1731.170452721855 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaIris(T t) {
            return 1.713740719173 + 1704.450855027201 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaBamberga(T t) {
            return 5.598641292287 + 1428.948917844273 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaCeres(T t) {
            return 2.805136360408 + 1364.756513629990 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaPallas(T t) {
            return 2.326989734620 + 1361.923207632842 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaJupiter(T t) {
            return 0.599546107035 + 529.6909615623250 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaSaturn(T t) {
            return 0.874018510107 + 213.2990861084880 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaUranus(T t) {
            return 5.481225395663 + 74.78165903077800 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaNeptune(T t) {
            return 5.311897933164 + 38.13297222612500 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaPluto(T t) {
            return 0.3595362285049309 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaMoonD(T t) {
            return 5.198466400630 + 77713.7714481804 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaMoonF(T t) {
            return 1.627905136020 + 84334.6615717837 * t;
        }

        template<typename T>
            requires numerical<T>
        double lambdaMoonL(T t) {
            return 2.35555638750 + 83286.9142477147 * t;
        }

    }  // namespace vsop2013

}  // namespace astro

#endif  // CONSTANT_H
