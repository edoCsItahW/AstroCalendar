#! /user/bin/python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: lea.py
# author: edocsitahw
# data: 2025/8/3 21:54
# desc:
# -------------------------<edocsitahw>----------------------------

from .constant import JulianDay, TimeScale, binPow
from python.dataReader.dataReader import *
from functools import cache
from math import cos, sin

ARGS_TABLE = [
    lambda t: 125.04455501 - (696791.937631 * t + 636.02 * t ** 2 + 7.625 * t ** 3 - 0.3586 * t ** 4) / 3600,
    lambda t: 297.85019547 + (16029616012.090 * t - 637.06 * t ** 2 + 6.593 * t ** 3 - 0.3169 * t ** 4) / 3600,
    lambda t: 357.52910918 + (1295965810.481 * t - 55.32 * t ** 2 + 0.136 * t ** 3 - 0.1149 * t ** 4) / 3600,
    lambda t: 134.96340251 + (17179159232.178 * t + 3187.92 * t ** 2 + 51.635 * t ** 3 - 2.4470 * t ** 4) / 3600,
    lambda t: 93.27209062 + (17395272628.478 * t - 1275.12 * t ** 2 - 1.037 * t ** 3 + 0.0417 * t ** 4) / 3600,
    lambda t: 252.25090552 + (5381016286.88982 * t - 1.92789 * t ** 2 + 0.00639 * t ** 3) / 3600,
    lambda t: 181.97980085 + (2106641364.33548 * t + 0.59381 * t ** 2 - 0.00627 * t ** 3) / 3600,
    lambda t: 100.46645683 + 1295977422.83429 * t - 2.04411 * t ** 2 - 0.00523 * t ** 3,
    lambda t: 355.43299958 + 689050774.93988 * t + 0.94264 * t ** 2 - 0.01043 * t ** 3,
    lambda t: 34.35151874 + 109256603.77991 * t - 30.60378 * t ** 2 + 0.05706 * t ** 3 + 0.04667 * t ** 4,
    lambda t: 50.07744430 + 43996098.55732 * t + 75.61614 * t ** 2 - 0.16618 * t ** 3 - 0.11484 * t ** 4,
    lambda t: 314.05500511 + 15424811.93933 * t - 1.75083 * t ** 2 + 0.02156 * t ** 3,
    lambda t: 304.34866548 + 78655032.20744 * t + 0.21103 * t ** 2 - 0.00895 * t ** 3,
    lambda t: (50288.2 * t + 111.2022 * t ** 2 + 0.0773 * t ** 3 - 0.2353 * t ** 4) / 3600
]


def meanLongitude(t: float) -> float:
    return 218.31664563 + (173256437.2370470 * t - 527.90 * t ** 2 + 6.6655 * t ** 3 - 0.5522 * t ** 4) / 3600


@cache
def calcOmega(t: float, data: tuple[int | float]) -> float:
    return sum(n * func(t) for n, func in zip(data, ARGS_TABLE))


def calcR(t: float, data: Data) -> float:
    return sum(
        sum(term.amplitudes[i] * binPow(t, i) * cos(calcOmega(t, tuple(term.coefficients)) + term.phases[i]) for i in range(3))
        for term in data.terms)


def calcV(t: float, data: Data) -> float:
    return meanLongitude(t) + sum(
        sum(term.amplitudes[i] * binPow(t, i) * sin(calcOmega(t, tuple(term.coefficients)) + term.phases[i]) for i in range(3))
        for term in data.terms)


def calcU(t: float, data: Data) -> float:
    return sum(
        sum(term.amplitudes[i] * binPow(t, i) * sin(calcOmega(t, tuple(term.coefficients)) + term.phases[i]) for i in range(3))
        for term in data.terms)


def lea406(jd: JulianDay, *, rData: Data, vData: Data, tData: Data) -> tuple[float, float, float]:
    tdb = jd.convert(TimeScale.TDB).value / 365250

    r = calcR(tdb, rData)
    v = calcV(tdb, vData) / 3600
    u = calcU(tdb, tData) / 3600

    return r, v, u
