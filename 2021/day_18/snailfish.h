#include <string>

class SnailfishNumber {
public:
    using Magnitude = unsigned long long;

    explicit SnailfishNumber(std::string number);
    
    void reduce();
    bool explode();
    bool split();
    Magnitude get_magnitude() const;

    const std::string& get_number() const { return number_; }

private:
    std::size_t get_explode_open_token_position() const;
    std::size_t get_explode_left_target_position(std::size_t explode_position) const;
    std::size_t get_explode_right_target_position(std::size_t separator_position) const;
    void add_value_to_number_at(std::size_t position, int value);
    int get_explode_left_value(std::size_t separator_position) const;
    int get_explode_right_value(std::size_t separator_position) const;
    void explode_left_target(std::size_t explode_position, int value);
    void explode_right_target(std::size_t separator_position);

    bool is_digit(std::size_t pos) const;
    std::size_t get_number_digits_count_at(std::size_t position) const;

    std::size_t get_split_position() const;

    std::string number_;
};

SnailfishNumber operator+(const SnailfishNumber& left, const SnailfishNumber& right);
