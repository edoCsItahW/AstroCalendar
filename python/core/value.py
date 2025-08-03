#! /user/bin/python3

#  Copyright (c) 2025. All rights reserved.
#  This source code is licensed under the CC BY-NC-SA
#  (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
#  This software is protected by copyright law. Reproduction, distribution, or use for commercial
#  purposes is prohibited without the author's permission. If you have any questions or require
#  permission, please contact the author: 2207150234@st.sziit.edu.cn

# -------------------------<edocsitahw>----------------------------
# file: value.py
# author: edocsitahw
# data: 2025/8/2 08:11
# desc:
# -------------------------<edocsitahw>----------------------------
__all__ = [
    "UnitKind",
    "Radin", "Degree", "Arcminute", "Arcsecond",
    "Meter", "Kilometer", "AU",
    "Second", "Minute", "Hour", "Day", "Year",
    "Dimension", "cast",
    "Pair", "Unit",
    "Value"
]

from math import pi
from fractions import Fraction
from numbers import Rational
from enum import Enum
from collections.abc import Sequence
from typing import Optional, Self, overload, Any

class UnitKind(Enum):
    Angle = 1
    Distance = 2
    Time = 3


class _Dimension:
    def __init__(self, kind: UnitKind, ratio: Fraction | int | float = Fraction(1, 1), *, name: Optional[str] = None):
        self._kind = kind
        self._ratio = ratio
        self._name = name

    @property
    def kind(self) -> UnitKind:
        return self._kind

    @property
    def ratio(self) -> Fraction:
        return self._ratio

    @property
    def name(self) -> Optional[str]:
        return self._name

    def __repr__(self) -> str:
        return self.name or 'unknown'


# 单位 = 量纲 * 系数
# 例如：度 = 弧度 * 180/π
Radin = _Dimension(UnitKind.Angle, name="Radin")
Degree = _Dimension(UnitKind.Angle, 180 / pi, name="Degree")
Arcminute = _Dimension(UnitKind.Angle, Fraction(1, 60), name="Arcminute")
Arcsecond = _Dimension(UnitKind.Angle, Fraction(1, 3600), name="Arcsecond")

Meter = _Dimension(UnitKind.Distance, name="Meter")
Kilometer = _Dimension(UnitKind.Distance, Fraction(1000), name="Kilometer")
AU = _Dimension(UnitKind.Distance, Fraction(149597870700), name="AU")

Second = _Dimension(UnitKind.Time, name="Second")
Minute = _Dimension(UnitKind.Time, Fraction(60), name="Minute")
Hour = _Dimension(UnitKind.Time, Fraction(3600), name="Hour")
Day = _Dimension(UnitKind.Time, Fraction(86400), name="Day")
Year = _Dimension(UnitKind.Time, Fraction(31557600), name="Year")


def middleDim(dim: _Dimension) -> _Dimension:
    match dim.kind:
        case UnitKind.Angle:
            return Radin
        case UnitKind.Distance:
            return Meter
        case UnitKind.Time:
            return Second
    raise ValueError(  # 错误类型
        f"unknown dimension kind: {dim.kind}")


class Dimension:
    def __init__(self, dim: _Dimension, exp: int = 1):
        self._dim = dim
        self.exp = exp

    @property
    def dimension(self) -> _Dimension:
        return self._dim

    @property
    def kind(self) -> UnitKind:
        return self._dim.kind

    @property
    def ratio(self) -> Fraction:
        return self._dim.ratio ** self.exp

    @property
    def name(self) -> Optional[str]:
        return self._dim.name

    def __repr__(self) -> str:
        return f"{self._dim}{'' if self.exp == 1 else f'^{self.exp}'}"


def cast(fromDim: Dimension, toDim: Dimension) -> float | int | Fraction:
    if fromDim.kind == toDim.kind:
        return toDim.ratio / fromDim.ratio

    raise ValueError(  # 错误类型
        f"cannot cast from {fromDim.kind} to {toDim.kind}")


class Pair[T, U]:
    def __init__(self, fst: T, snd: U):
        self.fst = fst
        self.snd = snd

    def __repr__(self) -> str:
        return f"({self.fst}, {self.snd})"


MiddleMap = dict[UnitKind, Pair[Optional[Dimension], Optional[Dimension]]]


def normalize(
        numerator: list[Dimension | _Dimension] | Dimension | _Dimension,
        denominator: list[Dimension | _Dimension] | Dimension | _Dimension
) -> tuple[list[Dimension], list[Dimension]]:  # 单位化
    handle = lambda x, src: x if isinstance(x, Dimension) else Dimension(x, 1 if src else -1)

    numerator = [handle(x, True) for x in numerator] if isinstance(numerator, Sequence) else [handle(numerator, True)]
    denominator = [handle(x, False) for x in denominator] if isinstance(denominator, Sequence) else [
        handle(denominator, False)]

    return numerator, denominator


class Unit:
    @overload
    def __init__(self, numerator: list[Dimension | _Dimension], denominator: list[Dimension | _Dimension], *,
                 factor: int | float | Fraction = 1) -> None:
        ...

    @overload
    def __init__(self, numerator: Dimension | _Dimension, denominator: Dimension | _Dimension, *,
                 factor: int | float | Fraction = 1) -> None:
        ...

    def __init__(self,
                 numerator: list[Dimension | _Dimension] | Dimension | _Dimension,
                 denominator: list[Dimension | _Dimension] | Dimension | _Dimension,
                 *, factor: int | float | Fraction = 1
                 ):
        self._numerator, self._denominator = normalize(numerator, denominator)
        self.factor = factor

    @property
    def numerator(self) -> list[Dimension]:
        return self._numerator

    @property
    def denominator(self) -> list[Dimension]:
        return self._denominator

    @staticmethod
    def merge(fromUnit: 'Unit', toUnit: 'Unit') -> MiddleMap:
        result: MiddleMap = {}

        isSource = True
        for dim in [*fromUnit.numerator, *fromUnit.denominator, 'n', *toUnit.numerator, *toUnit.denominator]:
            if isinstance(dim, str):
                if dim == 'n':
                    isSource = False
                continue

            attr = "fst" if isSource else "snd"

            if dim.kind in result:
                if (value := getattr(result[dim.kind], attr)) is None:
                    setattr(result[dim.kind], attr, dim)

                else:
                    if dim.name == value.name:
                        setattr(result[dim.kind], attr, Dimension(dim.dimension, dim.exp + value.exp))
                        continue

                    raise ValueError(  # 单位重复
                        f"在合并`{fromUnit}`和`{toUnit}`时，存在纬度相同({dim.kind})但名称不同的单位'{dim}'和'{value}'")

            else:
                result[dim.kind] = Pair(dim if isSource else None, None if isSource else dim)

        return result

    def __add__(self, other: Self) -> Self:
        if isinstance(other, Unit):
            denominator = []
            numerator = []
            factor = 1

            middle = Unit.merge(self, other)

            for _, pair in middle.items():
                # 单位求和运算要求量纲、量纲数量和系数相等
                if pair.fst is None or pair.snd is None:
                    raise ValueError(  # 量纲数量不相等
                        f"在计算`{self} + {other}`时，存在额外的单位'{pair.fst or pair.snd}'")

                if pair.fst.exp != pair.snd.exp:
                    raise ValueError(  # 幂次不相等
                        f"在计算`{self} + {other}`时，存在量纲幂次不相等的单位'{pair.fst}'和'{pair.snd}'")

                factor /= cast(pair.fst, pair.snd)
                (numerator if pair.fst.exp > 0 else denominator).append(pair.snd)

            return Unit(numerator, denominator, factor=factor)

        else:
            raise ValueError(  # 错误类型
                f"cannot add units with {type(other)}")

    def __neg__(self) -> Self:
        return Unit(self.numerator, self.denominator, factor=self.factor * -1)

    def __sub__(self, other: Self) -> Self:
        self.__add__(-other)

    def __mul__(self, other: Any) -> Self:
        if isinstance(other, Unit):
            numerator = []
            denominator = []
            factor = 1

            middle = Unit.merge(self, other)

            for _, pair in middle.items():
                # 乘则相合
                if pair.fst is None or pair.snd is None:
                    factor /= cast(pair.fst, pair.snd)
                    (numerator if (exp := pair.fst.exp + pair.snd.exp) > 0 else denominator).append(
                        Dimension(pair.snd.dimension, exp))

                elif pair.fst:
                    numerator.append(pair.fst)
                else:
                    denominator.append(pair.snd)

            return Unit(numerator, denominator, factor=factor)

        elif isinstance(other, (int, float, Fraction)):
            return Unit(self.numerator, self.denominator, factor=self.factor * other)

        raise ValueError(  # 错误类型
            f"cannot multiply units with {type(other)}")

    def __truediv__(self, other: Any) -> Self:
        if isinstance(other, Unit):
            numerator = []
            denominator = []
            factor = 1

            middle = Unit.merge(self, other)

            for _, pair in middle.items():
                # 除则相消
                if pair.fst is not None or pair.snd is not None:
                    factor /= cast(pair.fst, pair.snd)

                elif pair.fst:
                    denominator.append(pair.fst)
                else:
                    numerator.append(pair.snd)

            return Unit(numerator, denominator, factor=factor)

        elif isinstance(other, (int, float, Fraction)):
            return Unit(self.numerator, self.denominator, factor=self.factor / other)

        raise ValueError(  # 错误类型
            f"cannot divide units with {type(other)}")

    def __pow__(self, other: int) -> Self:
        numerator = [Dimension(x.dimension, x.exp * other) for x in self.numerator]
        denominator = [Dimension(x.dimension, x.exp * other) for x in self.denominator]
        return Unit(numerator, denominator, factor=self.factor ** other)

    def __eq__(self, other: Any) -> bool:
        if isinstance(other, Unit):
            return self.numerator == other.numerator and self.denominator == other.denominator

        return False

    def __repr__(self) -> str:
        return f"{'' if self.factor == 1 else f'{self.factor} * '}{'·'.join(map(str, self.numerator))}{'/' if self.denominator else ''}{'·'.join(map(lambda x: str(x).replace('-', ''), self.denominator))}"


class Value:
    def __init__(self, value: float | int | Fraction, unit: Unit):
        self.value = value
        self.unit = unit
        self.value *= unit.factor
        self.unit.factor = 1

    def __add__(self, other: Self) -> Self:
        if isinstance(other, Value):
            resultUnit = self.unit + other.unit
            factor = resultUnit.factor
            resultUnit.factor = 1
            return Value(self.value * factor + other.value, resultUnit)

        raise ValueError(  # 错误类型
            f"cannot add values with {type(other)}")

    def __sub__(self, other: Self) -> Self:
        if isinstance(other, Value):
            resultUnit = self.unit - other.unit
            factor = resultUnit.factor
            resultUnit.factor = 1
            return Value(self.value * factor - other.value, resultUnit)

        raise ValueError(  # 错误类型
            f"cannot subtract values with {type(other)}")

    def __mul__(self, other: Any) -> Self:
        if isinstance(other, Value):
            resultUnit = self.unit * other.unit
            factor = resultUnit.factor
            resultUnit.factor = 1
            return Value(self.value * factor * other.value, resultUnit)

        elif isinstance(other, (int, float, Fraction)):
            return Value(self.value * other, self.unit)

        raise ValueError(  # 错误类型
            f"cannot multiply values with {type(other)}")

    def __truediv__(self, other: Any) -> Self:
        if isinstance(other, Value):
            resultUnit = self.unit / other.unit
            factor = resultUnit.factor
            resultUnit.factor = 1
            return Value(self.value * factor / other.value, resultUnit)

        elif isinstance(other, (int, float, Fraction)):
            return Value(self.value / other, self.unit)

        raise ValueError(  # 错误类型
            f"cannot divide values with {type(other)}")

    def __pow__(self, other: int) -> Self:
        return Value(self.value ** other, self.unit ** other)

    def __repr__(self) -> str:
        hasUnit = self.unit.numerator or self.unit.denominator
        return f"{self.value}{'(' if hasUnit else ''}{self.unit}{')' if hasUnit else ''}"


if __name__ == '__main__':
    a = Value(1, Unit(Meter, Second))
    b = Value(1, Unit(Kilometer, Hour))

    print(a / b)
