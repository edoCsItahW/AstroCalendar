#! /user/bin/python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: readLEAData.py
# author: edocsitahw
# data: 2025/8/3 19:35
# desc:
# -------------------------<edocsitahw>----------------------------
from dataReader import *


def readData(filePath: str) -> None:
    with open(filePath, "r") as f:
        tokens = Lexer.tokenize(f.read())

        for i, token in enumerate(tokens):
            if 350 <= i <= 375:
                print(i, ": ", token)

        print("词法分析完毕")

        data = Parser(tokens).parse()

        print("语法分析完毕")

        for term in data.terms:
            print([term.id, term.coefficients, term.amplitudes, term.phases])


if __name__ == '__main__':
    readData(r"E:\code\astroCalendar\data\LEA-406\table9.dat")
