#! \user\bin\python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: main.py
# author: edocsitahw
# data: 2025\8\8 23:42
# desc:
# -------------------------<edocsitahw>----------------------------

from core.lea import lea406
from core.vsop import vsop2013
from core.constant import JulianDay
from datetime import datetime
from python.dataReader.dataReader import *
from os import path


def readAllData(**kwargs: str) -> dict:
    result = {}

    for name, filename in kwargs.items():
        if path.exists(filename):
            with open(filename, 'r') as f:
                print(f"Reading {filename}...")
                result[name] = parse(f.read())

    return result


if __name__ == "__main__":
    date = datetime(2025, 8, 8)

    jd = JulianDay.fromDatetime(date)

    data = readAllData(
        vsop=r"E:\code\astroCalendar\data\VSOP2013\VSOP2013p3.dat",
        r=r"E:\code\astroCalendar\data\LEA-406\table9.dat",
        v=r"E:\code\astroCalendar\data\LEA-406\table10.dat",
        t=r"E:\code\astroCalendar\data\LEA-406\table11.dat"
    )

    vsopData = data['vsop']

    sunR, sunV, sunU = vsop2013(jd, vsopData)

    print(sunR, sunV, sunU)

    leaRData, leaVData, leaUData = data['r'], data['v'], data['t']

    leaR, leaV, leaU = lea406(jd, rData=leaRData, vData=leaVData, tData=leaUData)

    print(leaR, leaV, leaU)


