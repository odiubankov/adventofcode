#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <iostream>

namespace {

struct Position {
    std::size_t row;
    std::size_t column;
    int heuristics;
};

struct PositionComparator {
    bool operator()(const Position& a, const Position& b) const {
        return a.heuristics > b.heuristics;
    }
};

using CaveRow = std::vector<int>;
using CaveMap = std::vector<CaveRow>;
using VisitedRow = std::vector<bool>;
using VisitedMap = std::vector<VisitedRow>;
using PositionQueue = std::priority_queue<Position, std::vector<Position>, PositionComparator>;
using Neighbours = std::vector<Position>;

CaveRow read_cave_row(const std::string& input_string) {
    CaveRow cave_row;
    cave_row.reserve(input_string.size());
    std::transform(begin(input_string), end(input_string), back_inserter(cave_row), [](char c) { return c - '0'; } );
    return cave_row;
}

CaveMap read_cave_map(const std::string& input_path) {
    std::ifstream input_stream{input_path};
    CaveMap cave_map;
    std::string input_string;
    while (std::getline(input_stream, input_string)) {
        cave_map.push_back(read_cave_row(input_string));
    }

    return cave_map;
}

Neighbours get_neighbours(const Position& position, const CaveMap& cave_map) {
    Neighbours neighbours;
    if (position.row != 0) {
        neighbours.emplace_back(Position{position.row - 1, position.column, position.heuristics});
    }

    if (position.column != 0) {
        neighbours.emplace_back(Position{position.row, position.column - 1, position.heuristics});
    }

    if (position.row != (cave_map.size() - 1)) {
        neighbours.emplace_back(Position{position.row + 1, position.column, position.heuristics});
    }

    if (position.column != (cave_map.front().size() - 1)) {
        neighbours.emplace_back(Position{position.row, position.column + 1, position.heuristics});
    }

    return neighbours;
}

int get_min_path(const CaveMap& cave_map) {
    PositionQueue queue;
    VisitedMap visited_map(cave_map.size(), VisitedRow(cave_map.front().size(), false));
    queue.emplace(Position{0, 0, 0});
    while (!queue.empty()) {
        const auto next_position = queue.top();
        queue.pop();
        if (visited_map.at(next_position.row).at(next_position.column)) {
            continue;
        }

        visited_map.at(next_position.row).at(next_position.column) = true;
        if (next_position.row == (cave_map.size() - 1) && next_position.column == (cave_map.front().size() - 1)) {
            return next_position.heuristics + next_position.row + next_position.column;
        }

        visited_map.at(next_position.row).at(next_position.column) = true;
        const auto neighbours = get_neighbours(next_position, cave_map);
        for (auto neighbour : neighbours) {
            if (visited_map.at(neighbour.row).at(neighbour.column)) {
                continue;
            }

            neighbour.heuristics += next_position.row;
            neighbour.heuristics += next_position.column;
            neighbour.heuristics -= neighbour.row;
            neighbour.heuristics -= neighbour.column;
            neighbour.heuristics += cave_map.at(neighbour.row).at(neighbour.column);
            queue.push(std::move(neighbour));
        }
    }
    
    return 0;
}

int get_next_value(int value) {
    if (value < 9) {
        return value + 1;
    } else {
        return 1;
    }
}

CaveMap multiply_map(const CaveMap& cave_map, int coefficient) {
    CaveMap big_map(cave_map.size() * 5, CaveRow(cave_map.front().size() * 5, 0));
    for (std::size_t row = 0; row < cave_map.size(); ++row) {
        for (std::size_t column = 0; column < cave_map.front().size(); ++column) {
            big_map.at(row).at(column) = cave_map.at(row).at(column);
        }
    }

    for (std::size_t row = 0; row < cave_map.size(); ++row) {
        for (std::size_t column = cave_map.front().size(); column < big_map.front().size(); ++column) {
            const auto prev_val = big_map.at(row).at(column - cave_map.front().size());
            big_map.at(row).at(column) = get_next_value(prev_val);
        }
    }

    for (std::size_t row = cave_map.size(); row < big_map.size(); ++row) {
        for (std::size_t column = 0; column < big_map.front().size(); ++column) {
            const auto prev_val = big_map.at(row - cave_map.size()).at(column);
            big_map.at(row).at(column) = get_next_value(prev_val);
        }
    }

    return big_map;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    const auto cave_map = read_cave_map(input_path);
    std::cout << "minimum risk path " << get_min_path(cave_map) << std::endl;

    const auto big_map = multiply_map(cave_map, 5);
    std::cout << "big map minimum risk path " << get_min_path(big_map) << std::endl;
    return 0;
}
