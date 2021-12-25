#include <iostream>
#include <vector>

namespace {

struct Area {
    int x1 = 0;
    int x2 = 0;
    int y1 = 0;
    int y2 = 0;
};

int get_min_x(const Area& area) {
    int x = 0;
    for (int i = 1;; ++i) {
        x += i;
        if (x >= area.x1 && x <= area.x2) {
            return i;
        }
    }

    return -1;
}

int get_max_height(const Area& area) {
    return (std::abs(area.y1) * (std::abs(area.y1) - 1)) / 2;
}

int get_possibilities_count(const Area& area) {
    const auto min_init_x = get_min_x(area);
    const auto max_init_y = std::abs(area.y1) - 1;
    const auto max_init_x = area.x2;
    const auto min_init_y = area.y1;
    int possibilities_cnt = 0;
    for (int init_x = min_init_x; init_x <= max_init_x; ++init_x) {
        for (int init_y = min_init_y; init_y <= max_init_y; ++init_y) {
            int current_x = init_x;
            int current_y = init_y;
            int x_pos = 0;
            int y_pos = 0;
            while (x_pos <= area.x2 && y_pos >= area.y1) {
                if (x_pos >= area.x1 && y_pos <= area.y2) {
                    ++possibilities_cnt;
                    break;
                }

                x_pos += current_x;
                if (current_x > 0) {
                    --current_x;
                }

                y_pos += current_y;
                --current_y;
            }
        }
    }

    return possibilities_cnt;
}

}

int main() {
    Area area;
    area.x1 = 277;
    area.x2 = 318;
    area.y1 = -92;
    area.y2 = -53;
        
    // area.x1 = 20;
    // area.x2 = 30;
    // area.y1 = -10;
    // area.y2 = -5;

    const auto max_height = get_max_height(area);
    std::cout << "max height " << max_height << std::endl;
    std::cout << "possibilities count " << get_possibilities_count(area) << std::endl;
    return 0;
}
