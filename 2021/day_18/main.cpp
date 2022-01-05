#include "2021/day_18/snailfish.h"

#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    std::string input_string;
    std::getline(input_stream, input_string);
    SnailfishNumber all_sum_snailfish_number{input_string};
    std::vector<SnailfishNumber> all_numbers{all_sum_snailfish_number};
    SnailfishNumber::Magnitude max_magnitude = 0;
    while (std::getline(input_stream, input_string)) {
        SnailfishNumber second_snailfish_number{input_string};
        all_sum_snailfish_number = all_sum_snailfish_number + second_snailfish_number;
        all_sum_snailfish_number.reduce();

        for (const auto& number : all_numbers) {
            auto sum_number = number + second_snailfish_number;
            sum_number.reduce();
            max_magnitude = std::max(max_magnitude, sum_number.get_magnitude());

            sum_number = second_snailfish_number + number;
            sum_number.reduce();
            max_magnitude = std::max(max_magnitude, sum_number.get_magnitude());
        }

        all_numbers.push_back(std::move(second_snailfish_number));
    }

    std::cout << "result: " << all_sum_snailfish_number.get_magnitude() << std::endl;
    std::cout << "max 2 sum: " << max_magnitude << std::endl;
    return 0;
}
