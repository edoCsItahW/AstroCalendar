// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file parser.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/25 22:38
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "parser.h"
#include "utils.h"
#include <format>
#include <stdexcept>

namespace astro::reader {

    Parser::Parser(const std::vector<std::shared_ptr<Token>> &tokens)
        : tokens(tokens) {}

    std::shared_ptr<Token> Parser::current() { return inScope() ? tokens[idx] : nullptr; }

    Data Parser::parse() {
        Data data;

        skip();

        if (current() && current()->type == TokenType::IDENTIFIER) {
            type       = VSOP;
            data.type_ = VSOP;
        } else {
            type       = LEA;
            data.type_ = LEA;
        }

        while (inScope()) {
            skip();

            if (data.type_ == VSOP) {
                if (auto table = parseTable()) data.tables.push_back(std::move(table));
            }

            else if (auto term = parseTerm()) {
                data.terms.push_back(std::move(term));

                if (current() && current()->type == TokenType::NEWLINE) expect(TokenType::NEWLINE);
            }
        }

        return data;
    }

    std::shared_ptr<Table> Parser::parseTable() {
        Table table;

        table.header = parseHeader();

        skip();

        while (inScope() && current()->type != TokenType::IDENTIFIER) {
            if (auto term = parseTerm()) table.terms.push_back(std::move(term));

            skip();
        }

        return std::make_shared<Table>(std::move(table));
    }

    std::shared_ptr<Header> Parser::parseHeader() {
        Header header;

        skip();

        while (inScope()) {
            if (auto field = parseExpression()) {
                header.fields.push_back(std::move(field));

                if (!current() || current()->type == TokenType::NEWLINE) break;
            }

            skip();
        }

        return std::make_shared<Header>(std::move(header));
    }

    std::shared_ptr<Term> Parser::parseTerm() {
        Term term;

        skip();

        term.id = std::make_shared<Integer>(Integer{expect(TokenType::INT)->value});

        for (std::size_t i{}; i < (type == VSOP ? 17 : 14); ++i)
            if (auto item = parseLiteral()) term.coefficients.push_back(std::move(item));

        if (type == VSOP) {
            term.sinMantissa = parseLiteral();
            term.sinExponent = parseLiteral();
            term.cosMantissa = parseLiteral();
            term.cosExponent = parseLiteral();
        }

        else {
            for (std::size_t i{}; i < 3; ++i)
                if (auto item = parseLiteral()) term.amplitudes.push_back(std::move(item));

            for (std::size_t i{}; i < 3; ++i)
                if (auto item = parseLiteral()) term.phases.push_back(std::move(item));
        }

        return std::make_shared<Term>(std::move(term));
    }

    std::shared_ptr<Expression> Parser::parseExpression() {
        switch (current()->type) {
            using enum TokenType;
            case IDENTIFIER: return parseIdentifier();
            case MUL: return parseVariable();
            default: {
                auto l = parseLiteral();
                return l;
            }
        }
    }

    std::shared_ptr<Identifier> Parser::parseIdentifier() { return std::make_shared<Identifier>(Identifier{advance()->value}); }

    std::shared_ptr<Variable> Parser::parseVariable() {
        Variable variable;

        expect(TokenType::MUL);

        variable.name = expect(TokenType::IDENTIFIER)->value;

        expect(TokenType::MUL);

        variable.flag = expect(TokenType::INT)->value;

        return std::make_shared<Variable>(std::move(variable));
    }

    std::shared_ptr<Literal> Parser::parseLiteral() {
        skip();

        switch (current()->type) {
            using enum TokenType;
            case INT: return std::make_shared<Integer>(Integer{advance()->value});
            case FLOAT: return std::make_shared<Float>(Float{advance()->value});
            default: throw std::runtime_error(std::format("Literal: Unexpected token type {} at position {}", enumToStr(current()->type), idx));
        }
    }

    std::shared_ptr<Token> Parser::advance() {
        const auto token = current();
        ++idx;
        return std::make_shared<Token>(*token);
    }

    bool Parser::inScope() const noexcept { return idx < tokens.size() && tokens[idx] != nullptr && tokens[idx]->type != TokenType::END; }

    std::shared_ptr<Token> Parser::expect(const TokenType type) {
        if (type != TokenType::NEWLINE) skip();

        if (inScope() && type == current()->type) return advance();

        throw std::runtime_error(std::format("expect: Unexpected token type {} at position {} expected {}", enumToStr(current()->type), idx, enumToStr(type)));
    }

    void Parser::skip() noexcept {
        while (current() && current()->type == TokenType::NEWLINE) ++idx;
    }

}  // namespace astro::reader
