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

#include <memory>
#include <string>
#include <vector>

struct AST {
    const char *nodeName;

    virtual ~AST() = default;

    [[nodiscard]] virtual const char *getNodeName() const;

    [[nodiscard]] virtual std::string toJSON() const = 0;
};

struct Expression : AST {};

struct Identifier final : Expression {
    const char *nodeName = "Identifier";

    Identifier(std::string name);

    std::string name;

    [[nodiscard]] std::string toJSON() const override;
};

struct Variable final : Expression {
    const char *nodeName = "Variable";

    std::string name;

    std::string flag;

    Variable() = default;

    Variable(std::string name, std::string flag);

    [[nodiscard]] std::string toJSON() const override;
};

struct Literal : Expression {
    [[nodiscard]] virtual std::string value() const = 0;
};

struct Integer final : Literal {
    const char *nodeName = "Integer";

    Integer(std::string value);

    std::string value_;

    [[nodiscard]] std::string toJSON() const override;

    [[nodiscard]] std::string value() const override;
};

struct Float final : Literal {
    const char *nodeName = "Float";

    Float(std::string value);

    std::string value_;

    [[nodiscard]] std::string toJSON() const override;

    [[nodiscard]] std::string value() const override;
};

struct Header final : AST {
    const char *nodeName = "Header";

    std::vector<std::shared_ptr<Expression>> fields;

    Header() = default;

    Header(std::vector<std::shared_ptr<Expression>> fields);

    [[nodiscard]] std::string toJSON() const override;
};

struct Term final : AST {
    const char *nodeName = "Term";
    std::shared_ptr<Integer> id;

    std::vector<std::shared_ptr<Literal>> coefficients;

    std::shared_ptr<Literal> sinMantissa;

    std::shared_ptr<Literal> cosMantissa;

    std::shared_ptr<Literal> sinExponent;

    std::shared_ptr<Literal> cosExponent;

    Term() = default;

    Term(
        const std::shared_ptr<Integer> &id, const std::vector<std::shared_ptr<Literal>> &coefficients, const std::shared_ptr<Literal> &sinMantissa, const std::shared_ptr<Literal> &cosMantissa,
        const std::shared_ptr<Literal> &sinExponent, const std::shared_ptr<Literal> &cosExponent
    );

    [[nodiscard]] std::string toJSON() const override;
};

struct Table final : AST {
    const char *nodeName = "Table";

    std::shared_ptr<Header> header;

    std::vector<std::shared_ptr<Term>> terms;

    Table() = default;

    Table(std::shared_ptr<Header> header, std::vector<std::shared_ptr<Term>> terms);

    [[nodiscard]] std::string toJSON() const override;
};

struct Data final : AST {
    const char *nodeName = "Data";

    std::vector<std::shared_ptr<Table>> tables;

    Data() = default;

    Data(std::vector<std::shared_ptr<Table>> tables);

    [[nodiscard]] std::string toJSON() const override;
};

#endif  // AST_H
