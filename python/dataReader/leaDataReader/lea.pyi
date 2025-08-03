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
    def value(self) -> str: ...


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
    术语节点
    """

    @property
    def id(self) -> int: ...

    @property
    def coefficients(self) -> list[Literal]: ...

    @property
    def amplitudes(self) -> list[Literal]: ...

    @property
    def phases(self) -> list[Literal]: ...


class Data(AST):
    """
    数据节点
    """

    @property
    def terms(self) -> list[Term]: ...
