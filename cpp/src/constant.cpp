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
