#include "bingo_board.h"

#include <algorithm>
#include <sstream>

namespace {
    constexpr auto BoardNumberDelimiter = ' ';
}

namespace aoc {

BingoBoard::BingoBoard(const BoardNumberStrings& board_number_strings) {
    for (std::size_t row = 0; row < board_number_strings.size(); ++row) {
        std::istringstream numbers_stream{board_number_strings.at(row)};
        for (std::size_t col = 0; col < BoardWidth; ++col) {
            std::string number_string;
            do {
                std::getline(numbers_stream, number_string, BoardNumberDelimiter);
            } while(number_string.empty());
            const auto number = std::stoi(number_string);
            number_positions_[number].emplace_back(row, col);
        }
    }
}

bool BingoBoard::check_is_winning(int next_number) {
    const auto number_it = number_positions_.find(next_number);
    auto winning = false;
    for (const auto& cell : number_it->second) {
        const auto row = cell.first;
        if ((++(row_marked_count_.at(row))) == BoardWidth) {
            winning = true;
            break;
        }

        const auto column = cell.second;
        if ((++(column_marked_count_.at(column))) == BoardWidth) {
            winning = true;
            break;
        }
    }

    number_positions_.erase(number_it);
    return winning;
}

int BingoBoard::get_score() const {
    int score = 0;
    for (const auto& number_positions : number_positions_) {
        const auto number = number_positions.first;
        const auto count = number_positions.second.size();
        score += (number * count);
    }

    return score;
}

BingoBoard::Numbers BingoBoard::get_present_numbers() const {
    BingoBoard::Numbers numbers;
    numbers.reserve(number_positions_.size());
    std::transform(
        begin(number_positions_), end(number_positions_),
        back_inserter(numbers),
        [](const std::pair<int, CellPositions>& cell_positions) {
            return cell_positions.first;
        });
    return numbers;
}

} // namespace aoc
