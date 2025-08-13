// Copyright (c) 2025. All rights reserved.
// This source code is licensed under the CC BY-NC-SA
// (Creative Commons Attribution-NonCommercial-NoDerivatives) License, By Xiao Songtao.
// This software is protected by copyright law. Reproduction, distribution, or use for commercial
// purposes is prohibited without the author's permission. If you have any questions or require
// permission, please contact the author: 2207150234@st.sziit.edu.cn

/**
 * @file test.cpp 
 * @author edocsitahw 
 * @version 1.1
 * @date 2025/07/26 12:33
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */
#include "../src/parser.h"
#include <fstream>
#include <iostream>

int main() {
    using namespace astro::reader;

    std::ifstream file;

    // file.open(R"(test.dat)");
    file.open(R"(E:\code\astroCalendar\data\LEA-406\table9.dat)");
    // file.open(R"(E:\code\astroCalendar\cpp\dataReader\test\vsop_test.dat)");

    auto content = std::string(std::istreambuf_iterator(file), std::istreambuf_iterator<char>());

    auto tokens = Lexer::tokenize(content);

    for (const auto& token : tokens)
        std::cout << *token << std::endl;

    Parser parser(tokens);

    auto ast = parser.parse();

    std::cout << ast.toJSON() << std::endl;
}
