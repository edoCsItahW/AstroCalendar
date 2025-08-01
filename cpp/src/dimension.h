// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for
// commercial purposes is prohibited without the author's permission. If you have any questions
// or require permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file dimension.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 12:32
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef DIMENSION_H
#define DIMENSION_H
#pragma once

#include <ratio>
#include <type_traits>

namespace astro {

    /** @struct Dimension
     *
     * @brief 量纲模板类
     *
     * @tparam D 量纲
     * @tparam E 整数次幂
     */
    template<typename D, int E>
    struct Dimension {
        using dimension               = D;
        static constexpr int exponent = E;
    };

    /** @struct DimensionList
     *
     * @brief 量纲列表模板类（表示复合量纲）
     *
     * @tparam Ps 量纲列表
     */
    template<typename... Ps>
    struct DimensionList {};

    /** @typedef Dimensionless
     *
     * @brief 空量纲（无量纲量）
     */
    using Dimensionless = DimensionList<>;

    /** @struct Unit
     *
     * @brief 单位模板类
     *
     * @tparam L 量纲列表
     * @tparam R 比例因子
     */
    template<typename L, typename R>
    struct Unit {
        using dimension = L;
        using ratio     = R;
    };

    /** @struct remove_dimension
     *
     * @brief 从量纲列表中移除指定量纲
     *
     * @tparam L 量纲列表
     * @tparam D 要移除的量纲
     */
    template<typename L, typename D>
    struct remove_dimension;

    /** @struct remove_dimension
     *
     * @brief 从量纲列表中移除指定量纲（递归终止条件，空列表）
     *
     * @tparam D 要移除的量纲
     */
    template<typename D>
    struct remove_dimension<Dimensionless, D> {
        using type = Dimensionless;
    };

    /** @struct remove_dimension
     *
     * @brief 从量纲列表中移除指定量纲（递归）
     *
     * @tparam D 要移除的量纲
     * @tparam E 整数次幂
     * @tparam Ps 量纲列表
     */
    template<typename D, int E, typename... Ps>
    struct remove_dimension<DimensionList<Dimension<D, E>, Ps...>, D> {
        using type = DimensionList<Ps...>;
    };

    /** @struct remove_dimension
     *
     * @brief 从量纲列表中移除指定量纲（递归）
     *
     * @tparam O 其他量纲
     * @tparam E 整数次幂
     * @tparam Ps 量纲列表
     * @tparam D 要移除的量纲
     */
    template<typename O, int E, typename... Ps, typename D>
    struct remove_dimension<DimensionList<Dimension<O, E>, Ps...>, D> {
        using type = typename remove_dimension<DimensionList<Ps...>, D>::type;
    };

    /** @struct merge_dimensions
     *
     * @brief 合并两个量纲列表
     *
     * @tparam L1 量纲列表1
     * @tparam L2 量纲列表2
     * @tparam R 结果
     */
    template<typename L1, typename L2, typename R>
    struct merge_dimensions_impl;

    /** @struct merge_dimensions
     *
     * @brief 合并两个量纲列表（终止条件，空列表）
     *
     * @tparam R 结果
     */
    template<typename R>
    struct merge_dimensions_impl<Dimensionless, Dimensionless, R> {
        using type = R;
    };

    template<typename... Rs>
    struct merge_dimensions_impl<Dimensionless, Dimensionless, DimensionList<Rs...>> {
        using type = DimensionList<Rs...>;
    };

    /** @struct merge_dimensions
     *
     * @brief 合并两个量纲列表（第一个列表为空，第二个列表不为空）
     *
     * @tparam D 待处理的量纲
     * @tparam E 整数次幂
     * @tparam Re 剩余列表
     * @tparam Rs 结果列表
     */
    template<typename D, int E, typename... Re, typename... Rs>
    struct merge_dimensions_impl<Dimensionless, DimensionList<Dimension<D, E>, Re...>, DimensionList<Rs...>> {
        using next_result = std::conditional_t<E != 0, DimensionList<Rs..., Dimension<D, E>>, DimensionList<Rs...>>;
        using type        = typename merge_dimensions_impl<Dimensionless, DimensionList<Re...>, next_result>::type;
    };

    /** @struct merge_dimensions
     *
     * @brief 合并两个量纲列表（第二个列表为空，第一个列表不为空）
     *
     * @tparam D 待处理的量纲
     * @tparam E 整数次幂
     * @tparam Re 剩余列表
     * @tparam Rs 结果列表
     */
    template<typename D, int E, typename... Re, typename... Rs>
    struct merge_dimensions_impl<DimensionList<Dimension<D, E>, Re...>, Dimensionless, DimensionList<Rs...>> {
        using type = typename merge_dimensions_impl<DimensionList<Re...>, Dimensionless, DimensionList<Rs..., Dimension<D, E>>>::type;
    };

    /** @struct merge_dimensions
     *
     * @brief 合并两个量纲列表（递归，处理第一个量纲）
     *
     * @tparam D 量纲
     * @tparam E1 整数次幂
     * @tparam Re 剩余列表
     * @tparam L 量纲列表
     * @tparam Rs 结果列表
     */
    template<typename D, int E1, typename... Re, typename L, typename... Rs>
    struct merge_dimensions_impl<DimensionList<Dimension<D, E1>, Re...>, L, DimensionList<Rs...>> {
        struct find_same_dimension {
            template<typename, typename = void>
            struct impl {
                static constexpr bool found = false;
                using type                  = Dimension<D, E1>;
            };

            template<typename O, int E2, typename... Ps>
            struct impl<DimensionList<Dimension<O, E2>, Ps...>, std::enable_if_t<std::is_same_v<D, O>>> {
                static constexpr bool found            = true;
                static constexpr int combined_exponent = E1 + E2;
                using type                             = std::conditional_t<combined_exponent != 0, Dimension<D, combined_exponent>, void>;
            };

            template<typename O, int E2, typename... Ps>
            struct impl<DimensionList<Dimension<O, E2>, Ps...>, std::enable_if_t<!std::is_same_v<D, O>>> : impl<DimensionList<Ps...>> {};

            static constexpr bool found = impl<L>::found;
            using type                  = typename impl<L>::type;
        };

        using found_type = find_same_dimension;
        using new_power  = typename found_type::type;

        using list2 = std::conditional_t<found_type::found, typename remove_dimension<L, D>::type, L>;

        using next_result = std::conditional_t<found_type::found && !std::is_void_v<new_power>, DimensionList<Rs..., new_power>, DimensionList<Rs...>>;

        using type = typename merge_dimensions_impl<DimensionList<Re...>, list2, next_result>::type;
    };

    /** @struct merge_dimensions
     *
     * @brief 合并两个量纲列表
     *
     * @tparam L1 量纲列表1
     * @tparam L2 量纲列表2
     */
    template<typename L1, typename L2>
    struct merge_dimensions {
        using type = typename merge_dimensions_impl<L1, L2, Dimensionless>::type;
    };

    /** @typedef merge_dimensions_t
     *
     * @brief 合并两个量纲列表
     *
     * @tparam L1 量纲列表1
     * @tparam L2 量纲列表2
     */
    template<typename L1, typename L2>
    using merge_dimensions_t = typename merge_dimensions<L1, L2>::type;

    /** @struct dimension_power_impl
     *
     * @brief 实现量纲幂次计算的辅助模板
     */
    template<int N, typename T>
    struct dimension_power_impl;

    // 无量纲的情况
    template<int N>
    struct dimension_power_impl<N, Dimensionless> {
        using type = Dimensionless;
    };

    // 处理 DimensionList 的情况
    template<int N, typename... Ps>
    struct dimension_power_impl<N, DimensionList<Ps...>> {
        using type = DimensionList<Dimension<typename Ps::dimension, Ps::exponent * N>...>;
    };

    // 处理单个 Dimension 的情况
    template<int N, typename Dim, int E>
    struct dimension_power_impl<N, Dimension<Dim, E>> {
        using type = Dimension<Dim, E * N>;
    };

    /** @struct dimension_power
     *
     * @brief 计算指定量纲的整数次幂
     */
    template<int N, typename D>
    struct dimension_power {
        using type = typename dimension_power_impl<N, D>::type;
    };

    // 零次幂返回无量纲
    template<typename D>
    struct dimension_power<0, D> {
        using type = Dimensionless;
    };

    /** @typedef dimension_power_t
     *
     * @brief 计算指定量纲的整数次幂
     *
     * @tparam N 整数次幂
     * @tparam D 量纲
     */
    template<int N, typename D>
    using dimension_power_t = typename dimension_power<N, D>::type;

    /** @struct ratio_power
     *
     * @brief 计算比例因子的整数次幂（递归）
     *
     * @tparam R
     * @tparam N
     */
    template<typename R, int N>
    struct ratio_power {
    private:
        using half      = ratio_power<R, N / 2>;
        using half_sq   = std::ratio_multiply<typename half::type, typename half::type>;
        using remainder = ratio_power<R, N % 2>;

    public:
        using type = std::ratio_multiply<half_sq, typename remainder::type>;
    };

    /** @struct ratio_power
     *
     * @brief 计算比例因子的整数次幂（递归终止条件）
     *
     * @tparam R 比例因子
     */
    template<typename R>
    struct ratio_power<R, 0> {
        using type = std::ratio<1>;
    };

    /** @struct ratio_power
     *
     * @brief 计算比例因子的整数次幂（递归）
     *
     * @tparam R 比例因子
     * */
    template<typename R>
    struct ratio_power<R, 1> {
        using type = R;
    };

    /** @struct ratio_power
     *
     * @brief 计算比例因子的整数次幂（递归）
     *
     * @tparam R 比例因子
     * */
    template<typename R>
    struct ratio_power<R, -1> {
        using type = std::ratio_divide<std::ratio<1>, R>;
    };

    /** @typedef ratio_power_t
     *
     * @brief 计算比例因子的整数次幂
     *
     * @tparam R 比例因子
     * @tparam N 整数次幂
     */
    template<typename R, int N>
    using ratio_power_t = typename ratio_power<R, N>::type;

}  // namespace astro

#endif  // DIMENSION_H
