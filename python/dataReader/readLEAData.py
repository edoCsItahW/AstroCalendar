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
from leaDataReader import *

def literalValue(literal: Literal) -> float | int | str:
    return literal.value

def readData(filePath: str) -> None:
    with open(filePath, "r") as f:
        tokens = Lexer.tokenize(f.read())

        print("词法分析完毕")

        data = Parser(tokens).parse()

        print("语法分析完毕")

        for term in data.terms:
            print([
                term.id,
                [literalValue(literal) for literal in term.coefficients],
                [literalValue(literal) for literal in term.amplitudes],
                [literalValue(literal) for literal in term.phases]
            ])


if __name__ == '__main__':
    readData(r"..\..\data\LEA-406\table6.dat")
