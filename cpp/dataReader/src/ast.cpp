// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file ast.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/26 02:58
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "ast.h"
#include <format>
#include <sstream>
#include <utility>

const char *AST::getNodeName() const { return nodeName; }

#ifdef VSOP

Data::Data(std::vector<std::shared_ptr<Table>> tables)
    : tables(std::move(tables)) {}

std::string Data::toJSON() const {
    std::ostringstream oss;

    oss << R"({"nodeName": ")" << nodeName << R"(","tables":[)";

    for (std::size_t i{}; i < tables.size(); ++i) {
        oss << tables[i]->toJSON();

        if (i != tables.size() - 1) oss << ",";
    }

    oss << "]}";

    return oss.str();
}

Table::Table(std::shared_ptr<Header> header, std::vector<std::shared_ptr<Term>> terms)
    : header(std::move(header))
    , terms(std::move(terms)) {}

std::string Table::toJSON() const {
    std::ostringstream oss;

    oss << R"({"nodeName": ")" << nodeName << std::format(R"(","header": {},"terms": [)", header->toJSON());

    for (std::size_t i{}; i < terms.size(); ++i) {
        oss << terms[i]->toJSON();

        if (i != terms.size() - 1) oss << ",";
    }

    oss << "]}";

    return oss.str();
}

Header::Header(std::vector<std::shared_ptr<Expression>> fields)
    : fields(std::move(fields)) {}

std::string Header::toJSON() const {
    std::ostringstream oss;

    oss << R"({"nodeName": ")" << nodeName << R"(","fields": [)";

    for (std::size_t i{}; i < fields.size(); ++i) {
        oss << fields[i]->toJSON();

        if (i != fields.size() - 1) oss << ",";
    }

    oss << "]}";

    return oss.str();
}

Term::Term(
    const std::shared_ptr<Integer> &id, const std::vector<std::shared_ptr<Literal>> &coefficients, const std::shared_ptr<Literal> &sinMantissa, const std::shared_ptr<Literal> &cosMantissa,
    const std::shared_ptr<Literal> &sinExponent, const std::shared_ptr<Literal> &cosExponent
)
    : id(id)
    , coefficients(coefficients)
    , sinMantissa(sinMantissa)
    , cosMantissa(cosMantissa)
    , sinExponent(sinExponent)
    , cosExponent(cosExponent) {}

std::string Term::toJSON() const {
    std::ostringstream oss;

    oss << R"({"nodeName": ")" << nodeName << std::format(R"(","id": {},"coefficients": [)", id->toJSON());

    for (std::size_t i{}; i < coefficients.size(); ++i) {
        oss << coefficients[i]->toJSON();

        if (i != coefficients.size() - 1) oss << ",";
    }

    oss << "],"
        << std::format(R"("sinMantissa": {},"cosMantissa": {},"sinExponent": {},"cosExponent": {})", sinMantissa->toJSON(), cosMantissa->toJSON(), sinExponent->toJSON(), cosExponent->toJSON());

    oss << "}";

    return oss.str();
}

#elifdef LEA

Data::Data(std::vector<std::shared_ptr<Term>> terms)
    : terms(std::move(terms)) {}

std::string Data::toJSON() const {
    std::ostringstream oss;

    oss << R"({"nodeName": ")" << nodeName << R"(","terms":[)";

    for (std::size_t i{}; i < terms.size(); ++i) {
        oss << terms[i]->toJSON();

        if (i != terms.size() - 1) oss << ",";
    }

    oss << "]}";

    return oss.str();
}

Term::Term(std::shared_ptr<Integer> id, std::vector<std::shared_ptr<Literal>> coefficients, std::vector<std::shared_ptr<Literal>> amplitudes, std::vector<std::shared_ptr<Literal>> phases)
    : id(std::move(id))
    , coefficients(std::move(coefficients))
    , amplitudes(std::move(amplitudes))
    , phases(std::move(phases)) {}

std::string Term::toJSON() const {
    std::ostringstream oss;

    oss << R"({"nodeName": ")" << nodeName << std::format(R"(","id": {},"coefficients": [)", id->toJSON());

    for (std::size_t i{}; i < coefficients.size(); ++i) {
        oss << coefficients[i]->toJSON();

        if (i != coefficients.size() - 1) oss << ",";
    }

    oss << "]," << std::format(R"("amplitudes": [)", "");

    for (std::size_t i{}; i < amplitudes.size(); ++i) {
        oss << amplitudes[i]->toJSON();

        if (i != amplitudes.size() - 1) oss << ",";
    }

    oss << "]," << std::format(R"("phases": [)", "");

    for (std::size_t i{}; i < phases.size(); ++i) {
        oss << phases[i]->toJSON();

        if (i != phases.size() - 1) oss << ",";
    }

    oss << "]}";

    return oss.str();
}


#else

    #error "must define VSOP or LEA macro"

#endif

Identifier::Identifier(std::string name)
    : name(std::move(name)) {}

std::string Identifier::toJSON() const { return std::format(R"({{"nodeName": "{}","name": "{}"}})", nodeName, name); }

Variable::Variable(std::string name, std::string flag)
    : name(std::move(name))
    , flag(std::move(flag)) {}

std::string Variable::toJSON() const { return std::format(R"({{"nodeName": "{}","name": "{}","flag": "{}"}})", nodeName, name, flag); }

Integer::Integer(std::string value)
    : value_(std::move(value)) {}

std::string Integer::value() const { return value_; }

std::string Integer::toJSON() const { return std::format(R"({{"nodeName": "{}","value": "{}"}})", nodeName, value_); }

Float::Float(std::string value)
    : value_(std::move(value)) {}

std::string Float::value() const { return value_; }

std::string Float::toJSON() const { return std::format(R"({{"nodeName": "{}","value": "{}"}})", nodeName, value_); }
