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
#include "../src/utils.h"
#include <format>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

Data parse(const std::string &src) {
    auto tokens = Lexer::tokenize(src);

    return Parser(tokens).parse();
}

py::object variantToObj(const LiteralValue &value) {
    return std::visit([](auto &&arg) -> py::object { return py::cast(arg); }, value);
}

py::list variantToList(const std::vector<std::shared_ptr<Literal>> &values) {
    py::list result;

    for (const auto &value : values) result.append(variantToObj(value->value()));

    return result;
}

#define ATTR_HANDLER_WHEN(field, base, tp)                                                                                                                                                             \
    #field, [](const base &self) {                                                                                                                                                                     \
        if (type != tp) throw py::value_error("Data object is not a " #tp " data file.");                                                                                                              \
        return variantToObj(self.field->value());                                                                                                                                                      \
    }

#define ATTR_HANDLER(field, base) #field, [](const base &self) { return variantToObj(self.field->value()); }

#define ATTR_HANDLER_LIST_WHEN(field, base, tp)                                                                                                                                                        \
    #field, [](const base &self) {                                                                                                                                                                     \
        if (type != tp) throw py::value_error("Data object is not a " #tp " data file.");                                                                                                              \
        return variantToList(self.field);                                                                                                                                                              \
    }

#define ATTR_HANDLER_LIST(field, base) #field, [](const base &self) { return variantToList(self.field); }

PYBIND11_MODULE(dataReader, m) {
    m.doc() = R"(A module for reading VSOP2013 or LEA-406 data files, it includes a parser and a lexer for VSOP2013 and LEA-406 data files.)";

    m.def("parse", &parse, R"(Parse a VSOP2013 or LEA-406 data file and return a Data object.)");

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
        .def_readonly("terms", &Data::terms)
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
        .def_property_readonly(ATTR_HANDLER(id, Term))
        .def_property_readonly(ATTR_HANDLER_LIST(coefficients, Term))
        .def_property_readonly(ATTR_HANDLER(sinMantissa, Term))
        .def_property_readonly(ATTR_HANDLER(cosMantissa, Term))
        .def_property_readonly(ATTR_HANDLER(sinExponent, Term))
        .def_property_readonly(ATTR_HANDLER(cosExponent, Term))
        .def_property_readonly(ATTR_HANDLER_LIST(amplitudes, Term))
        .def_property_readonly(ATTR_HANDLER_LIST(phases, Term))
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
        .def_property_readonly("value", [](const Integer &self) { return variantToObj(self.value()); })
        .def("toJSON", &Integer::toJSON, "Convert the Integer object to a JSON string.");

    py::class_<Float, Literal, py::smart_holder>(m, "Float")
        .def(py::init([](const py::float_ &value) { return Float(std::to_string(value.cast<double>())); }), py::arg("value"))
        .def_readonly("nodeName", &Float::nodeName)
        .def_property_readonly("value", [](const Float &self) { return variantToObj(self.value()); })
        .def("toJSON", &Float::toJSON, "Convert the Float object to a JSON string.");
}
