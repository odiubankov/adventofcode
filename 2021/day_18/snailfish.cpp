#include "snailfish.h"

#include <sstream>
#include <stack>
#include <limits>

namespace {
using MagnitudeStack = std::stack<SnailfishNumber::Magnitude>;

static constexpr auto open_token = '[';
static constexpr auto close_token = ']';
static constexpr auto separator = ',';
static constexpr std::size_t explode_count = 5;
static constexpr std::size_t split_number_length = 2;

}

SnailfishNumber::SnailfishNumber(std::string number)
: number_{std::move(number)}
{}

std::size_t SnailfishNumber::get_explode_open_token_position() const {
    std::size_t open_count = 0;
    for (std::size_t pos = 0; pos < number_.size(); ++pos) {
        if (number_.at(pos) == open_token) {
            ++open_count;
        } else if (number_.at(pos) == close_token) {
            --open_count;
        }

        if (open_count == explode_count) {
            return pos;
        }
    }

    return std::string::npos;
}

std::size_t SnailfishNumber::get_explode_left_target_position(std::size_t explode_position) const {
    for (std::size_t pos = explode_position - 1; pos > 0; --pos) {
        if (is_digit(pos)) {
            while (is_digit(pos - 1)) {
                --pos;
            }

            return pos;
        }
    }

    return std::string::npos;
}

std::size_t SnailfishNumber::get_explode_right_target_position(std::size_t separator_position) const {
    for (std::size_t pos = number_.find(close_token, separator_position); pos < number_.size(); ++pos) {
        if (is_digit(pos)) {
            return pos;
        }
    }

    return std::string::npos;
}

std::size_t SnailfishNumber::get_number_digits_count_at(std::size_t position) const {
    std::size_t length = 1;
    while (is_digit(position + length)) {
        ++length;
    }

    return length;
}

void SnailfishNumber::add_value_to_number_at(std::size_t position, int value) {
    const auto digits_count = get_number_digits_count_at(position);
    const auto number_str = number_.substr(position, digits_count);
    const auto number = std::stoi(number_str);
    std::stringstream new_number;
    new_number << number_.substr(0, position);
    new_number << number + value;
    new_number << number_.substr(position + digits_count);
    number_ = new_number.str();
}

bool SnailfishNumber::is_digit(std::size_t pos) const {
    return number_.at(pos) >= '0' && number_.at(pos) <= '9';
}

void SnailfishNumber::explode_right_target(std::size_t separator_position) {
    const auto right_number_position = get_explode_right_target_position(separator_position);
    if (right_number_position != std::string::npos) {
        const auto explode_right = get_explode_right_value(separator_position);
        add_value_to_number_at(right_number_position, explode_right);
    }
}

int SnailfishNumber::get_explode_right_value(std::size_t separator_position) const {
    const auto explode_right_start = separator_position + 1;
    const auto right_length = get_number_digits_count_at(explode_right_start);
    const auto explode_right_str = number_.substr(explode_right_start, right_length);
    return std::stoi(explode_right_str);
}

int SnailfishNumber::get_explode_left_value(std::size_t separator_position) const {
    std::size_t start_position = separator_position - 1;
    while (is_digit(start_position - 1)) {
        --start_position;
    }
    
    const auto left_val_str = number_.substr(start_position, separator_position - start_position);
    return std::stoi(left_val_str);
}

void SnailfishNumber::explode_left_target(std::size_t explode_position, int value) {
    const auto left_number_position = get_explode_left_target_position(explode_position);
    if (left_number_position != std::string::npos) {
        add_value_to_number_at(left_number_position, value);
    }    
}

bool SnailfishNumber::explode() {
    const auto explode_position = get_explode_open_token_position();
    if (explode_position == std::string::npos) {
        return false;
    }

    const auto separator_position = number_.find(separator, explode_position);
    explode_right_target(separator_position);
    const auto explode_left = get_explode_left_value(separator_position);

    const auto explode_end_pos = number_.find(close_token, explode_position);
    std::stringstream new_number;
    new_number << number_.substr(0, explode_position);
    new_number << "0";
    new_number << number_.substr(explode_end_pos + 1);
    number_ = new_number.str();

    explode_left_target(explode_position, explode_left);
    return true;
}

bool SnailfishNumber::split() {
    const auto split_position = get_split_position();
    if (split_position == std::string::npos) {
        return false;
    }

    const auto split_number_str = number_.substr(split_position, split_number_length);
    const auto split_number = std::stoi(split_number_str);
    const auto split_left = split_number / 2;
    const auto even = split_number % 2 == 0;
    const auto split_right = split_left + (even ? 0 : 1);
    std::stringstream new_number;
    new_number << number_.substr(0, split_position);
    new_number << open_token << split_left << separator << split_right << close_token;
    new_number << number_.substr(split_position + split_number_length);
    number_ = new_number.str();
    return true;
}

std::size_t SnailfishNumber::get_split_position() const {
    for (std::size_t pos = 0; pos < number_.size(); ++pos) {
        if (is_digit(pos) && get_number_digits_count_at(pos) == split_number_length) {
            return pos;
        }
    }

    return std::string::npos;
}

SnailfishNumber operator+(const SnailfishNumber& left, const SnailfishNumber& right) {
    std::stringstream result_number;
    result_number << open_token << left.get_number() << separator << right.get_number() << close_token;
    SnailfishNumber result{result_number.str()};
    return result;
}

void SnailfishNumber::reduce() {
    bool one_action_done = false;
    do {
        one_action_done = explode() || split();
    } while (one_action_done);
}

SnailfishNumber::Magnitude SnailfishNumber::get_magnitude() const {
    MagnitudeStack left, right;
    std::stack<bool> is_left;
    is_left.push(true);
    auto number = std::numeric_limits<Magnitude>::max();
    for (std::size_t pos = 0; pos < number_.size(); ++pos) {
        if (is_digit(pos)) {
            number = static_cast<Magnitude>(number_.at(pos) - '0');
        } else if (number_.at(pos) == separator) {
            if (number != std::numeric_limits<Magnitude>::max()) {
                left.push(number);
                number = std::numeric_limits<Magnitude>::max();
                is_left.pop();
            }

            is_left.push(false);
        } else if (number_.at(pos) == close_token) {
            if (number != std::numeric_limits<Magnitude>::max()) {
                right.push(number);
                number = std::numeric_limits<Magnitude>::max();
                is_left.pop();
            }

            const auto pair_magnitude = left.top() * 3 + right.top() * 2;
            left.pop();
            right.pop();
            if (is_left.top()) {
                left.push(pair_magnitude);
            } else {
                right.push(pair_magnitude);
            }

            is_left.pop();
        } else if (number_.at(pos) == open_token) {
            is_left.push(true);
        }
    }

    return left.top();
}
