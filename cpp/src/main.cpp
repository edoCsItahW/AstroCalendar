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
 * @date 2025/08/11 14:39
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "main.h"
#include "calender.h"
#include "constant.h"
#include <memory>
#include <ranges>

namespace astro {
    LunarDate gregorianToLunar(const DateTime& gregorianDate, const reader::Data& data, const reader::Data& rData, const reader::Data& vData, const reader::Data& uData) {
        auto jd = gregorianDate.toJulianDay();

        auto tdb_jd_C = julianCentury(jd, TDB);

        const auto solarAppCoord = [&](double t) { return solarApparentCoordinate(t, data); };

        const auto moonAppCoord = [&](double t) { return moonApparentCoordinate(t, data, rData, vData, uData); };

        // 一些农历重要时刻

        // 可能是去年冬至也可能是当年冬至
        const auto leftWinterSolstice = findSolarTermBackward(tdb_jd_C, Term::WinterSolstice, solarAppCoord);

        // 如果leftWinterSolstice是当年的冬至，则该变量是明年冬至，反之
        const auto rightWinterSolstice = findSolarTermForward(leftWinterSolstice, Term::WinterSolstice, solarAppCoord);

        // 从leftWinterSolstice向后找到4个节气，包括十二月和可能的闰十二月
        std::array<std::pair<Term, double>, 24> terms;

        double currTerm = leftWinterSolstice;
        for (std::size_t i = 21; i < 26; ++i) {
            auto term     = TERM_TABLE[i % 24];
            terms[i % 24] = {term, currTerm};  // 先赋值后迭代，因为冬至也存储
            currTerm      = findSolarTermForward(currTerm, term, solarAppCoord);
        }

        const int mouthCount = (rightWinterSolstice - leftWinterSolstice) / MEAN_LUNAR_MONTH;

        // 冬至所在月首，即十一月
        const auto firstNovember = findPrevNewMoon(leftWinterSolstice, solarAppCoord, moonAppCoord);

        // 十二月
        const auto firstDecember = findNextNewMoon(firstNovember, solarAppCoord, moonAppCoord);

        // 正月，也可能是闰十二月
        auto firstJanuary = findNextNewMoon(firstDecember, solarAppCoord, moonAppCoord);

        // 是否已经置闰
        bool hasLeap = false;

        const std::shared_ptr<double> meybeLeapDecember = nullptr;

        if (mouthCount >= 13 &&                                                    // 13个月则需要需要置闰
            (firstDecember > terms[23].second || terms[23].second > firstJanuary)  // 中气大寒不在十二月，则存在闰十二月
        ) {
            *meybeLeapDecember = firstJanuary;
            firstJanuary       = findNextNewMoon(firstJanuary, solarAppCoord, moonAppCoord);
            hasLeap            = true;
        }

        std::array<std::pair<double, bool>, 13> months;

        if (firstJanuary < tdb_jd_C) {  // leftWinterSolstice是去年的冬至，firstJanuary是当年的正月
            // 开始生成节气表

            // 如果不存在闰12月，那么terms中1,2两个节气就是当年的
            auto currSyzygy = hasLeap ? terms[23].second : terms[1].second;
            for (std::size_t i = hasLeap ? 0 : 2; i < 24; ++i) {
                currSyzygy = findSolarTermForward(currSyzygy, TERM_TABLE[i], solarAppCoord);  // 先迭代再赋值，因为第一个值不存储
                terms[i]   = {TERM_TABLE[i], currSyzygy};
            }

            bool nextHasLeap = false;

            if (!hasLeap) {  // 没有置闰，判断当年冬至到年尾是否可闰
                auto nextYearWinterSolstice = findSolarTermForward(rightWinterSolstice, Term::WinterSolstice, solarAppCoord);

                auto nextMouthCount = (nextYearWinterSolstice - rightWinterSolstice) / MEAN_LUNAR_MONTH;

                if (nextMouthCount > 13) nextHasLeap = true;
            }

            // 开始生成月表
            auto currMonth = firstJanuary;  // 当年正月
            for (std::size_t i = 0, termIdx{1}; i < 13; ++i) {
                currMonth = findNextNewMoon(currMonth, solarAppCoord, moonAppCoord);

                auto lastTerm = terms[termIdx].second;

                auto nextMonth = findNextNewMoon(currMonth, solarAppCoord, moonAppCoord);

                months[i] = {currMonth, false};  // 先赋值后迭代，因为正月也存储

                // 判断是否为闰月
                if (!hasLeap && (currMonth > lastTerm || lastTerm < nextMonth) && (!nextHasLeap || i < 10)) {  // 十一月后则还要看后半部分可不可闰
                    months[i].second = true;
                    hasLeap          = true;
                }

                termIdx += 2;  // 确保是奇数，取到中气
                currMonth = nextMonth;
            }
        }

        else {  // leftWinterSolstice是当年的冬至，firstJanuary是明年的正月
            // 开始生成节气表

            // terms中21,22,23三个节气就是当年的，如果存在闰12月，则terms中1,2两个节气也是当年的，但可能存在一年双节气的情况，所以不存储，也不影响
            auto currSyzygy = hasLeap ? terms[23].second : terms[1].second;
            for (std::size_t i = hasLeap ? 0 : 2; i < 21; ++i) {
                currSyzygy = findSolarTermForward(currSyzygy, TERM_TABLE[i], solarAppCoord);  // 先迭代再赋值，因为第一个值不存储
                terms[i]   = {TERM_TABLE[i], currSyzygy};
            }

            // 确定去年冬至到当年冬至间是否有闰
            bool prevHasLeap = false;

            if (!hasLeap) {  // 如果当年有闰则前面一年不可能有闰，因为不存在连续两年有闰月
                auto prevYearWinterSolstice = findSolarTermBackward(leftWinterSolstice, Term::WinterSolstice, solarAppCoord);

                const auto prevMouthCount = (prevYearWinterSolstice - leftWinterSolstice) / MEAN_LUNAR_MONTH;

                // 去年大寒
                const auto midTerm = findSolarTermForward(prevYearWinterSolstice, Term::MajorCold, solarAppCoord);

                auto prevNovember = findPrevNewMoon(prevYearWinterSolstice, solarAppCoord, moonAppCoord);

                auto prevDecember = findNextNewMoon(prevNovember, solarAppCoord, moonAppCoord);

                if (prevMouthCount >= 13 && (prevDecember > midTerm || midTerm > prevNovember)) prevHasLeap = true;
            }

            // 开始生成月表，逆向
            auto currMonth = firstNovember;  // 当年冬至所在月首
            for (int i = 11, termIdx = 21; i >= 0; --i) {
                currMonth = findPrevNewMoon(currMonth, solarAppCoord, moonAppCoord);

                auto prevTerm = terms[termIdx].second;

                auto prevMonth = findPrevNewMoon(currMonth, solarAppCoord, moonAppCoord);

                months[i] = {currMonth, false};

                // 判断是否为闰月
                if (!prevHasLeap && (currMonth > prevTerm || prevTerm < prevMonth)) {
                    months[i].second = true;
                    prevHasLeap      = true;
                }

                termIdx -= 2;  // 确保是奇数，取到中气
                currMonth = prevMonth;
            }
        }

        // 计算农历日期
        auto lunarYear = JulianDay(months[0].first, TDB).toDateTime().year;  // 农历年份是正月所在公历年份

        int lunarMonth = months.size() - 1;
        for (int i{}; i < months.size(); ++i)
            if (months[i].first > tdb_jd_C) lunarMonth = i;

        int day = std::floor(jd.value - months[lunarMonth].first) + 1;

        return {lunarYear, lunarMonth + 1, day, gregorianDate.hour, gregorianDate.minute, gregorianDate.second, months[lunarMonth].second};
    }

}  // namespace astro
