// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file main.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/11 14:39
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef MAIN_H
#define MAIN_H
#pragma once

#include "src/ast.h"
#include "constant.h"

namespace astro {
    LunarDate gregorianToLunar(const DateTime& gregorianDate, const reader::Data& data, const reader::Data& rData, const reader::Data& vData, const reader::Data& uData);
}

#endif  // MAIN_H
