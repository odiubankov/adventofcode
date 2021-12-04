#ifndef ADVENT_OF_CODE_2021_DAY4_BINGO_SUBSYSTEM_H
#define ADVENT_OF_CODE_2021_DAY4_BINGO_SUBSYSTEM_H

#include <string>
#include <sstream>

namespace aoc {

class BingoSubsystem {
public:
    explicit BingoSubsystem(std::string numbers_string, char delimiter);
    int get_next();

private:
    const std::string numbers_string_;
    const char delimiter_;
    std::istringstream numbers_stream_;
};

}// namespace aoc

#endif // ADVENT_OF_CODE_2021_DAY4_BINGO_SUBSYSTEM_H
