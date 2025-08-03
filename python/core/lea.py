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

from constant import JulianDay, TimeScale
from value import Value, Degree, Arcminute, Arcsecond

ARGS_TABLE = [
    lambda t: 125.04455501 - (696791.937631 * t + 636.02 * t ** 2 + 7.625 * t ** 3 - 0.3586 * t ** 4) / 3600,
    lambda t: 297.85019547 + 16029616012.090 * t - 637.06 * t ** 2 + 6.593 * t ** 3 - 0.3169 * t ** 4,
    
]

def meanLongitude(t: float) -> float:
    return 218.31664563 + (173256437.2370470 * t - 527.90 * t ** 2 + 6.6655 * t **3 - 0.5522 * t ** 4) / 3600


