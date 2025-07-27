// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/05/29 17:08
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <string_view>

bool isDigit(const char c) { return c >= '0' && c <= '9'; }

bool isLetter(const char c) { return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'; }

bool isHexDigit(const char c) { return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

bool isWhitespace(const char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

bool isIdentifier(const char c) { return isLetter(c) || isDigit(c) || c == '_'; }

std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::ranges::transform(result, result.begin(), tolower);
    return result;
}

bool isUpperCase(const std::string& str) {
    return std::ranges::all_of(str, [](const char c) { return isLetter(c) && std::toupper(c) == c; });
}

bool isLowerCase(const std::string& str) {
    return std::ranges::all_of(str, [](const char c) { return isLetter(c) && std::tolower(c) == c; });
}

bool isCapitalized(const std::string& str) {
    if (str.empty()) return false;

    return isUpperCase(&str[0]) && std::ranges::all_of(str.substr(1), [](const char c) { return isLowerCase(&c); });
}
