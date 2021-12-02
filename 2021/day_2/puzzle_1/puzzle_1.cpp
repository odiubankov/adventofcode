#include <string>
#include <fstream>
#include <iostream>
#include <functional>

namespace {
    auto constexpr Up = "up";
    auto constexpr Down = "down";
    auto constexpr Forward = "forward";
}

using Action = std::function<void(int distance, int& horizontal_pos, int& vertival_pos)>;

void go_up(const int distance, int& /*horizontal_pos*/, int& vertival_pos) {
    vertival_pos -= distance;
}

void go_down(const int distance, int& /*horizontal_pos*/, int& vertival_pos) {
    vertival_pos += distance;
}

void go_forward(const int distance, int& horizontal_pos, int& /*vertival_pos*/) {
    horizontal_pos += distance;
}

Action get_action(const std::string& direction) {
    if (direction == Up) {
        return go_up;
    } else if (direction == Down) {
        return go_down;
    } else if (direction == Forward) {
        return go_forward;
    }

    return nullptr;
}

int main(int argc, char *argv[]) {
    std::string input_file_path = argv[1];
    std::ifstream in_file{input_file_path};
    std::string direction;
    int distance = 0;
    int horizontal_pos = 0;
    int vertival_pos = 0;
    while (in_file >> direction >> distance) {
        const auto action = get_action(direction);
        action(distance, horizontal_pos, vertival_pos);
    }

    std::cout << "horizontal_pos * vertival_pos " << horizontal_pos * vertival_pos << std::endl;
    return 0;
}
