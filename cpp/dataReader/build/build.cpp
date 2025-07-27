// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file build.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/25 21:56
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "../src/parser.h"
#include <format>
#include <fstream>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

Data parse(const std::string &src) {
    auto tokens = Lexer::tokenize(src);

    return Parser(tokens).parse();
}

PYBIND11_MODULE(dataReader, m) {
    m.doc() = R"(A module for reading VSOP2013 data files, it includes a parser and a lexer for VSOP2013 data files.)";

    m.def("parse", &parse, R"(Parse a VSOP2013 data file and return a Data object.)");

    py::enum_<TokenType>(m, "TokenType")
        .value("INT", TokenType::INT)
        .value("FLOAT", TokenType::FLOAT)
        .value("IDENTIFIER", TokenType::IDENTIFIER)
        .value("NEWLINE", TokenType::NEWLINE)
        .value("MUL", TokenType::MUL)
        .value("END", TokenType::END)
        .export_values();

    py::class_<Token, py::smart_holder>(m, "Token")
        .def(py::init<TokenType, std::string>(), py::arg("type"), py::arg("value"))
        .def_readonly("type", &Token::type)
        .def_readonly("value", &Token::value)
        .def("__str__", &Token::toString);

    py::class_<Lexer, py::smart_holder>(m, "Lexer")
        .def(py::init<std::string>(), py::arg("src"))
        .def_static("tokenize", &Lexer::tokenize, py::arg("src"))
        .def("next", &Lexer::next, "Get the next token in the lexer.");

    py::class_<Parser, py::smart_holder>(m, "Parser")
        .def(py::init<std::vector<std::shared_ptr<Token>>>(), py::arg("tokens"))
        .def("parse", &Parser::parse, "Parse the VSOP2013 data file and return a Data object.");

    py::class_<AST, py::smart_holder>(m, "AST");

    py::class_<Data, AST, py::smart_holder>(m, "Data")
        .def(py::init())
        .def_readonly("nodeName", &Data::nodeName)
        .def_readonly("tables", &Data::tables)
        .def("toJSON", &Data::toJSON, "Convert the Data object to a JSON string.");

    py::class_<Table, AST, py::smart_holder>(m, "Table")
        .def(py::init())
        .def_readonly("nodeName", &Table::nodeName)
        .def_readonly("header", &Table::header)
        .def_readonly("terms", &Table::terms)
        .def("toJSON", &Table::toJSON, "Convert the Table object to a JSON string.");

    py::class_<Header, AST, py::smart_holder>(m, "Header")
        .def(py::init())
        .def_readonly("nodeName", &Header::nodeName)
        .def_readonly("fields", &Header::fields)
        .def("toJSON", &Header::toJSON, "Convert the Header object to a JSON string.");

    py::class_<Term, AST, py::smart_holder>(m, "Term")
        .def(py::init())
        .def_readonly("nodeName", &Term::nodeName)
        .def_property_readonly("id", [](const Term &self) { return std::stoi(self.id->value()); })
        .def_readonly("coefficients", &Term::coefficients)
        .def_readonly("sinMantissa", &Term::sinMantissa)
        .def_readonly("cosMantissa", &Term::cosMantissa)
        .def_readonly("sinExponent", &Term::sinExponent)
        .def_readonly("cosExponent", &Term::cosExponent)
        .def("toJSON", &Term::toJSON, "Convert the Term object to a JSON string.");

    py::class_<Expression, AST, py::smart_holder>(m, "Expression");

    py::class_<Identifier, Expression, py::smart_holder>(m, "Identifier")
        .def(py::init<std::string>(), py::arg("name"))
        .def_readonly("nodeName", &Identifier::nodeName)
        .def_readwrite("name", &Identifier::name)
        .def("toJSON", &Identifier::toJSON, "Convert the Identifier object to a JSON string.");

    py::class_<Variable, Expression, py::smart_holder>(m, "Variable")
        .def(py::init<std::string, std::string>(), py::arg("name"), py::arg("flag"))
        .def_readonly("nodeName", &Variable::nodeName)
        .def_readwrite("name", &Variable::name)
        .def_readwrite("flag", &Variable::flag)
        .def("toJSON", &Variable::toJSON, "Convert the Variable object to a JSON string.");

    py::class_<Literal, Expression, py::smart_holder>(m, "Literal");

    py::class_<Integer, Literal, py::smart_holder>(m, "Integer")
        .def(py::init([](const py::int_ &value) { return Integer(std::to_string(value.cast<int>())); }), py::arg("value"))
        .def_readonly("nodeName", &Integer::nodeName)
        .def_property(
            "value", [](const Integer &self) { return std::stoi(self.value()); }, [](Integer &self, int value) { self.value_ = std::to_string(value); }
        )
        .def("toJSON", &Integer::toJSON, "Convert the Integer object to a JSON string.");

    py::class_<Float, Literal, py::smart_holder>(m, "Float")
        .def(py::init([](const py::float_ &value) { return Float(std::to_string(value.cast<double>())); }), py::arg("value"))
        .def_readonly("nodeName", &Float::nodeName)
        .def_property(
            "value", [](const Float &self) { return std::stod(self.value()); }, [](Float &self, double value) { self.value_ = std::to_string(value); }
        )
        .def("toJSON", &Float::toJSON, "Convert the Float object to a JSON string.");
}
