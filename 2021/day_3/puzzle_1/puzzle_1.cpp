#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
    auto constexpr SET_BIT = '1';
}

using ReportBitsCount = std::vector<size_t>;

void calculate_set_bits(const std::string& report_line, ReportBitsCount& report_bits_count) {
    for (size_t i = 0; i < report_line.size(); ++i) {
        if (report_line.at(i) == SET_BIT) {
            ++(report_bits_count.at(i));
        }
    }
}

int main(int argc, char *argv[]) {
    const auto diagnostic_report_path = argv[1];
    std::ifstream diagnostic_report{diagnostic_report_path};
    std::string report_line;
    diagnostic_report >> report_line;
    ReportBitsCount report_bits_count(report_line.size(), 0);
    size_t report_lines_cnt = 0;
    do {
        ++report_lines_cnt;
        calculate_set_bits(report_line, report_bits_count);
    } while (diagnostic_report >> report_line);

    int gamma_rate = 0;
    int epsilon_rate = 0;
    for (const auto bits_cnt : report_bits_count) {
        gamma_rate <<= 1;
        epsilon_rate <<= 1;
        if (bits_cnt > (report_lines_cnt - bits_cnt)) {
            gamma_rate |= 1;
        } else {
            epsilon_rate |= 1;
        }
    }

    const auto power_consumption = gamma_rate * epsilon_rate;
    std::cout << "power_consumption is " << power_consumption << std::endl;
    return 0;
}
