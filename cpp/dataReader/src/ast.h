// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file ast.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/25 22:39
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef AST_H
#define AST_H
#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace astro::reader {
    enum Type { VSOP, LEA };

    extern Type type;

    using LiteralValue = std::variant<std::string, int, double>;

    struct AST {
        const char *nodeName;

        virtual ~AST() = default;

        [[nodiscard]] virtual const char *getNodeName() const;

        [[nodiscard]] virtual std::string toJSON() const = 0;
    };

    template<typename T>
        requires std::is_base_of_v<AST, T>
    void vectorJSONToSteam(const char *fieldName, const std::vector<std::shared_ptr<T>> &vec, std::ostream &oss) {
        oss << "\"" << fieldName << "\": [";

        for (std::size_t i{}; i < vec.size(); ++i) {
            oss << vec[i]->toJSON();

            if (i != vec.size() - 1) oss << ",";
        }
    }

    struct Expression : AST {};

    struct Identifier final : Expression {
        const char *nodeName = "Identifier";

        Identifier(std::string name);

        Identifier(const Identifier &other) = default;

        std::string name;

        [[nodiscard]] std::string toJSON() const override;
    };

    struct Variable final : Expression {
        const char *nodeName = "Variable";

        std::string name;

        std::string flag;

        Variable() = default;

        Variable(std::string name, std::string flag);

        Variable(const Variable &other) = default;

        [[nodiscard]] std::string toJSON() const override;
    };

    struct Literal : Expression {
        [[nodiscard]] virtual LiteralValue value() const = 0;
    };

    struct Integer final : Literal {
        const char *nodeName = "Integer";

        Integer(std::string value);

        Integer(const Integer &other) = default;

        std::string value_;

        mutable bool hasCache_ = false;

        mutable int cache_ = 0;

        [[nodiscard]] std::string toJSON() const override;

        [[nodiscard]] LiteralValue value() const override;
    };

    struct Float final : Literal {
        const char *nodeName = "Float";

        Float(std::string value);

        Float(const Float &other) = default;

        std::string value_;

        mutable bool hasCache_ = false;

        mutable double cache_ = 0.0;

        [[nodiscard]] std::string toJSON() const override;

        [[nodiscard]] LiteralValue value() const override;
    };

    struct Header final : AST {
        const char *nodeName = "Header";

        std::vector<std::shared_ptr<Expression>> fields;

        Header() = default;

        Header(std::vector<std::shared_ptr<Expression>> fields);

        Header(const Header &other) = default;

        [[nodiscard]] std::string toJSON() const override;
    };

    struct Term final : AST {
        const char *nodeName = "Term";
        std::shared_ptr<Integer> id;

        std::vector<std::shared_ptr<Literal>> coefficients;

        // 当TYPE为VSOP时
        std::shared_ptr<Literal> sinMantissa;

        // 当TYPE为VSOP时
        std::shared_ptr<Literal> cosMantissa;

        // 当TYPE为VSOP时
        std::shared_ptr<Literal> sinExponent;

        // 当TYPE为VSOP时
        std::shared_ptr<Literal> cosExponent;

        // 当TYPE为LEA时
        std::vector<std::shared_ptr<Literal>> amplitudes;

        // 当TYPE为LEA时
        std::vector<std::shared_ptr<Literal>> phases;

        Term() = default;

        // 重载: 当TYPE为VSOP时
        Term(
            const std::shared_ptr<Integer> &id, const std::vector<std::shared_ptr<Literal>> &coefficients, const std::shared_ptr<Literal> &sinMantissa, const std::shared_ptr<Literal> &cosMantissa,
            const std::shared_ptr<Literal> &sinExponent, const std::shared_ptr<Literal> &cosExponent
        );

        // 重载: 当TYPE为LEA时
        Term(
            const std::shared_ptr<Integer> &id, const std::vector<std::shared_ptr<Literal>> &coefficients, const std::vector<std::shared_ptr<Literal>> &amplitudes,
            const std::vector<std::shared_ptr<Literal>> &phases
        );

        Term(const Term &other) = default;

        [[nodiscard]] std::string toJSON() const override;
    };

    struct Table final : AST {
        const char *nodeName = "Table";

        std::shared_ptr<Header> header;

        std::vector<std::shared_ptr<Term>> terms;

        Table() = default;

        Table(std::shared_ptr<Header> header, std::vector<std::shared_ptr<Term>> terms);

        Table(const Table &other) = default;

        [[nodiscard]] std::string toJSON() const override;
    };

    struct Data final : AST {
        const char *nodeName = "Data";

        Type type_;

        // 当TYPE为VSOP时
        std::vector<std::shared_ptr<Table>> tables;

        // 当TYPE为LEA时
        std::vector<std::shared_ptr<Term>> terms;

        Data() = default;

        // 重载: 当TYPE为VSOP时
        Data(std::vector<std::shared_ptr<Table>> tables);

        // 重载: 当TYPE为LEA时
        Data(std::vector<std::shared_ptr<Term>> terms);

        Data(const Data &other) = default;

        [[nodiscard]] std::string toJSON() const override;
    };
}  // namespace astro::reader


#endif  // AST_H
