// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file units.hpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/01 14:14
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef UNITS_HPP
#define UNITS_HPP
#pragma once

namespace astro {
    template<char... Cs>
    constexpr Val<Degree, str_type_t<Cs...>> operator""_aD() {
        using T = str_type<Cs...>;
        return Val<Degree, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Radian, str_type_t<Cs...>> operator""_aR() {
        using T = str_type<Cs...>;
        return Val<Radian, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Arcminute, str_type_t<Cs...>> operator""_aAM() {
        using T = str_type<Cs...>;
        return Val<Arcminute, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Arcsecond, str_type_t<Cs...>> operator""_aAS() {
        using T = str_type<Cs...>;
        return Val<Arcsecond, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Meter, str_type_t<Cs...>> operator""_dM() {
        using T = str_type<Cs...>;
        return Val<Meter, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Kilometer, str_type_t<Cs...>> operator""_dK() {
        using T = str_type<Cs...>;
        return Val<Kilometer, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<AU, str_type_t<Cs...>> operator""_dAU() {
        using T = str_type<Cs...>;
        return Val<AU, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Year, str_type_t<Cs...>> operator""_tY() {
        using T = str_type<Cs...>;
        return Val<Year, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Day, str_type_t<Cs...>> operator""_tD() {
        using T = str_type<Cs...>;
        return Val<Day, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Second, str_type_t<Cs...>> operator""_tS() {
        using T = str_type<Cs...>;
        return Val<Second, typename T::type>(T::value);
    }

    template<char... Cs>
    constexpr Val<Century, str_type_t<Cs...>> operator""_tC() {
        using T = str_type<Cs...>;
        return Val<Century, typename T::type>(T::value);
    }
}  // namespace astro

#endif  // UNITS_HPP
