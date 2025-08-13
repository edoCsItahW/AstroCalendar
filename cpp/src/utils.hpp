// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file utils.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/13 00:33
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UTILS_HPP
#define UTILS_HPP
#pragma once

#include <stdexcept>
#include <format>

namespace astro {
    template<typename T, typename R>
        requires std::is_arithmetic_v<T>
    R binPow(T base, int exp) {
        T result = 1;

        while (exp > 0) {
            if (exp & 1) result = result * base;

            base = base * base;
            exp  = exp >> 1;
        }

        return result;
    }

    template<typename Func, typename A, typename R>
        requires arithmeticFunc<Func, A, R>
    double brent(const Func& func, A a, A b, double tol, int maxIter) {
        auto xa = static_cast<double>(a);
        auto xb = static_cast<double>(b);

        // 初始检查
        double fa = func(xa);
        double fb = func(xb);

        if (fa * fb > 0) throw std::invalid_argument("brent: root must be bracketed");

        if (std::abs(fa) < std::abs(fb)) {
            std::swap(xa, xb);
            std::swap(fa, fb);
        }

        double xc   = xa;       // 前一个迭代点
        double fc   = fa;       // 前一个函数值
        bool bisect = true;     // 上一步是否使用二分法
        double d    = xb - xa;  // 初始步长

        for (int i = 0; i < maxIter; ++i) {
            // 收敛检查（增强版）
            if (std::abs(fb) < tol || std::abs(xb - xa) < tol) return xb;

            double s = 0.0;

            // 1. 尝试逆二次插值（标准公式）
            if (fa != fc && fb != fc) {
                double L0 = (xa * fb * fc) / ((fa - fb) * (fa - fc));
                double L1 = (xb * fa * fc) / ((fb - fa) * (fb - fc));
                double L2 = (xc * fa * fb) / ((fc - fa) * (fc - fb));
                s         = L0 + L1 + L2;
            }
            // 2. 尝试割线法（带零分母保护）
            else {
                if (std::abs(fb - fa) < 1e-12)
                    s = (xa + xb) / 2;
                else
                    s = xb - fb * (xb - xa) / (fb - fa);
            }

            // 3. 检查插值结果有效性（标准Brent条件）
            double minBound = std::min(xa, xb);
            double maxBound = std::max(xa, xb);
            double midpoint = (xa + xb) / 2;

            bool condition1 = (s < minBound) || (s > maxBound);
            bool condition2 = bisect && (std::abs(s - xb) >= std::abs(xb - xc) / 2);
            bool condition3 = !bisect && (std::abs(s - xb) >= std::abs(xc - d) / 2);
            bool condition4 = bisect && (std::abs(xb - xc) < tol);
            bool condition5 = !bisect && (std::abs(xc - d) < tol);

            if (condition1 || condition2 || condition3 || condition4 || condition5) {
                s      = midpoint;
                bisect = true;
            } else {
                bisect = false;
            }

            // 计算新点函数值
            double fs = func(s);
            d         = xc;  // 保存上上步的点
            xc        = xb;  // 保存上步的点

            // 更新区间
            if (fa * fs < 0) {
                xb = s;
                fb = fs;
            } else {
                xa = s;
                fa = fs;
            }

            // 确保b是最优估计点
            if (std::abs(fa) < std::abs(fb)) {
                std::swap(xa, xb);
                std::swap(fa, fb);
            }
        }

        throw std::runtime_error("Brent's method did not converge");
    }

    template<typename Func, typename A, typename R>
        requires arithmeticFunc<Func, A, R>
    double findRootForward(const Func& func, A x, double step, double tol) {
        double a = x;
        double b = x + step;

        while (func(a) * func(b) > 0) b += step;

        return brent(func, a, b, tol, 1000);
    }

    template<typename Func, typename A, typename R>
        requires arithmeticFunc<Func, A, R>
    double findRootBackward(const Func& func, A x, double step, double tol) {
        double a = x;
        double b = x - step;

        while (func(a) * func(b) > 0) b -= step;

        return brent(func, a, b, tol, 1000);
    }

    template<typename T>
    void rangeCheck(T x, T a, T b) {
        if (x < a || x > b) throw std::out_of_range(std::format("{} is out of range [{}, {}]", x, a, b));
    }
}  // namespace astro

#endif  // UTILS_HPP
