#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <limits>
#include <cmath>

using CrabsCount = std::unordered_map<int, int>;
using AlignCost = unsigned long long;

AlignCost get_const_crab_move_cost(int from, int to) {
    return std::abs(from - to);
}

AlignCost get_linear_crab_move_cost(int from, int to) {
    const auto linear_cost = get_const_crab_move_cost(from, to);
    return (linear_cost * (linear_cost + 1)) / 2;
}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    CrabsCount crabs_count;
    int min_crab_pos = std::numeric_limits<int>::max();
    int max_crab_pos = std::numeric_limits<int>::min();
    while (std::getline(input_stream, input_string, ',')) {
        const auto crab_pos = std::stoi(input_string);
        min_crab_pos = std::min(min_crab_pos, crab_pos);
        max_crab_pos = std::max(max_crab_pos, crab_pos);
        ++crabs_count[crab_pos];
    }

    auto min_align_cost = std::numeric_limits<AlignCost>::max();
    for (int align_to = min_crab_pos; align_to <= max_crab_pos; ++align_to) {
        AlignCost align_cost = 0;
        for (const auto& align_from : crabs_count) {
            const auto one_crab_align_cost = get_linear_crab_move_cost(align_from.first, align_to);
            align_cost += (one_crab_align_cost * align_from.second);
        }

        if (align_cost < min_align_cost) {
            min_align_cost = align_cost;
        } else {
            break;
        }
    }

    std::cout << "alignment cost: " << min_align_cost << std::endl;
    return 0;
}
