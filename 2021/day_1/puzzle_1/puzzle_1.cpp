#include <string>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
    std::string input_file_path = argv[1];
    std::ifstream in_file{input_file_path};
    int prev_val = 0;
    in_file >> prev_val;
    int current_val = 0;
    int increased_cnt = 0;
    while (in_file >> current_val) {
        if (current_val > prev_val) {
            ++increased_cnt;
        }

        prev_val = current_val;
    }

    std::cout << "Depth increased " << increased_cnt << " times" << std::endl;
    return 0;
}
