// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file lexer.h
 * @author edocsitahw
 * @version 1.1
 * @date 2025/07/25 21:56
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#ifndef LEXER_H
#define LEXER_H
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

enum class TokenType { INT, FLOAT, IDENTIFIER, MUL, NEWLINE, END };

struct Token {
    TokenType type;
    std::string value;

    [[nodiscard]] std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

class Lexer {
public:
    Lexer(const std::string &);

    Token next() const;

    static std::vector<std::shared_ptr<Token>> tokenize(const std::string &);

private:
    std::string src;
    mutable std::size_t idx;

    Token extractIdentifier() const;

    Token extractNumber() const;

    std::optional<char> current() const;

    std::optional<char> peek(int offset = 1) const;

    bool inScope() const noexcept;

    void skip() const;
};

#endif  // LEXER_H
