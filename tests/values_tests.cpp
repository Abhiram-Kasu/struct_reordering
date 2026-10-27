#include <struct_reordering.hpp>

#include <iostream>
#include <memory>
#include <string>

struct Data {
    char tag;
    std::string text;
    std::unique_ptr<int> pointer;
    const int values[2];
    int& reference;
};
using Reordered = struct_reordering::optimized_t<Data>;
static_assert(std::is_same_v<decltype(Reordered::tag), char>);
static_assert(std::is_same_v<decltype(Reordered::text), std::string>);
static_assert(std::is_same_v<decltype(Reordered::pointer), std::unique_ptr<int>>);
static_assert(std::is_same_v<decltype(Reordered::values), const int[2]>);
static_assert(std::is_same_v<decltype(Reordered::reference), int&>);

int main()
{
    int number = 7;
    Reordered value{
        .text = "hello", .pointer = std::make_unique<int>(42),
        .reference = number, .values = {1, 2}, .tag = 'c'};
    if (value.text != "hello" || *value.pointer != 42 ||
        value.values[0] != 1 || value.values[1] != 2 || value.tag != 'c') {
        std::cerr << "Generated aggregate did not preserve initialized values\n";
        return 1;
    }
    value.reference = 9;
    if (number != 9) {
        std::cerr << "Reference member lost its binding\n";
        return 1;
    }
    auto moved = std::move(value);
    if (value.pointer || !moved.pointer || *moved.pointer != 42 ||
        moved.text != "hello" || &moved.reference != &number) {
        std::cerr << "Generated aggregate did not preserve move semantics\n";
        return 1;
    }
}
