#include <string>
#include <fstream>
#include <array>
#include <numeric>
#include <iostream>

using Lanternfishes = std::array<unsigned long long, 9>;

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    Lanternfishes lanternfishes{};
    while (std::getline(input_stream, input_string, ',')) {
        ++lanternfishes.at(std::stoi(input_string));
    }

    for (int day = 1; day <= 256; ++day) {
        Lanternfishes day_lanternfishes{};
        for (size_t i = 0; i < lanternfishes.size() - 1; ++i) {
            day_lanternfishes.at(i) = lanternfishes.at(i + 1);
        }

        day_lanternfishes.at(6) += lanternfishes.at(0);
        day_lanternfishes.at(8) = lanternfishes.at(0);
        lanternfishes = std::move(day_lanternfishes);
    }

    const auto total_fishes = std::accumulate(begin(lanternfishes), end(lanternfishes), 0ull);
    std::cout << "total fishes: " << total_fishes << std::endl;
    return 0;
}
