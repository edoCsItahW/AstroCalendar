// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/10 16:37
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UTILS_H
#define UTILS_H
#pragma once

#include <type_traits>
#include <concepts>

namespace astro {
    template<typename F, typename A, typename R>
    concept arithmeticFunc = requires(F f, A a, R r) {
        requires std::is_arithmetic_v<A> || std::convertible_to<A, double>;
        requires std::is_arithmetic_v<R> || std::convertible_to<R, double>;
        { f(a) } -> std::same_as<R>;
    };

    template<typename T, typename R = T>
        requires std::is_arithmetic_v<T>
    R binPow(T base, int exp);

    template<typename Func, typename A, typename R = std::invoke_result_t<Func, A>>
        requires arithmeticFunc<Func, A, R>
    double brent(const Func& func, A a, A b, double tol = 1e-6, int maxIter = 1000);

    template<typename Func, typename A, typename R = std::invoke_result_t<Func, A>>
        requires arithmeticFunc<Func, A, R>
    double findRootForward(const Func& func, A x, double step, double tol = 1e-6);

    template<typename Func, typename A, typename R = std::invoke_result_t<Func, A>>
        requires arithmeticFunc<Func, A, R>
    double findRootBackward(const Func& func, A x, double step, double tol = 1e-6);

    template<typename T>
    void rangeCheck(T x, T a, T b);
}  // namespace astro

#include "utils.hpp"

#endif  // UTILS_H
