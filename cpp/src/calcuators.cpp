// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file calcuators.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/23 17:18
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "calcuators.h"
#include <vector>

const double J2000 = 2451545.0;

const double SYNODIC_MONTH = 29.530588853;  // 朔望月长度

const double TROPICAL_YEAR = 365.24219878;  // 回归年长度

const double PRECESSION[4] = {
    50287.92262,  // 黄经总岁差（角秒/世纪）
    111.24406,    // 章动主项系数
    0.076,        // 章动修正1
    0.000085      // 章动修正2
};
/**
 * @if zh
 *
 * @brief 时间段，用于表示时间段的长度，其等价于儒略日数。
 *
 * @endif
 * */
/**
 * @if zh
 *
 * @brief 将当前时间段转换为指定的时间尺度，并修正@ref value 属性
 *
 * @param s 目标的时间尺度
 *
 * @endif
 */
// 更新章动系数表
const double NUTATION_COEFFS[][9] = {
    {0,  0, 0,  0, 1, -171996, -174.2, 92025, 8.9 },
    {-2, 0, 0,  2, 2, -13187,  -1.6,   5736,  -3.1},
    {0,  0, 0,  2, 2, -2274,   -0.2,   977,   -0.5},
    {0,  0, 0,  0, 2, 2062,    0.2,    -895,  0.5 },
    {0,  1, 0,  0, 0, 1426,    -3.4,   54,    -0.1},
    {0,  0, 1,  0, 0, 712,     0.1,    -7,    0   },
    {0,  0, 0,  2, 1, -517,    1.2,    224,   -0.6},
    {0,  0, 1,  2, 2, -386,    -0.4,   200,   0   },
    {0,  0, -1, 2, 2, -301,    0,      129,   -0.1},
    {1,  0, 0,  0, 0, 217,     -0.5,   -95,   0.3 },
    {0,  0, 1,  0, 1, -158,    0,      0,     0   },
    // 更多系数...
};

// 增加太阳摄动项
const double SOLAR_COEFFS[3][4] = {
    {280.4664567, 36000.76982779, 0.0003032028, 0.0000000209},
    {1.914602,    0.004817,       0.000014,     0.000000001 },
    {0.019993,    -0.000101,      0.000000,     0.000000    }
};

// 增加月球摄动项
const double LUNAR_COEFFS[][4] = {
    {218.3164591, 481267.88134236, -0.0013268, 0.0000019 },
    {134.9629814, 477198.867398,   0.0086972,  0.00000001},
    {93.2720950,  483202.017538,   -0.0036825, 0.0000031 },
    {235.700000,  890534.2230,     -0.0013889, 0.00000017}
};

// 角度归一化函数
double normalize(double angle) {
    angle = std::fmod(angle, 360.0);
    return angle < 0 ? angle + 360.0 : angle;
}

// 儒略日计算
double julianDay(const DateTime &dt) {
    // 处理公元前年份
    int Y = dt.year < 0 ? dt.year + 1 : dt.year;
    int m = dt.month;
    int d = dt.day;

    if (m <= 2) {
        Y--;
        m += 12;
    }

    // 格里高利历修正 (1582年10月15日后)
    int A = Y / 100;
    int B = Y < 1582 || (Y == 1582 && m < 10) || (Y == 1582 && m == 10 && d < 15) ? 0 : 2 - A + A / 4;

    return floor(365.25 * (Y + 4716)) + floor(30.6001 * (m + 1)) + d + B - 1524.5 + (dt.hour + dt.minute / 60.0 + dt.second / 3600.0) / 24.0;
}

// 儒略日转公历
DateTime gregorianDay(double jd) {
    jd += 0.5;
    int Z    = static_cast<int>(jd);
    double F = jd - Z;

    int A = Z < 2299161 ? Z : static_cast<int>((Z - 1867216.25) / 36524.25);
    int B = Z + 1 + A - A / 4;
    int C = B + 1524;
    int D = static_cast<int>((C - 122.1) / 365.25);
    int E = static_cast<int>(365.25 * D);
    int G = static_cast<int>((C - E) / 30.6001);

    int day   = C - E - static_cast<int>(30.6001 * G);
    int month = G < 13.5 ? G - 1 : G - 13;
    int year  = month > 2.5 ? D - 4716 : D - 4715;
    if (year <= 0) year--;

    double time    = F * 24;
    int hour       = static_cast<int>(time);
    double minFrac = (time - hour) * 60;
    int minute     = static_cast<int>(minFrac);
    double second  = (minFrac - minute) * 60;

    return {year, month, day, hour, minute, second};
}

// 修正章动计算
void precessionNutation(const double jd, double &dPsi, double &dEpsilon) {
    double T  = (jd - J2000) / 36525.0;
    double T2 = T * T;
    double T3 = T2 * T;

    // 计算平黄赤交角
    double epsilon0 = 23.4392911111 - 0.0130041667 * T - 0.00000016389 * T2 + 0.00000050361 * T3;

    // 月亮轨道参数
    double D     = normalize(297.85036 + 445267.111480 * T - 0.0019142 * T2 + T3 / 189474.0);
    double M     = normalize(357.52772 + 35999.050340 * T - 0.0001603 * T2 - T3 / 300000.0);
    double Mp    = normalize(134.96298 + 477198.867398 * T + 0.0086972 * T2 + T3 / 56250.0);
    double F     = normalize(93.27191 + 483202.017538 * T - 0.0036825 * T2 + T3 / 327270.0);
    double Omega = normalize(125.04452 - 1934.136261 * T + 0.0020708 * T2 + T3 / 450000.0);

    dPsi     = 0.0;
    dEpsilon = 0.0;

    // 计算章动
    for (const auto &c : NUTATION_COEFFS) {
        double arg = c[0] * D + c[1] * M + c[2] * Mp + c[3] * F + c[4] * Omega;
        arg        = arg * M_PI / 180.0;  // 转为弧度

        dPsi += (c[5] + c[6] * T) * sin(arg);
        dEpsilon += (c[7] + c[8] * T) * cos(arg);
    }

    // 角秒转为弧度
    dPsi /= 3600.0 * 180.0 / M_PI;
    dEpsilon /= 3600.0 * 180.0 / M_PI;
}

// 太阳黄经计算（增加精度）
double solarLongitude(double jd) {
    double T  = (jd - J2000) / 36525.0;
    double T2 = T * T;
    double T3 = T2 * T;

    // 平黄经
    double L0 = SOLAR_COEFFS[0][0] + SOLAR_COEFFS[0][1] * T + SOLAR_COEFFS[0][2] * T2 + SOLAR_COEFFS[0][3] * T3;
    L0        = normalize(L0);

    // 平近点角
    double M     = normalize(357.5291092 + 35999.0502909 * T - 0.0001536 * T2 + T3 / 24490000.0);
    double M_rad = M * M_PI / 180.0;

    // 中心差（增加高阶项）
    double C = (SOLAR_COEFFS[1][0] + SOLAR_COEFFS[1][1] * T + SOLAR_COEFFS[1][2] * T2) * sin(M_rad) + (SOLAR_COEFFS[2][0] + SOLAR_COEFFS[2][1] * T) * sin(2 * M_rad) + 0.000289 * sin(3 * M_rad)
             + 0.000020 * sin(4 * M_rad);

    // 真黄经
    double O = L0 + C;

    // 光行差修正
    O -= 0.00569;

    // 章动修正
    double dPsi, dEpsilon;
    precessionNutation(jd, dPsi, dEpsilon);
    O += dPsi * 180.0 / M_PI;

    return normalize(O);
}

// 月球黄经计算（增加摄动项）
double lunarLongitude(double jd) {
    double T  = (jd - J2000) / 36525.0;
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;

    // 平黄经
    double Lm = LUNAR_COEFFS[0][0] + LUNAR_COEFFS[0][1] * T + LUNAR_COEFFS[0][2] * T2 + LUNAR_COEFFS[0][3] * T3;

    // 平近点角
    double M     = normalize(LUNAR_COEFFS[1][0] + LUNAR_COEFFS[1][1] * T + LUNAR_COEFFS[1][2] * T2 + LUNAR_COEFFS[1][3] * T3);
    double M_rad = M * M_PI / 180.0;

    // 升交点平黄经
    double Omega     = normalize(LUNAR_COEFFS[2][0] + LUNAR_COEFFS[2][1] * T + LUNAR_COEFFS[2][2] * T2 + LUNAR_COEFFS[2][3] * T3);
    double Omega_rad = Omega * M_PI / 180.0;

    // 近地点平黄经
    double F     = normalize(93.272095 + 483202.017523 * T - 0.0036539 * T2 - T3 / 3526000.0 + T4 / 863310000.0);
    double F_rad = F * M_PI / 180.0;

    // 主要摄动项（增加高阶项）
    double perturbation = 6.288750 * sin(M_rad) +                  // 椭圆差
                          1.274018 * sin(2 * F_rad - M_rad) +      // 出差
                          0.658309 * sin(2 * F_rad) +              // 二均差
                          0.213616 * sin(2 * M_rad) -              // 月角差
                          0.185596 * sin(M_rad) * cos(F_rad) +     // 改正项
                          0.114336 * sin(2 * F_rad - 2 * M_rad) +  // 改正项
                          0.058793 * sin(2 * F_rad - 3 * M_rad) +  // 改正项
                          0.057212 * sin(2 * M_rad - M_rad) +      // 改正项
                          0.053320 * sin(2 * F_rad + M_rad) +      // 改正项
                          0.045874 * sin(2 * F_rad - 4 * M_rad) +  // 改正项
                          0.041024 * sin(M_rad - 2 * F_rad) +      // 改正项
                          0.034718 * sin(F_rad) +                  // 改正项
                          0.030465 * sin(M_rad + F_rad) +          // 改正项
                          0.015326 * sin(2 * (F_rad - M_rad)) -    // 新增项
                          0.012528 * sin(3 * M_rad);               // 新增项

    // 真黄经
    double lambda = Lm + perturbation;

    // 章动修正
    double dPsi, dEpsilon;
    precessionNutation(jd, dPsi, dEpsilon);
    lambda += dPsi * 180.0 / M_PI;

    return normalize(lambda);
}

// 朔日计算（增加迭代次数）
double calculateNewMoon(double k) {
    const double baseNM = 2451550.09765;
    double T            = k / 1236.85;
    double T2           = T * T;
    double T3           = T2 * T;

    double jd = baseNM + SYNODIC_MONTH * k + 0.0001337 * T2 - 0.00000015 * T3 + 0.00000000073 * T3 * T;

    // 增加迭代次数至5次
    for (int i = 0; i < 7; i++) {
        double sunLon   = solarLongitude(jd);
        double moonLon  = lunarLongitude(jd);
        double deltaLon = moonLon - sunLon;

        // 归一化角度差
        if (deltaLon > 180.0) deltaLon -= 360.0;
        if (deltaLon < -180.0) deltaLon += 360.0;

        // 使用更精确的月球速度
        const double lunarSpeed = 12.19075 * (1.0 - 0.0001 * T);
        jd -= deltaLon / lunarSpeed;
    }

    return jd;
}

// 查找最近的朔日
double findNewMoonNear(double jd) {
    double k = floor((jd - J2000 - 1) / SYNODIC_MONTH);

    double nm1 = calculateNewMoon(k);
    double nm2 = calculateNewMoon(k + 1);

    return fabs(jd - nm1) < fabs(jd - nm2) ? nm1 : nm2;
}

// 冬至计算（增加迭代次数）
double winterSolstice(int year) {
    DateTime approx(year, 12, 21);
    double jd    = julianDay(approx);
    double lower = jd - 15;
    double upper = jd + 15;

    // 增加迭代次数至12次
    for (int i = 0; i < 15; i++) {
        double mid = (lower + upper) / 2.0;
        double lon = solarLongitude(mid);

        if (lon > 270.1)
            upper = mid;
        else if (lon < 269.9)
            lower = mid;
        else
            return mid;
    }
    return (lower + upper) / 2.0;
}

// 节气计算（扩大搜索范围）
double solarTerm(double jd, int index) {
    double base      = 270.0;
    double targetLon = base + index * 15.0;
    double approxJD  = jd + index * 15.2;
    double lower     = approxJD - 15;
    double upper     = approxJD + 15;

    // 增加迭代次数至12次
    for (int i = 0; i < 15; i++) {
        double mid  = (lower + upper) / 2.0;
        double lon  = solarLongitude(mid);
        double diff = lon - targetLon;

        if (diff > 180.0) diff -= 360.0;
        if (diff < -180.0) diff += 360.0;

        if (diff > 0.1)
            upper = mid;
        else if (diff < -0.1)
            lower = mid;
        else
            return mid;
    }
    return (lower + upper) / 2.0;
}

// 核心改进：完全重写的农历转换函数
LunarDate gregorianToLunar(const DateTime &dt) {
    double jd = julianDay(dt);

    // 1. 计算关键天文点
    double winterJD     = winterSolstice(dt.year);
    double prevWinterJD = winterSolstice(dt.year - 1);
    double nextWinterJD = winterSolstice(dt.year + 1);

    // 2. 构建朔日序列（从去年冬至到明年冬至）
    std::vector<double> newMoons;
    double k0 = floor((prevWinterJD - J2000) / SYNODIC_MONTH) - 2;
    for (int i = 0; i < 30; i++) {
        double nm = calculateNewMoon(k0 + i);
        if (nm < prevWinterJD - 5) continue;
        if (nm > nextWinterJD + 5) break;
        newMoons.push_back(nm);
    }

    // 3. 确定包含jd的朔日区间
    int monthIndex = -1;
    for (size_t i = 0; i < newMoons.size() - 1; i++)
        if (jd >= newMoons[i] && jd < newMoons[i + 1]) {
            monthIndex = i;
            break;
        }

    if (monthIndex == -1)
        // 边界情况处理
        if (jd < newMoons[0])
            monthIndex = 0;
        else
            monthIndex = newMoons.size() - 2;

    int day = static_cast<int>(floor(jd - newMoons[monthIndex])) + 1;

    // 4. 确定农历年（以冬至为界）
    int lunarYear = dt.year;
    if (newMoons[monthIndex] < winterJD)
        lunarYear = dt.year - 1;
    else if (newMoons[monthIndex] >= nextWinterJD)
        lunarYear = dt.year + 1;

    // 5. 确定月份和闰月
    int month        = 0;
    bool isLeapMonth = false;

    // 5.1 计算两个冬至之间的朔日数
    std::vector<double> yearNewMoons;
    for (double nm : newMoons)
        if (nm >= prevWinterJD && nm < nextWinterJD) yearNewMoons.push_back(nm);
    int moonCount = yearNewMoons.size();

    // 5.2 闰月判断
    int leapMonth = -1;
    if (moonCount == 13)
        for (int i = 0; i < moonCount - 1; i++) {
            double midJD  = yearNewMoons[i] + 14.5;
            int termIndex = static_cast<int>(floor((solarLongitude(midJD) - 270.0) / 15.0 + 0.5)) % 24;
            double termJD = solarTerm(midJD, termIndex);

            // 中气应在本月内
            if (termJD < yearNewMoons[i] || termJD >= yearNewMoons[i + 1]) {
                leapMonth = i;
                break;
            }
        }

    // 5.3 计算月份
    // 确定冬至所在月索引
    int winterIndex = -1;
    for (size_t i = 0; i < yearNewMoons.size() - 1; i++)
        if (winterJD >= yearNewMoons[i] && winterJD < yearNewMoons[i + 1]) {
            winterIndex = i;
            break;
        }

    if (winterIndex != -1) {
        // 月份计算：冬至月为11月
        month = (11 + monthIndex - winterIndex) % 12;
        if (month <= 0) month += 12;

        // 闰月处理
        if (leapMonth != -1) {
            if (monthIndex == leapMonth)
                isLeapMonth = true;
            else if (monthIndex > leapMonth)
                month--;
        }
    } else
        // 备选方案
        month = (monthIndex + 10) % 12 + 1;

    return {lunarYear, month, day, isLeapMonth};
}
