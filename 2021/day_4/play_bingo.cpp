#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "2021/day_4/bingo_subsystem.h"
#include "2021/day_4/bingo_board.h"

namespace {
    constexpr auto InputNumbersDelimeter = ',';
}

namespace aoc {
    using Boards = std::vector<aoc::BingoBoard>;
    using BoardIndexes = std::vector<std::size_t>;
    using NumberBoards = std::unordered_map<int, BoardIndexes>;
}

aoc::BingoBoard read_board(std::ifstream& bingo_input) {
    aoc::BingoBoard::BoardNumberStrings board_number_strings;
    for (std::size_t row = 0; row < aoc::BingoBoard::BoardWidth; ++row) {
        std::getline(bingo_input, board_number_strings.at(row));
    }

    return aoc::BingoBoard{board_number_strings};
}

aoc::Boards read_boards(std::ifstream& bingo_input) {
    aoc::Boards boards;
    std::string separation_line;
    while (std::getline(bingo_input, separation_line)) {
        auto board = read_board(bingo_input);
        boards.push_back(std::move(board));
    }

    return boards;
}

aoc::NumberBoards index_boards(const aoc::Boards& boards) {
    aoc::NumberBoards number_boards;
    for (std::size_t i = 0; i < boards.size(); ++i) {
        for (const auto number : boards.at(i).get_present_numbers()) {
            number_boards[number].push_back(i);
        }
    }

    return number_boards;
}

void play_bingo(
        aoc::Boards& boards,
        aoc::NumberBoards& number_boards,
        aoc::BingoSubsystem& bingo_subsystem) {
    bool first_board_found = false;
    std::unordered_set<std::size_t> won_boards;
    for (;;) {
        const auto next_number = bingo_subsystem.get_next();
        const auto number_boards_it = number_boards.find(next_number);
        if (number_boards_it == end(number_boards))
            continue;
        bool board_won = false;
        int max_score = 0;
        int min_score = std::numeric_limits<int>::max();
        for (const auto board_index : number_boards_it->second) {
            if (won_boards.find(board_index) != end(won_boards))
                continue;

            if (boards.at(board_index).check_is_winning(next_number)) {
                board_won = true;
                const auto board_score = boards.at(board_index).get_score();
                max_score = std::max(max_score, board_score);
                min_score = std::min(min_score, board_score);
                won_boards.insert(board_index);
            }
        }

        if (board_won && !first_board_found) {
            std::cout << "first winning score is " << (max_score * next_number) << std::endl;
            first_board_found = true;
        }

        number_boards.erase(number_boards_it);
        if (won_boards.size() == boards.size()) {
            std::cout << "last winning score is " << (min_score * next_number) << std::endl;
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    const auto bingo_input_path = argv[1];
    std::ifstream bingo_input{bingo_input_path};
    
    std::string numbers_string;
    std::getline(bingo_input, numbers_string);
    aoc::BingoSubsystem bingo_subsystem{std::move(numbers_string), InputNumbersDelimeter};

    auto boards = read_boards(bingo_input);
    auto number_boards = index_boards(boards);
    play_bingo(boards, number_boards, bingo_subsystem);

    return 0;
}
