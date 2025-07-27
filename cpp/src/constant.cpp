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
    const double AU = 149'597'870'700.0;

    const double J2000 = 2451545.0;

    double DateTime::toJulianDay() const {
        // 处理公元前年份情况
        int y = year < 0 ? year + 1 : year;
        int m = month;
        int d = day;

        if (m <= 2) {
            y--;
            m += 12;
        }

        // 格里高利历修正（1582年10月5日至1582年10月14日）
        int A = y / 100;
        int B = y < 1582 || (y == 1582 && m < 10) || (y == 1582 && m == 10 && d < 15) ? 0 : 2 - A + A / 4;

        // 计算儒略日数
        double jd = std::floor(365.25 * (y + 4716)) + std::floor(30.6001 * (m + 1)) + d + B - 1524.5;

        return jd + (hour + minute / 60.0 + second / 3600.0) / 24.0;
    }

    DateTime Duration::toDateTime() const {
        DateTime dt{};

        dt.scale = scale;

        auto jd = value;

        int Z    = std::floor(jd);
        double F = jd - Z;

        int A = Z < 2299161 ? Z : static_cast<int>((Z - 1867216.25) / 36524.25);
        int B = Z + 1 + A - A / 4;

        int C = B + 1524;
        int D = std::floor((C - 122.1) / 365.25);
        int E = std::floor(365.25 * D);
        int G = std::floor((C - E) / 30.6001);

        dt.day   = C - E + F - std::floor(30.6001 * G);
        dt.month = G - (G < 13.5 ? 1 : 13);
        dt.year  = D - (dt.month > 2.5 ? 4716 : 4715);

        double h  = F * 24.0;
        dt.hour   = static_cast<int>(h);
        double m  = (h - dt.hour) * 60.0;
        dt.minute = static_cast<int>(m);
        dt.second = static_cast<int>((m - dt.minute) * 60.0);

        return dt;
    }

    namespace vsop2013 {}  // namespace vsop2013

}  // namespace astro
