#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>

namespace {
using MapRow = std::vector<int>;
using HeightMap = std::vector<MapRow>;
using Neighbor = std::pair<std::size_t, std::size_t>;
using Neighbors = std::vector<Neighbor>;
using TopBasins = std::priority_queue<int, std::vector<int>, std::greater<int>>;

constexpr std::size_t top_basins_count = 3;

MapRow get_map_row(const std::string& input_string) {
    MapRow map_row;
    map_row.reserve(input_string.size());
    for (const auto height : input_string) {
        map_row.push_back(height - '0');
    }

    return map_row;
}

Neighbors get_neighbors(const HeightMap& height_map, std::size_t row, std::size_t column) {
    Neighbors neighbors;
    if (row > 0) {
        neighbors.emplace_back(row - 1, column);
    }

    if (column > 0) {
        neighbors.emplace_back(row, column - 1);
    }

    if (row < (height_map.size() - 1)) {
        neighbors.emplace_back(row + 1, column);
    }

    if (column < (height_map.at(0).size() - 1)) {
        neighbors.emplace_back(row, column + 1);
    }

    return neighbors;
}

int get_total_risk_level(const HeightMap& height_map) {
    int total_risk_level = 0;
    for (std::size_t row = 0; row < height_map.size(); ++row) {
        for (std::size_t column = 0; column < height_map.at(0).size(); ++column) {
            const auto neighbors = get_neighbors(height_map, row, column);
            if (std::all_of(begin(neighbors), end(neighbors), 
                    [&](const Neighbor& neighbor) { 
                        return height_map.at(neighbor.first).at(neighbor.second) > height_map.at(row).at(column);
                })) {
                total_risk_level += (1 + height_map.at(row).at(column));
            }
        }
    }

    return total_risk_level;
}

std::size_t get_basin_size(HeightMap& height_map, std::size_t row, std::size_t column) {
    height_map.at(row).at(column) = 9;
    const auto neighbors = get_neighbors(height_map, row, column);
    std::size_t basin_size = 1;
    for (const auto& neighbor : neighbors) {
        const auto neighbor_height = height_map.at(neighbor.first).at(neighbor.second);
        if (neighbor_height != 9) {
            basin_size += get_basin_size(height_map, neighbor.first, neighbor.second);
        }
    }

    return basin_size;
}

int get_top_basins(HeightMap height_map) {
    TopBasins top_basins;
    for (std::size_t row = 0; row < height_map.size(); ++row) {
        for (std::size_t column = 0; column < height_map.front().size(); ++column) {
            if (height_map.at(row).at(column) != 9) {
                const auto basin_size = get_basin_size(height_map, row, column);
                top_basins.push(basin_size);
                if (top_basins.size() > top_basins_count) {
                    top_basins.pop();
                }
            }
        }
    }

    std::size_t top_basins_product = 1;
    while (!top_basins.empty()) {
        top_basins_product *= top_basins.top();
        top_basins.pop();
    }
    
    return top_basins_product;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    HeightMap height_map;
    while (std::getline(input_stream, input_string)) {
        height_map.push_back(get_map_row(input_string));
    }

    std::cout << "total risk level: " << get_total_risk_level(height_map) << std::endl;
    std::cout << "top basins: " << get_top_basins(height_map) << std::endl;
    return 0;
}
