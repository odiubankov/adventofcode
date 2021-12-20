#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

namespace {

enum class PacketType {
    Literal,
    Sum,
    Product,
    Minimum,
    Maximum,
    Greater,
    Less,
    Equal
};

enum class LengthType {
    Length,
    Count
};

struct PacketParseResult {
    std::size_t length = 0;
    unsigned long long version_sum = 0;
    unsigned long long value = 0;
};

using HexToBinary = std::unordered_map<char, std::string>;
using Operands = std::vector<unsigned long long>;
using EvaluationFunction = std::function<unsigned long long(const Operands& operands)>;

unsigned long long sum_operator(const Operands& operands) {
    return std::accumulate(begin(operands), end(operands), 0ull);
}

unsigned long long product_operator(const Operands& operands) {
    return std::accumulate(begin(operands), end(operands), 1ull,
        [](unsigned long long v1, unsigned long long v2) { return v1 * v2; });
}

unsigned long long minimum_operator(const Operands& operands) {
    return *std::min_element(begin(operands), end(operands));
}

unsigned long long maximum_operator(const Operands& operands) {
    return *std::max_element(begin(operands), end(operands));
}

unsigned long long greater_operator(const Operands& operands) {
    return operands.front() > operands.back() ? 1 : 0;
}

unsigned long long less_operator(const Operands& operands) {
    return operands.front() < operands.back() ? 1 : 0;
}

unsigned long long equal_operator(const Operands& operands) {
    return operands.front() == operands.back() ? 1 : 0;
}

EvaluationFunction get_evaluation_function(PacketType packet_type) {
    switch (packet_type) {
        case PacketType::Equal:
            return equal_operator;
        case PacketType::Greater:
            return greater_operator;
        case PacketType::Less:
            return less_operator;
        case PacketType::Maximum:
            return maximum_operator;
        case PacketType::Minimum:
            return minimum_operator;
        case PacketType::Product:
            return product_operator;
        case PacketType::Sum:
            return sum_operator;
    }
}

static const HexToBinary hex_to_binary {
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"}
};

static constexpr std::size_t version_length = 3;
static constexpr std::size_t type_length = 3;
static constexpr std::size_t length_payload_length = 15;
static constexpr std::size_t group_length = 5;
static constexpr std::size_t count_payload_length = 11;
static constexpr auto sum_packet_id = 0ull;
static constexpr auto product_packet_id = 1ull;
static constexpr auto minimum_packet_id = 2ull;
static constexpr auto maximum_packet_id = 3ull;
static constexpr auto liter_packet_id = 4ull;
static constexpr auto greater_packet_id = 5ull;
static constexpr auto less_packet_id = 6ull;
static constexpr auto equal_packet_id = 7ull;
static constexpr auto last_group_flag = '0';
static constexpr auto count_operator_flag = '1';
static constexpr std::size_t length_type_length = 1;

unsigned long long binary_to_decimal(const std::string& binary_str) {
    unsigned long long decimal = 0;
    for (const auto bit : binary_str) {
        decimal <<= 1;
        if (bit == '1') {
            decimal |= 1;
        }
    }

    return decimal;
}

PacketType get_packet_type(const std::string& type_str) {
    const auto type_number = binary_to_decimal(type_str);
    switch (type_number) {
        case liter_packet_id:
            return PacketType::Literal;
        case  sum_packet_id:
            return PacketType::Sum;
        case product_packet_id:
            return PacketType::Product;
        case minimum_packet_id:
            return PacketType::Minimum;
        case maximum_packet_id:
            return PacketType::Maximum;
        case greater_packet_id:
            return PacketType::Greater;
        case less_packet_id:
            return PacketType::Less;
        case equal_packet_id:
            return PacketType::Equal;
    }

    return PacketType::Literal;
}

PacketParseResult parse_literal_packet(const std::string& packet_str) {
    PacketParseResult result;
    std::stringstream value_str;
    result.length = version_length + type_length;
    do {
        value_str << packet_str.substr(result.length + 1, group_length - 1);
        result.length += group_length;
    } while (packet_str.at(result.length - group_length) != last_group_flag);
    result.value = binary_to_decimal(value_str.str());
    return result;
}

LengthType get_length_type(char type) {
    if (type == count_operator_flag) {
        return LengthType::Count;
    }

    return LengthType::Length;
}

PacketParseResult parse_packet(const std::string& packet_str);

PacketParseResult parse_max_length_operator_packet(const std::string& packet_str, PacketType packet_type) {
    const auto max_length_str = packet_str.substr(version_length + type_length + length_type_length, length_payload_length);
    const auto max_length = binary_to_decimal(max_length_str);

    auto start_pos = version_length + type_length + length_type_length + length_payload_length;
    PacketParseResult result;
    result.length = version_length + type_length + length_type_length + length_payload_length + max_length;
    Operands operands;
    do {
        const auto sub_packet_str = packet_str.substr(start_pos);
        const auto sub_packet_result = parse_packet(sub_packet_str);
        result.version_sum += sub_packet_result.version_sum;
        start_pos += sub_packet_result.length;
        operands.push_back(sub_packet_result.value);
    } while (start_pos < result.length);
    const auto evaluation_function = get_evaluation_function(packet_type);
    result.value = evaluation_function(operands);
    return result;
}

PacketParseResult parse_count_operator_packet(const std::string& packet_str, PacketType packet_type) {
    const auto count_str = packet_str.substr(version_length + type_length + length_type_length, count_payload_length);
    const auto count = binary_to_decimal(count_str);
    PacketParseResult result;
    result.length = version_length + type_length + length_type_length + count_payload_length;
    Operands operands;
    for (auto i = 0ull; i < count; ++i) {
        const auto sub_packet_str = packet_str.substr(result.length);
        const auto sub_packet_result = parse_packet(sub_packet_str);
        result.length += sub_packet_result.length;
        result.version_sum += sub_packet_result.version_sum;
        operands.push_back(sub_packet_result.value);
    }
    const auto evaluation_function = get_evaluation_function(packet_type);
    result.value = evaluation_function(operands);
    return result;
}

PacketParseResult parse_operator_packet(const std::string& packet_str, PacketType packet_type) {
    const auto length_type = get_length_type(packet_str.at(version_length + type_length));
    if (length_type == LengthType::Length) {
        return parse_max_length_operator_packet(packet_str, packet_type);
    } else {
        return parse_count_operator_packet(packet_str, packet_type);
    }
}

PacketParseResult parse_packet(const std::string& packet_str) {
    if (packet_str.size() < (version_length + type_length + group_length)) {
        return {};
    }

    const auto  version_str = packet_str.substr(0, version_length);
    const auto version = binary_to_decimal(version_str);

    const auto type_str = packet_str.substr(version_length, type_length);
    const auto packet_type = get_packet_type(type_str);
    if (packet_type == PacketType::Literal) {
        auto result = parse_literal_packet(packet_str);
        result.version_sum = version;
        return result;
    }

    auto result = parse_operator_packet(packet_str, packet_type);
    result.version_sum += version;
    return result;
}

void parse_hex_packet(const std::string& packet_str) {
    std::stringstream ss;
    for (const auto character : packet_str) {
        ss << hex_to_binary.at(character);
    }

    const auto binary_string = ss.str();
    const auto result = parse_packet(ss.str());
    std::cout << "version sum: " << result.version_sum << std::endl;
    std::cout << "evaluation result: " << result.value << std::endl;
}

}

int main(int argc, char *argv[]) {
    const std::string input_path = argv[1];
    std::ifstream input_stream{input_path};
    auto version_sum = 0ull;
    std::string input_string;
    std::getline(input_stream, input_string);
    parse_hex_packet(input_string);
    return 0;
}
