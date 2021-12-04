#ifndef ADVENT_OF_CODE_2021_DAY4_BINGO_BOARD_H
#define ADVENT_OF_CODE_2021_DAY4_BINGO_BOARD_H

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

namespace aoc {

class BingoBoard {
public:
    static constexpr std::size_t BoardWidth = 5;
    using BoardNumberStrings = std::array<std::string, BoardWidth>;
    using Numbers = std::vector<int>;

    explicit BingoBoard(const BoardNumberStrings& board_number_strings);

    bool check_is_winning(int next_number);
    int get_score() const;
    Numbers get_present_numbers() const;

private:
    using MarkedCount = std::array<std::size_t, BoardWidth>;
    using CellPosition = std::pair<std::size_t, std::size_t>;
    using CellPositions = std::vector<CellPosition>;
    using NumberPositions = std::unordered_map<int, CellPositions>;

    MarkedCount row_marked_count_{};
    MarkedCount column_marked_count_{};
    NumberPositions number_positions_;
};

} // namespace aoc

#endif // ADVENT_OF_CODE_2021_DAY4_BINGO_BOARD_H
