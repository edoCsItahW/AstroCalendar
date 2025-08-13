#! /user/bin/python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: readVSOPData
# author: edocsitahw
# data: 2025/7/26 16:21
# desc:
# -------------------------<edocsitahw>----------------------------

from dataReader import *


def expressionValue(expr: Expression) -> str | float | int:
    if isinstance(expr, Identifier):
        return expr.name

    elif isinstance(expr, Variable):
        return f"{expr.name}-{expr.flag}"

    elif isinstance(expr, Literal):
        return expr.value

    raise TypeError("Invalid expression type")


def readData(filePath: str) -> None:
    with open(filePath, "r") as f:
        tokens = Lexer.tokenize(f.read())

        print("词法分析完毕")

        data = Parser(tokens).parse()

        print("语法分析完毕")

        for table in data.tables:
            if expressionValue(table.header.fields[1]) == 3 and expressionValue(table.header.fields[2]) == 2:
                for term in table.terms:
                    print([term.id, term.coefficients, term.sinMantissa, term.cosMantissa, term.sinExponent,
                           term.cosExponent])


if __name__ == '__main__':
    readData(r"..\..\data\VSOP2013\VSOP2013p3.dat")
