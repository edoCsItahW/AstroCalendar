// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for
// commercial purposes is prohibited without the author's permission. If you have any questions
// or require permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file main.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/23 17:17
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */

// #include "src/type.h"

#include "src/constant.h"
#include <iostream>

int main() {
    using namespace astro;

    JulianDay jd{1000.0, UTC};

    auto v = convert(jd, TT);

    std::cout << v.value() << std::endl;

    return 0;
}
