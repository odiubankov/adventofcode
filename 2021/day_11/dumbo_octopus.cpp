#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace{
using OctopusesRow = std::vector<int>;
using OctopusesTable = std::vector<OctopusesRow>;
using Position = std::pair<std::size_t, std::size_t>;
using Neighbors = std::vector<Position>;

OctopusesRow get_octopuses_row(const std::string& input_string) {
    OctopusesRow octopuses_row;
    octopuses_row.reserve(input_string.size());
    for (const auto octopus : input_string) {
        octopuses_row.push_back(octopus - '0');
    }

    return octopuses_row;
}

OctopusesTable get_octopuses_table(const std::string& input_path) {
    std::ifstream input_stream{input_path};
    std::string input_string;
    OctopusesTable octopuses_table;
    while (std::getline(input_stream, input_string)) {
        octopuses_table.push_back(get_octopuses_row(input_string));
    }

    return octopuses_table;
}

Neighbors get_neighbors(const OctopusesTable& octopuses_table, const Position& position) {
    Neighbors neighbors;
    if (position.first > 0) {
        neighbors.emplace_back(position.first - 1, position.second);

        if (position.second > 0) {
            neighbors.emplace_back(position.first - 1, position.second - 1);
        }

        if (position.second < (octopuses_table.front().size() - 1)) {
            neighbors.emplace_back(position.first - 1, position.second + 1);
        }
    }

    if (position.second > 0) {
        neighbors.emplace_back(position.first, position.second - 1);
    }

    if (position.first < (octopuses_table.size() - 1)) {
        neighbors.emplace_back(position.first + 1, position.second);

        if (position.second > 0) {
            neighbors.emplace_back(position.first + 1, position.second - 1);
        }

        if (position.second < (octopuses_table.front().size() - 1)) {
            neighbors.emplace_back(position.first + 1, position.second + 1);
        }
    }

    if (position.second < (octopuses_table.front().size() - 1)) {
        neighbors.emplace_back(position.first, position.second + 1);
    }

    return neighbors;
}

void move_one_step(OctopusesTable& octopuses_table, const Position& position) {
    ++octopuses_table.at(position.first).at(position.second);
    if (octopuses_table.at(position.first).at(position.second) == 10) {
        const auto neighbors = get_neighbors(octopuses_table, position);
        for (const auto& neighbor : neighbors) {
            if (octopuses_table.at(neighbor.first).at(neighbor.second) < 10) {
                move_one_step(octopuses_table, neighbor);
            }
        }
    }
}

void print_octopuses_table(const OctopusesTable& octopuses_table) {
    for (const auto& row : octopuses_table) {
        for (const auto cell : row) {
            std::cout << cell;
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

std::size_t count_flashes_in_step(OctopusesTable& octopuses_table) {
    std::size_t total_flashes = 0;
    for (std::size_t row = 0; row < octopuses_table.size(); ++row) {
        for (std::size_t column = 0; column < octopuses_table.front().size(); ++column) {
            move_one_step(octopuses_table, {row, column});
        }
    }

    for (std::size_t row = 0; row < octopuses_table.size(); ++row) {
        for (std::size_t column = 0; column < octopuses_table.front().size(); ++column) {
            if (octopuses_table.at(row).at(column) > 9) {
                ++total_flashes;
                octopuses_table.at(row).at(column) = 0;
            }
        }
    }

    return total_flashes;    
}

std::size_t count_flashes_in_steps(std::size_t steps, OctopusesTable octopuses_table) {
    std::size_t total_flashes = 0;
    for (std::size_t step = 0; step < steps; ++step) {
        total_flashes += count_flashes_in_step(octopuses_table);
    }

    return total_flashes;
}

std::size_t get_all_flash_step(OctopusesTable octopuses_table) {
    const auto octopuses_count = octopuses_table.size() * octopuses_table.front().size();
    for (std::size_t step = 0; ; ++step) {
        if (count_flashes_in_step(octopuses_table) == octopuses_count) {
            return step + 1;
        }
    }
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    const auto octopuses_table = get_octopuses_table(input_path);
    
    const auto flashes_count = count_flashes_in_steps(100, octopuses_table);
    std::cout << "flashes count " << flashes_count << std::endl;

    const auto all_flash_step = get_all_flash_step(octopuses_table);
    std::cout << "all flash step " << all_flash_step << std::endl;
    return 0;
}
