/**
 * @file test.cpp
 * @author edocsitahw
 * @version 1.1
 * @date 2025/08/12 19:38
 * @brief
 * @copyright CC BY-NC-SA 2025. All rights reserved.
 * */

#include "src/lexer.h"
#include "src/parser.h"
#include "../src/main.h"
#include "../src/utils.h"
#include "../src/vsop.h"
#include <fstream>
#include <iostream>

astro::reader::Data parse(const std::string& content) {
    auto tokens = astro::reader::Lexer::tokenize(content);

    return astro::reader::Parser(tokens).parse();
}

void brent_test() {
    const auto func = [](double x) { return x * x - 2 * x + 1; };

    auto result = astro::brent(func, 0, 1);

    std::cout << "Brent Result: " << result << std::endl;
}

void main_test() {
    using namespace astro;

    std::ifstream vsopFile(R"(E:/code/astroCalendar/cpp/dataReader/test/vsop_test.dat)");

    std::string vsopContend{std::istreambuf_iterator(vsopFile), std::istreambuf_iterator<char>()};

    auto vsopData = parse(vsopContend);

    std::ifstream leaFile(R"(E:/code/astroCalendar/cpp/dataReader/test/lea_test.dat)");

    std::string leaContend{std::istreambuf_iterator(leaFile), std::istreambuf_iterator<char>()};

    auto leaData = parse(leaContend);

    auto date = DateTime{2025, 8, 12, 12, 0, 0, UTC};

    auto lunarDate = gregorianToLunar(date, vsopData, leaData, leaData, leaData);

    std::cout << "Lunar Date: " << lunarDate.toString() << std::endl;
}

void main_run() {
    using namespace astro;

    std::ifstream vsopFile(R"(E:/code/astroCalendar/data/VSOP2013/VSOP2013p3.dat)");

    std::string vsopContend{std::istreambuf_iterator(vsopFile), std::istreambuf_iterator<char>()};

    auto vsopData = parse(vsopContend);

    std::ifstream leaRFile(R"(E:/code/astroCalendar/data/LEA-406/table9.dat)");
    std::ifstream leaVFile(R"(E:/code/astroCalendar/data/LEA-406/table10.dat)");
    std::ifstream leaUFile(R"(E:/code/astroCalendar/data/LEA-406/table11.dat)");

    std::string leaRContend{std::istreambuf_iterator(leaRFile), std::istreambuf_iterator<char>()};
    std::string leaVContend{std::istreambuf_iterator(leaVFile), std::istreambuf_iterator<char>()};
    std::string leaUContend{std::istreambuf_iterator(leaUFile), std::istreambuf_iterator<char>()};

    auto leaRData = parse(leaRContend);
    auto leaVData = parse(leaVContend);
    auto leaUData = parse(leaUContend);

    auto date = DateTime{2025, 8, 12, 12, 0, 0, UTC};

    auto lunarDate = gregorianToLunar(date, vsopData, leaRData, leaVData, leaUData);

    std::cout << "Lunar Date: " << lunarDate.toString() << std::endl;
}

int main() {
    main_run();
    return 0;
}
