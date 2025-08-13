#! /user/bin/python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: calculate.py
# author: edocsitahw
# data: 2025/8/3 16:02
# desc:
# -------------------------<edocsitahw>----------------------------

from .constant import JulianDay, TimeScale, binPow
from python.dataReader.dataReader import *
from math import sin, cos, atan2, sqrt, asin, nan, inf

LAMBDA_TABLE = [
    lambda T: 4.402608631669 + 26087.90314068555 * T,
    lambda T: 3.176134461576 + 10213.28554743445 * T,
    lambda T: 1.753470369433 + 6283.075850353215 * T,
    lambda T: 6.203500014141 + 3340.612434145457 * T,
    lambda T: 4.091360003050 + 1731.170452721855 * T,
    lambda T: 1.713740719173 + 1704.450855027201 * T,
    lambda T: 5.598641292287 + 1428.948917844273 * T,
    lambda T: 2.805136360408 + 1364.756513629990 * T,
    lambda T: 2.326989734620 + 1361.923207632842 * T,
    lambda T: 0.599546107035 + 529.6909615623250 * T,
    lambda T: 0.874018510107 + 213.2990861084880 * T,
    lambda T: 5.481225395663 + 74.78165903077800 * T,
    lambda T: 5.311897933164 + 38.13297222612500 * T,
    lambda T: 0.3595362285049309 * T,
    lambda T: 5.198466400630 + 77713.7714481804 * T,
    lambda T: 1.627905136020 + 84334.6615717837 * T,
    lambda T: 2.35555638750 + 83286.9142477147 * T
]

def phi(t: float, data: list[float]) -> float:
    return sum(c * f(t) for c, f in zip(data, LAMBDA_TABLE))

def calcSeries(t: float, data: Term) -> float:
    p = phi(t, data.coefficients)

    return data.sinMantissa * binPow(10, int(data.sinExponent)) * sin(p) + data.cosMantissa * binPow(10, int(data.cosExponent)) * cos(p)

def vsop2013(jd: JulianDay, data: Data) -> tuple[float, float, float]:
    tdb = jd.convert(TimeScale.TDB).value / 365250

    if abs(tdb) > 100:
        raise ValueError(f"时间超出VSOP2013支持范围: t={tdb * 1000:.0f}年")

    varMap = {k: 0 for k in "alkhpq"}

    keyMap = {i: k for i, k in enumerate("alkhpq", start=1)}

    for table in data.tables:
        key = int(table.header.fields[2].value)

        for i, term in enumerate(table.terms, start=1):
            result = binPow(tdb, i) * calcSeries(tdb, term)
            if result == nan or varMap[keyMap[key]] == nan or abs(result) == inf:
                raise ValueError(f"第{i}项计算结果为nan")
            print(f"结果为{result}, 此时{keyMap[key]}的值为{varMap[keyMap[key]]}")
            varMap[keyMap[key]] += result

    a, l, k, h, p, q = varMap.values()

    e = sqrt(k ** 2 + h ** 2)
    peri = atan2(h, k)
    m = l - peri
    i = 2 * asin(sqrt(q ** 2 + p ** 2))
    o = atan2(p, q)

    eqE = lambda x: x - e * sin(x) - m
    diffEqE = lambda x: 1 - e * cos(x)

    E = m + e * sin(m)
    while abs((E2 := (E - eqE(E) / diffEqE(E))) - E > 1e-12):
        E = E2

    v = 2 * atan2(sqrt(1 + e) * sin(E/2), sqrt(1 - e) * cos(E/2))
    r = a * (1 - e ** 2) / (1 + e * cos(v))

    x, y, z = r * cos(v), r * sin(v), 0

    t = peri - v
    x = r * (cos(o) * cos(t) - sin(o) * sin(t) * cos(i))
    y = r * (sin(o) * cos(t) - cos(o) * sin(t) * cos(i))
    z = r * sin(t) * sin(i)

    x_sun, y_sun, z_sun = -x, -y, -z

    V_sun = atan2(y_sun, x_sun)
    U_sun = asin(z_sun / r)

    return V_sun, U_sun, r
