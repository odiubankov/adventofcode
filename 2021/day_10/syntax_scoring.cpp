#include <string>
#include <fstream>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <vector>
#include <algorithm>

namespace {
using CloseOpenSymbols = std::unordered_map<char, char>;
using OpenSymbols = std::stack<char>;
using SymbolPoints = std::unordered_map<char, int>;
using CompletionErrorPoints = std::vector<unsigned long long>;

const CloseOpenSymbols close_open_symbols{
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'}
};

const CloseOpenSymbols open_close_symbols{
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'}
};

const SymbolPoints error_symbol_points{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
};

const SymbolPoints completion_symbol_points{
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}
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
                return error_symbol_points.at(symbol);
            }

            open_symbols.pop();
        }
    }

    return 0;
}

unsigned long long get_completion_points(const std::string& input_string) {
    OpenSymbols open_symbols;
    for (const auto symbol : input_string) {
        if (is_open_symbol(symbol)) {
            open_symbols.push(symbol);
        } else {
            open_symbols.pop();
        }
    }

    unsigned long long completion_points = 0;
    while (!open_symbols.empty()) {
        completion_points *= 5;
        const auto missing_close_symbol = open_close_symbols.at(open_symbols.top());
        completion_points += completion_symbol_points.at(missing_close_symbol);
        open_symbols.pop();
    }
    
    return completion_points;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    int total_error_points = 0;
    CompletionErrorPoints completion_error_points;
    while (std::getline(input_stream, input_string)) {
        const auto line_error_points = get_error_points(input_string);
        if (line_error_points != 0) {
            total_error_points += line_error_points;
        } else {
            completion_error_points.push_back(get_completion_points(input_string));
        }
    }

    std::cout << "total error points " << total_error_points << std::endl;

    std::sort(begin(completion_error_points), end(completion_error_points));
    std::cout << "middle completion points " << completion_error_points.at(completion_error_points.size() / 2) << std::endl;
    return 0;
}