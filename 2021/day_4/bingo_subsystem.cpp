#include "bingo_subsystem.h"

namespace aoc {

BingoSubsystem::BingoSubsystem(std::string numbers_string, const char delimiter)
: numbers_string_{std::move(numbers_string)}
, delimiter_{delimiter}
, numbers_stream_{numbers_string_}
{}

int BingoSubsystem::get_next() {
    std::string number_string;
    std::getline(numbers_stream_, number_string, delimiter_);
    return std::stoi(number_string);
}

}// namespace aoc
