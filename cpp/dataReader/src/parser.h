// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file parser.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/25 22:38
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef PARSER_H
#define PARSER_H
#pragma once
#include "ast.h"
#include "lexer.h"
#include <cstddef>

class Parser {
public:
    Parser(const std::vector<std::shared_ptr<Token>>& tokens);

    Parser(const Parser&) = default;

    Parser(Parser&&) noexcept = default;

    Data parse();

private:
    std::vector<std::shared_ptr<Token>> tokens;

    mutable std::size_t idx{};

    std::shared_ptr<Token> current();

    std::shared_ptr<Token> advance();

#ifdef VSOP

    std::shared_ptr<Table> parseTable();

    std::shared_ptr<Header> parseHeader();

#endif

    std::shared_ptr<Term> parseTerm();

    std::shared_ptr<Expression> parseExpression();

    std::shared_ptr<Identifier> parseIdentifier();

    std::shared_ptr<Variable> parseVariable();

    std::shared_ptr<Literal> parseLiteral();

    std::shared_ptr<Token> expect(TokenType type);

    bool inScope() const noexcept;

    void skip() noexcept;
};

#endif  // PARSER_H
