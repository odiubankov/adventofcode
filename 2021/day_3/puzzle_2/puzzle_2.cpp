#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

namespace {
    auto constexpr SET_BIT = '1';
    auto constexpr UNSET_BIT = '0';
}

using ReportLines = std::vector<std::string>;

std::size_t calculate_set_bits(const ReportLines& report_lines, const std::size_t position) {
    return std::count_if(begin(report_lines), end(report_lines), 
        [=](const std::string& report_line) {
            return report_line.at(position) == SET_BIT;
        });
}

int get_rating_value(const ReportLines& report_lines, const bool keep_most_common, const bool keep_set_bits_if_equal_count) {
    auto rating_value = report_lines;
    std::size_t position = 0;
    while (rating_value.size() > 1) {
        const auto set_bits_count = calculate_set_bits(rating_value, position);
        const auto set_and_unset_equal_count = ((rating_value.size() - set_bits_count) == set_bits_count);
        const auto more_set_bits = (set_bits_count > (rating_value.size() - set_bits_count));
        auto look_up_bit = SET_BIT;
        if (set_and_unset_equal_count) {
            if (keep_set_bits_if_equal_count) {
                look_up_bit = SET_BIT;
            } else {
                look_up_bit = UNSET_BIT;
            }
        } else {
            if (keep_most_common) {
                if (more_set_bits) {
                    look_up_bit = SET_BIT;
                } else {
                    look_up_bit = UNSET_BIT;
                }
            } else {
                if (more_set_bits) {
                    look_up_bit = UNSET_BIT;
                } else {
                    look_up_bit = SET_BIT;
                }
            }
        }

        ReportLines remaining_values;
        std::copy_if(begin(rating_value), end(rating_value), back_inserter(remaining_values),
            [look_up_bit, position](const std::string& line) {
                return line.at(position) == look_up_bit;
            });
        rating_value = std::move(remaining_values);
        ++position;
    }

    int value = 0;
    for (const auto bit : rating_value.front()) {
        value <<= 1;
        if (bit == SET_BIT) {
            value |= 1;
        }
    }

    return value;
}

int main(int argc, char *argv[]) {
    const auto diagnostic_report_path = argv[1];
    std::ifstream diagnostic_report{diagnostic_report_path};
    std::string report_line;
    ReportLines report_lines;
    while (diagnostic_report >> report_line) {
        report_lines.push_back(report_line);
    };

    auto keep_set_bits_if_equal_count = true;
    auto keep_most_common = true;
    const auto oxygen_generator_rating = get_rating_value(report_lines, keep_most_common, keep_set_bits_if_equal_count);
    
    keep_set_bits_if_equal_count = false;
    keep_most_common = false;
    const auto co2_scrubber_rating = get_rating_value(report_lines, keep_most_common, keep_set_bits_if_equal_count);

    const auto life_support_rating = oxygen_generator_rating * co2_scrubber_rating;
    std::cout << "life_support_rating is: " << life_support_rating << std::endl;
    return 0;
}
