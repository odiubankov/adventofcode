#include <string>
#include <fstream>
#include <unordered_map>
#include <stack>
#include <iostream>

namespace {
using CloseOpenSymbols = std::unordered_map<char, char>;
using OpenSymbols = std::stack<char>;
using SymbolPoints = std::unordered_map<char, int>;

const CloseOpenSymbols close_open_symbols{
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'}
};

const SymbolPoints symbol_points{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
};

bool is_open_symbol(char symbol) {
    return close_open_symbols.find(symbol) == end(close_open_symbols);
}

int get_error_points(const std::string& input_string) {
    OpenSymbols open_symbols;
    for (const auto symbol : input_string) {
        if (is_open_symbol(symbol)) {
            open_symbols.push(symbol);
        } else {
            if (open_symbols.empty() || open_symbols.top() != close_open_symbols.at(symbol)) {
                return symbol_points.at(symbol);
            }

            open_symbols.pop();
        }
    }

    return 0;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    int total_error_points = 0;
    while (std::getline(input_stream, input_string)) {
        total_error_points += get_error_points(input_string);
    }

    std::cout << "total error points " << total_error_points << std::endl;
    return 0;
}