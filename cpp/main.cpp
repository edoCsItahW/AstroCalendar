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
//     std::cout << "����julianDay:\n\t��������: " << jd << std::endl;
// }
//
// void test_gregorianDay() {
//     DateTime datetime = {2025, 3, 20, 17, 1, 14};  // 2025����
//
//     auto jd = julianDay(datetime);
//
//     auto gd = gregorianDay(jd);
//
//     std::cout << "����gregorianDay:\n\t�����������������: " << gd.year << "��" << gd.month << "��" << gd.day << "��" << std::endl;
// }
//
// void test_solarLongitude() {
//     constexpr DateTime datetime = {2025, 3, 20, 17, 1, 13};  // 2025����
//
//     auto jd = julianDay(datetime);
//
//     auto sl = solarLongitude(jd);
//
//     std::cout << "����solarLongitude:\n\t̫���ƾ����Ƕȣ�: " << sl << std::endl;
// }
//
// void test_lunarLongitude() {
//     DateTime datetime = {2025, 8, 4, 16, 18};  // 2025�����������
//
//     auto jd = julianDay(datetime);
//
//     auto ll = lunarLongitude(jd);
//
//     std::cout << "����lunarLongitude:\n\t����ƾ����Ƕȣ�: " << ll << std::endl;
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
//     std::cout << "����calculateNewMoon:\n\t��" << k << "���µ�����ʱ��: " << dt.year << "��" << dt.month << "��" << dt.day << "��" << dt.hour << "ʱ" << dt.minute << "��" << dt.second << "��" << std::endl;
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
//     std::cout << "����findNewMoonNear:\n\t���������ʱ��: " << dt.year << "��" << dt.month << "��" << dt.day << "��" << dt.hour << "ʱ" << dt.minute << "��" << dt.second << "��" << std::endl;
// }
//
// void test_winterSolstice() {
//     // ӦΪ2025��12��21��23ʱ2��48��
//     auto jd = winterSolstice(2025);
//
//     auto dt = gregorianDay(jd);
//
//     std::cout << "����winterSolstice:\n\t����: " << dt.year << "��" << dt.month << "��" << dt.day << "��" << dt.hour << "ʱ" << dt.minute << "��" << dt.second << "��" << std::endl;
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
//     std::cout << "����solarTerm:\n\t��ʮ�Ľ���: " << dt.year << "��" << dt.month << "��" << dt.day << "��" << dt.hour << "ʱ" << dt.minute << "��" << dt.second << "��" << std::endl;
// }
//
// void test_gregorianToLunar() {
//     constexpr DateTime datetime = {2023, 10, 1};
//
//     auto ld = gregorianToLunar(datetime);
//
//     std::cout << "����gregorianToLunar:\n\t����: " << datetime.year << "��" << datetime.month << "��" << datetime.day << "��" << std::endl;
//     std::cout << "\tũ��: " << ld.year << "��" << ld.month << (ld.isLeapMonth? "��" : "") << "��" << ld.day << "��" << std::endl;
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
