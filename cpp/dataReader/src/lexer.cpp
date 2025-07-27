// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file lexer.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/25 21:56
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "lexer.h"
#include "utils.h"
#include <format>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << token.toString();
    return os;
}

std::string Token::toString() const { return std::format("Token<{}>('{}')", enumToStr(type), value); }

Lexer::Lexer(const std::string &src)
    : src(src)
    , idx(0) {}

Token Lexer::next() const {
    skip();

    const auto c = current();

    if (!c) return {TokenType::END, ""};

    if (*c == '\n') {
        ++idx;
        return {TokenType::NEWLINE, "\\n"};
    }

    if (isLetter(*c)) return extractIdentifier();

    if (isDigit(*c) || ((*c == '-' || *c == '+') && peek() && isDigit(*peek()))) return extractNumber();

    if (*c == '*') {
        ++idx;
        return {TokenType::MUL, "*"};
    }

    throw std::runtime_error(std::format("Unexpected character '{}' at position {}", *c, idx));
}

Token Lexer::extractIdentifier() const {
    std::string value;

    while (current() && (isalnum(*current()) || *current() == '-')) value += src[idx++];

    return {TokenType::IDENTIFIER, value};
}

Token Lexer::extractNumber() const {
    std::string value;
    bool isFloat = false;

    if (current() && (*current() == '-' || *current() == '+')) value += src[idx++];

    while (current() && (isDigit(*current()) || *current() == '.')) {
        if (*current() == '.') {
            if (isFloat) throw std::runtime_error(std::format("Multiple decimal points in number at position {}", idx));

            isFloat = true;
        }

        value += src[idx++];
    }

    return {isFloat ? TokenType::FLOAT : TokenType::INT, value};
}

std::optional<char> Lexer::current() const {
    if (inScope()) return src[idx];

    return std::nullopt;
}

std::optional<char> Lexer::peek(int offset) const {
    if (idx + offset < src.size()) return src[idx + offset];

    return std::nullopt;
}

bool Lexer::inScope() const noexcept { return idx < src.size(); }

void Lexer::skip() const {
    while (inScope() && isWhitespace(*current()) && *current() != '\n') idx++;
}

std::vector<std::shared_ptr<Token>> Lexer::tokenize(const std::string &src) {
    const Lexer lexer(src);
    std::vector<std::shared_ptr<Token>> tokens;
    std::shared_ptr<Token> token;

    while ((token = std::make_shared<Token>(lexer.next()))->type != TokenType::END) tokens.push_back(std::move(token));

    return tokens;
}
