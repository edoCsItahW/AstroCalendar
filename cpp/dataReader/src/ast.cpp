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

namespace astro::reader {
    Type type = LEA;

    const char *AST::getNodeName() const { return nodeName; }

    // -------------------- Data --------------------

    Data::Data(std::vector<std::shared_ptr<Table>> tables)
        : tables(std::move(tables)) {}

    Data::Data(std::vector<std::shared_ptr<Term>> terms)
        : terms(std::move(terms)) {}

    std::string Data::toJSON() const {
        std::ostringstream oss;

        oss << R"({"nodeName": ")" << nodeName << "\",";

        if (type == VSOP)
            vectorJSONToSteam("tables", tables, oss);

        else
            vectorJSONToSteam("terms", terms, oss);

        oss << "}";

        return oss.str();
    }

    // -------------------- Table --------------------

    Table::Table(std::shared_ptr<Header> header, std::vector<std::shared_ptr<Term>> terms)
        : header(std::move(header))
        , terms(std::move(terms)) {}

    std::string Table::toJSON() const {
        std::ostringstream oss;

        oss << R"({"nodeName": ")" << nodeName << std::format(R"(","header": {},)", header->toJSON());

        vectorJSONToSteam("terms", terms, oss);

        oss << "}";

        return oss.str();
    }

    // -------------------- Header --------------------

    Header::Header(std::vector<std::shared_ptr<Expression>> fields)
        : fields(std::move(fields)) {}

    std::string Header::toJSON() const {
        std::ostringstream oss;

        oss << R"({"nodeName": ")" << nodeName << "\",";

        vectorJSONToSteam("fields", fields, oss);

        oss << "}";

        return oss.str();
    }

    // -------------------- Term --------------------

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

    Term::Term(
        const std::shared_ptr<Integer> &id, const std::vector<std::shared_ptr<Literal>> &coefficients, const std::vector<std::shared_ptr<Literal>> &amplitudes,
        const std::vector<std::shared_ptr<Literal>> &phases
    )
        : id(id)
        , coefficients(coefficients)
        , amplitudes(amplitudes)
        , phases(phases) {}

    std::string Term::toJSON() const {
        std::ostringstream oss;

        oss << R"({"nodeName": ")" << nodeName << std::format(R"(","id": {},"coefficients": [)", id->toJSON());

        for (std::size_t i{}; i < coefficients.size(); ++i) {
            oss << coefficients[i]->toJSON();

            if (i != coefficients.size() - 1) oss << ",";
        }

        oss << "],";

        if (type == VSOP)
            oss << std::format(
                R"("sinMantissa": {},"cosMantissa": {},"sinExponent": {},"cosExponent": {})", sinMantissa->toJSON(), cosMantissa->toJSON(), sinExponent->toJSON(), cosExponent->toJSON()
            );

        else {
            vectorJSONToSteam("amplitudes", amplitudes, oss);

            oss << ",";

            vectorJSONToSteam("phases", phases, oss);
        }

        oss << "}";

        return oss.str();
    }

    // -------------------- Identifier --------------------

    Identifier::Identifier(std::string name)
        : name(std::move(name)) {}

    std::string Identifier::toJSON() const { return std::format(R"({{"nodeName": "{}","name": "{}"}})", nodeName, name); }

    // -------------------- Variable --------------------

    Variable::Variable(std::string name, std::string flag)
        : name(std::move(name))
        , flag(std::move(flag)) {}

    std::string Variable::toJSON() const { return std::format(R"({{"nodeName": "{}","name": "{}","flag": "{}"}})", nodeName, name, flag); }

    // -------------------- Integer --------------------

    Integer::Integer(std::string value)
        : value_(std::move(value)) {}

    LiteralValue Integer::value() const {
        if (!hasCache_) {
            cache_    = std::stoi(value_);
            hasCache_ = true;
        }

        return cache_;
    }

    std::string Integer::toJSON() const { return std::format(R"({{"nodeName": "{}","value": "{}"}})", nodeName, value_); }

    // -------------------- Float --------------------

    Float::Float(std::string value)
        : value_(std::move(value)) {}

    LiteralValue Float::value() const {
        if (!hasCache_) {
            cache_    = std::stod(value_);
            hasCache_ = true;
        }

        return cache_;
    }

    std::string Float::toJSON() const { return std::format(R"({{"nodeName": "{}","value": "{}"}})", nodeName, value_); }
}  // namespace astro::reader
