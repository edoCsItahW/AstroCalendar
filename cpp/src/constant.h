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

#include "date.h"

namespace astro {

    extern const JulianDay J2000;

    namespace vsop2013 {
        /**
         * @if zh
         * @brief 计算水星的
         *
         * @tparam T
         * @param t
         * @return
         *
         * @endif
         */
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

    }  // namespace vsop2013

}  // namespace astro

#include "constant.hpp"

#endif  // CONSTANT_H
