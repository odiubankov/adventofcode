#include <string>
#include <fstream>
#include <iostream>
#include <functional>

namespace {
    auto constexpr Up = "up";
    auto constexpr Down = "down";
    auto constexpr Forward = "forward";
}

struct Position {
    int horizontal = 0;
    int vertical = 0;
    int aim = 0;
};

using Action = std::function<void(int distance, Position& position)>;

void go_up(const int distance, Position& position) {
    position.aim -= distance;
}

void go_down(const int distance, Position& position) {
    position.aim += distance;
}

void go_forward(const int distance, Position& position) {
    position.horizontal += distance;
    position.vertical += (position.aim * distance);
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

    Position position;
    while (in_file >> direction >> distance) {
        const auto action = get_action(direction);
        action(distance, position);
    }

    std::cout << "horizontal_pos * vertival_pos " << position.horizontal * position.vertical << std::endl;
    return 0;
}
