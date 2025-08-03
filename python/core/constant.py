#! /user/bin/python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: constant.py
# author: edocsitahw
# data: 2025/8/3 14:20
# desc:
# -------------------------<edocsitahw>----------------------------


__all__ = ['TimeScale', 'JulianDay', 'J2000', 'leapSeconds']


from enum import Enum
from typing import Self, overload
from fractions import Fraction
from datetime import datetime, timezone
from math import inf, radians, cos, sin


class TimeScale(Enum):
    UTC = 0
    TAI = 1
    TT = 2
    TDB = 3


LEAP_SECONDS_TABLE = [
    (-500, lambda y: (y - 1280) / 100, lambda u: -20 + 32 * u ** 2),
    (500, lambda y: y / 100, lambda
        u: 10583.6 - 1014.41 * u + 33.78311 * u ** 2 - 5.952053 * u ** 3 - 0.1798452 * u ** 4 + 0.022174192 * u ** 5 + 0.0090316521 * u ** 6),
    (1600, lambda y: (y - 1000) / 100, lambda
        u: 1574.2 - 556.01 * u + 71.23472 * u ** 2 + 0.319781 * u ** 3 - 0.8503463 * u ** 4 - 0.005050998 * u ** 5 + 0.0083572073 * u ** 6),
    (1700, lambda y: y - 1600, lambda u: 120 - 0.9808 * u - 0.01532 * u ** 2 + u ** 3 / 7129.0),
    (1800, lambda y: y - 1700,
     lambda u: 8.83 + 0.1603 * u - 0.0059285 * u ** 2 + 0.00013336 * u ** 3 - u ** 4 / 1174000.0),
    (1860, lambda y: y - 1800, lambda
        u: 13.72 - 0.332447 * u + 0.0068612 * u ** 2 + 0.0041116 * u ** 3 - 0.00037436 * u ** 4 + 0.0000121272 * u ** 5 - 0.0000001699 * u ** 6
           + 0.000000000875 * u ** 7),
    (1900, lambda y: y - 1860,
     lambda u: 7.62 + 0.5737 * u - 0.251754 * u ** 2 + 0.01680668 * u ** 3 - 0.0004473624 * u ** 4 + u ** 5 / 233174.0),
    (1920, lambda y: y - 1900,
     lambda u: -2.79 + 1.494119 * u - 0.0598939 * u ** 2 + 0.0061966 * u ** 3 - 0.000197 * u ** 4),
    (1941, lambda y: y - 1920, lambda u: 21.20 + 0.84493 * u - 0.076100 * u ** 2 + 0.0020936 * u ** 3),
    (1961, lambda y: y - 1950, lambda u: 29.07 + 0.407 * u - u ** 2 / 233.0 + u ** 3 / 2547.0),
    (1986, lambda y: y - 1975, lambda u: 45.45 + 1.067 * u - u ** 2 / 260.0 - u ** 3 / 718.0),
    (2005, lambda y: y - 2000, lambda
        u: 63.86 + 0.3345 * u - 0.060374 * u ** 2 + 0.0017275 * u ** 3 + 0.000651814 * u ** 4 + 0.00002373599 * u ** 5),
    (2050, lambda y: y - 2000, lambda u: 62.92 + 0.32217 * u + 0.005589 * u ** 2),
    (2150, lambda y: -20 + 32 * ((y - 1820) / 100) ** 2 - 0.5628 * (2150 - y)),
    (inf, lambda y: (y - 1820) / 100, lambda u: -20 + 32 * u ** 2)
]


def convert[T: float | int | Fraction](value: T, fromScale: TimeScale, toScale: TimeScale) -> T:
    return globals()[f"to{toScale.name.upper()}"](value, fromScale)


class JulianDay:
    def __init__(self, value: float | int | Fraction, scale: TimeScale = TimeScale.UTC):
        self.value = value
        self._scale = scale

    @property
    def scale(self) -> TimeScale:
        return self._scale

    @scale.setter
    def scale(self, value: TimeScale):
        self.convert(value)

    def convert(self, target: TimeScale) -> Self:
        if self._scale != target:
            self.value = convert(self.value, self._scale, target)
            self._scale = target

        return self

    def toDatetime(self) -> datetime:
        Z = int(self.value)
        F = self.value - Z

        A = Z if Z < 2299161 else int((Z - 1867216.25) / 36524.25)
        B = Z + 1 + A - int(A / 4)

        C = B + 1524
        D = int((C - 122.1) / 365.25)
        E = int(365.25 * D)
        G = int((C - E) / 30.6001)

        day = int(C - E + F - int(30.6001 * G))
        month = G - 1 if G < 13.5 else G - 13
        year = D - 4716 if month > 2.5 else D - 4715

        h = F * 24
        hour = int(h)
        m = (h - hour) * 60
        minute = int(m)
        second = int((m - minute) * 60)

        return datetime(year, month, day, hour, minute, second, tzinfo=timezone.utc)


    @staticmethod
    def fromDatetime(dt: datetime) -> 'JulianDay':
        y = dt.year + 1 if dt.year < 0 else dt.year
        m = dt.month
        d = dt.day

        if m <= 2:
            y -= 1
            m += 12

        A = y // 100
        B = 0 if y < 1582 or (y == 1582 and m < 10) or (y == 1582 and m == 10 and d < 15) else 2 - A + A // 4

        JD = int(365.25 * (y + 4716)) + int(30.6001 * (m + 1)) + d + B - 1524.5

        return JulianDay(JD + (dt.hour - 12) / 24 + dt.minute / 1440 + dt.second / 86400, TimeScale.UTC)


J2000 = JulianDay(2451545.0, TimeScale.TT)


def leapSeconds[T: float | int | Fraction](value: T) -> T:
    dt = datetime.fromtimestamp(value, tz=timezone.utc)

    for (start, *funcs) in LEAP_SECONDS_TABLE:
        if dt.year < start:
            res = 0
            for func in funcs:
                res = func(dt.year)
            return res

    raise ValueError(  # 年份超出范围
        f"Year {dt.year} is out of range")

def calcG[T: float | int | Fraction](value: T) -> T:
    century = (value - J2000.value) / 36525.0
    return radians(357.5291092 + 35999.05034 * century)

def diffDeltaT[T: float | int | Fraction](value: T) -> T:
    G = calcG(value)

    gDiff = radians(35999.05034)

    return 0.001658 * cos(G + 0.0167 * sin(G)) * (1 + 0.0167 * cos(G)) * gDiff

def deltaT[T: float | int | Fraction](value: T) -> T:
    G = calcG(value)

    return 0.001658 * sin(G + 0.0167 * sin(G))

def TAI2UTC[T: float | int | Fraction](value: T) -> T:
    return value - leapSeconds(value)


def TT2TAI[T: float | int | Fraction](value: T) -> T:
    return value - 32.184


def UTC2TAI[T: float | int | Fraction](value: T) -> T:
    return value + leapSeconds(value)


def TAI2TT[T: float | int | Fraction](value: T) -> T:
    return value + 32.184


def TDB2TT[T: float | int | Fraction](value: T) -> T:
    prev = value

    while abs((new := prev - deltaT(prev) / diffDeltaT(prev)) - prev) < 1e-12:
        prev = new

    return new

def TT2TDB[T: float | int | Fraction](value: T) -> T:
    return value + deltaT(value)


def toUTC[T: float | int | Fraction](value: T, fromScale: TimeScale = TimeScale.TAI) -> T:
    match fromScale:
        case TimeScale.TAI:
            return TAI2UTC(value)
        case TimeScale.TT:
            return TT2TAI(TAI2UTC(value))
        case TimeScale.TDB:
            return TDB2TT(toUTC(value, TimeScale.TT))
        case _:
            raise ValueError(  # 错误类型
                "Invalid time scale")


def toTAI[T: float | int | Fraction](value: T, fromScale: TimeScale = TimeScale.UTC) -> T:
    match fromScale:
        case TimeScale.UTC:
            return UTC2TAI(value)
        case TimeScale.TT:
            return TT2TAI(value)
        case TimeScale.TDB:
            return TT2TAI(TDB2TT(value))
        case _:
            raise ValueError(  # 错误类型
                "Invalid time scale")


def toTT[T: float | int | Fraction](value: T, fromScale: TimeScale = TimeScale.UTC) -> T:
    match fromScale:
        case TimeScale.UTC:
            return TAI2TT(UTC2TAI(value))
        case TimeScale.TAI:
            return TAI2TT(value)
        case TimeScale.TDB:
            return TDB2TT(value)
        case _:
            raise ValueError(  # 错误类型
                "Invalid time scale")


def toTDB[T: float | int | Fraction](value: T, fromScale: TimeScale = TimeScale.UTC) -> T:
    match fromScale:
        case TimeScale.UTC:
            return TT2TDB(toUTC(value, TimeScale.TT))
        case TimeScale.TAI:
            return TT2TDB(TAI2TT(value))
        case TimeScale.TT:
            return TT2TDB(value)
        case _:
            raise ValueError(  # 错误类型
                "Invalid time scale")


if __name__ == '__main__':
    dt = J2000.toDatetime()
    print(dt)
