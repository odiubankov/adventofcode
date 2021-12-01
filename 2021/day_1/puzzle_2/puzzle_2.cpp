#include <string>
#include <fstream>
#include <iostream>
#include <list>

namespace {
    auto constexpr WindowSize = 3;
}

int main(int argc, char *argv[]) {
    std::string input_file_path = argv[1];
    std::ifstream in_file{input_file_path};
    int prev_window_sum = 0;
    std::list<int> window_items;
    int current_val = 0;
    for (int i = 0; i < WindowSize; ++i) {
        in_file >> current_val;
        window_items.push_back(current_val);
        prev_window_sum += current_val;
    }

    int increased_cnt = 0;
    while (in_file >> current_val) {
        auto current_window_sum = prev_window_sum;
        current_window_sum -= window_items.front();
        window_items.pop_front();
        window_items.push_back(current_val);
        current_window_sum += window_items.back();
        if (current_window_sum > prev_window_sum) {
            ++increased_cnt;
        }

        prev_window_sum = current_window_sum;
    }

    std::cout << "Depth increased " << increased_cnt << " times" << std::endl;
    return 0;
}
