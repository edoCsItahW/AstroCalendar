// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file main.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/23 17:17
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */

// #include "src/calcuators.h"
#include <iostream>
#include "src/constant.h"


// void test_julianDay() {
//     DateTime datetime = {2025, 3, 20, 17, 1, 14};
//
//     auto jd = julianDay(datetime);
//
//     std::cout << "测试julianDay:\n\t儒略日数: " << jd << std::endl;
// }
//
// void test_gregorianDay() {
//     DateTime datetime = {2025, 3, 20, 17, 1, 14};  // 2025春分
//
//     auto jd = julianDay(datetime);
//
//     auto gd = gregorianDay(jd);
//
//     std::cout << "测试gregorianDay:\n\t格里高利历（公历）: " << gd.year << "年" << gd.month << "月" << gd.day << "日" << std::endl;
// }
//
// void test_solarLongitude() {
//     constexpr DateTime datetime = {2025, 3, 20, 17, 1, 13};  // 2025春分
//
//     auto jd = julianDay(datetime);
//
//     auto sl = solarLongitude(jd);
//
//     std::cout << "测试solarLongitude:\n\t太阳黄经（角度）: " << sl << std::endl;
// }
//
// void test_lunarLongitude() {
//     DateTime datetime = {2025, 8, 4, 16, 18};  // 2025月球过升交点
//
//     auto jd = julianDay(datetime);
//
//     auto ll = lunarLongitude(jd);
//
//     std::cout << "测试lunarLongitude:\n\t月球黄经（角度）: " << ll << std::endl;
// }
//
// void test_calculateNewMoon() {
//     constexpr DateTime datetime = {2025, 7, 18, 4, 11};
//
//     auto jd = julianDay(datetime);
//
//     auto k = floor((jd - J2000 - 1) / SYNODIC_MONTH);
//
//     auto newMoonJD = calculateNewMoon(k);
//
//     auto dt = gregorianDay(newMoonJD);
//
//     std::cout << "测试calculateNewMoon:\n\t第" << k << "个月的新月时刻: " << dt.year << "年" << dt.month << "月" << dt.day << "日" << dt.hour << "时" << dt.minute << "分" << dt.second << "秒" << std::endl;
// }
//
// void test_findNewMoonNear() {
//     constexpr DateTime datetime = {2025, 7, 18, 4, 11};
//
//     auto jd = julianDay(datetime);
//
//     auto nm = findNewMoonNear(jd);
//
//     auto dt = gregorianDay(nm);
//
//     std::cout << "测试findNewMoonNear:\n\t最近的新月时刻: " << dt.year << "年" << dt.month << "月" << dt.day << "日" << dt.hour << "时" << dt.minute << "分" << dt.second << "秒" << std::endl;
// }
//
// void test_winterSolstice() {
//     // 应为2025年12月21日23时2分48秒
//     auto jd = winterSolstice(2025);
//
//     auto dt = gregorianDay(jd);
//
//     std::cout << "测试winterSolstice:\n\t冬至: " << dt.year << "年" << dt.month << "月" << dt.day << "日" << dt.hour << "时" << dt.minute << "分" << dt.second << "秒" << std::endl;
// }
//
// void test_solarTerm() {
//     DateTime datetime = {2025, 3, 5, 16, 7, 2};
//
//     auto jd = julianDay(datetime);
//
//     auto st = solarTerm(jd, 2);
//
//     auto dt = gregorianDay(st);
//
//     std::cout << "测试solarTerm:\n\t二十四节气: " << dt.year << "年" << dt.month << "月" << dt.day << "日" << dt.hour << "时" << dt.minute << "分" << dt.second << "秒" << std::endl;
// }
//
// void test_gregorianToLunar() {
//     constexpr DateTime datetime = {2023, 10, 1};
//
//     auto ld = gregorianToLunar(datetime);
//
//     std::cout << "测试gregorianToLunar:\n\t公历: " << datetime.year << "年" << datetime.month << "月" << datetime.day << "日" << std::endl;
//     std::cout << "\t农历: " << ld.year << "年" << ld.month << (ld.isLeapMonth? "闰" : "") << "月" << ld.day << "日" << std::endl;
// }

int main() {
    // test_julianDay();
    // test_solarLongitude();
    // test_lunarLongitude();
    // test_calculateNewMoon();
    // test_findNewMoonNear();
    // test_winterSolstice();
    // test_solarTerm();
    // test_gregorianToLunar();

    auto r = astro::vsop2013::lambdaMercury(2458850.0);

    return 0;
}
