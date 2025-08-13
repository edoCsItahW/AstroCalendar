# -*- coding: utf-8 -*-

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

from enum import Enum


class TokenType(Enum):
    """
    令牌类型枚举
    """
    INT = 1
    FLOAT = 2
    IDENTIFIER = 2
    NEWLINE = 3
    MUL = 4
    END = 5


class Token:
    """
    令牌类
    """

    def __init__(self, type: TokenType, value: str) -> None: ...

    @property
    def type(self) -> TokenType: ...

    @property
    def value(self) -> str: ...

    def __str__(self) -> str: ...


class Lexer:
    """
    词法分析器
    """

    def __init__(self, src: str) -> None: ...

    @staticmethod
    def tokenize(src: str) -> list[Token]: ...

    def next(self) -> Token: ...


class Parser:
    """
    语法分析器
    """

    def __init__(self, tokens: list[Token]) -> None: ...

    def parse(self) -> Data: ...


class AST:
    """
    抽象语法树
    """

    @property
    def nodeName(self) -> str: ...

    def toJSON(self) -> str: ...


class Literal(Expression):
    """
    文字节点
    """

    @property
    def value(self) -> int | float: ...


class Expression(AST): ...


class Integer(Literal):
    """
    整数节点
    """

    @property
    def value(self) -> int: ...


class Float(Literal):
    """
    浮点数节点
    """

    @property
    def value(self) -> float: ...


class Identifier(AST):
    """
    标识符节点
    """

    @property
    def name(self) -> str: ...


class Variable(AST):
    """
    变量节点
    """

    @property
    def name(self) -> str: ...

    @property
    def flag(self) -> str: ...


class Term(AST):
    """
    项节点
    """

    @property
    def id(self) -> int:
        """
        项索引

        :return: 项索引
        """
        ...

    @property
    def coefficients(self) -> list[float]:
        """
        当解析VSOP数据时，此属性存在17个元素，解析LEA数据时，此属性存在14个元素

        :return: 系数列表
        """
        ...

    @property
    def amplitudes(self) -> list[float]:
        """
        仅当解析LEA数据时存在值，且仅存在3个元素

        :return: 振幅列表
        """
        ...

    @property
    def phases(self) -> list[float]:
        """
        仅当解析LEA数据时存在值，且仅存在3个元素

        :return: 相位列表
        """
        ...

    @property
    def sinMantissa(self) -> float:
        """
        仅当解析VSOP数据时存在值

        :return: 正弦项的尾数
        """
        ...

    @property
    def cosMantissa(self) -> float:
        """
        仅当解析VSOP数据时存在值

        :return: 余弦项的尾数
        """
        ...

    @property
    def sinExponent(self) -> int:
        """
        仅当解析VSOP数据时存在值

        :return: 正弦项的指数
        """
        ...

    @property
    def cosExponent(self) -> int:
        """
        仅当解析VSOP数据时存在值

        :return: 余弦项的指数
        """
        ...


class Header(AST):
    """
    表头节点

    此节点类型为VSOP数据的表头节点，读取LEA数据时不应使用此节点类型
    """

    @property
    def fields(self) -> list[Expression]:
        """
        字段节点列表，包括

        1. VSOP标识: VSOP2013
        2. 行星索引: int
        3. 变量索引: int
        4. 时间幂次: int
        5. 包含项数: int
        6. 相关名称: str
        7. 未知
        8. 未知
        9. 未知

        :return: 字段节点列表
        """
        ...


class Table(AST):
    """
    表节点

    此节点类型为VSOP数据的表节点，读取LEA数据时不应使用此节点类型
    """

    @property
    def header(self) -> Header:
        """
        表头节点

        :return: 表头节点
        """
        ...

    @property
    def terms(self) -> list[Term]:
        """
        项节点列表

        :return: 项节点列表
        """
        ...


class Data(AST):
    """
    数据节点
    """

    @property
    def tables(self) -> list[Table]:
        """
        此属性仅当解析VSOP数据时才存在值

        :return: 表节点列表
        """
        ...

    @property
    def terms(self) -> list[Term]:
        """
        此属性仅当解析LEA数据时才存在值

        :return: 项节点列表
        """
        ...


def parse(src: str) -> Data:
    """
    解析数据

    :param src: 数据源代码
    :return: 数据节点
    """
    ...
