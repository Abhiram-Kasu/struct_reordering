#include <struct_reordering.hpp>

#include <cstddef>

using struct_reordering::optimized_t;

struct Padded {
    char tag;
    double value;
    char flag;
    int count;
};
using Reordered = optimized_t<Padded>;
static_assert(std::is_aggregate_v<Reordered>);
static_assert(!std::is_same_v<Padded, Reordered>);
static_assert(std::is_same_v<decltype(Reordered::value), double>);
static_assert(std::is_same_v<decltype(Reordered::count), int>);
static_assert(std::is_same_v<decltype(Reordered::tag), char>);
static_assert(std::is_same_v<decltype(Reordered::flag), char>);
static_assert(offsetof(Reordered, value) == 0);
static_assert(offsetof(Reordered, value) < offsetof(Reordered, count));
static_assert(offsetof(Reordered, count) < offsetof(Reordered, tag));
static_assert(offsetof(Reordered, tag) < offsetof(Reordered, flag));
static_assert(sizeof(Reordered) < sizeof(Padded));

// Member alignment, rather than type alignment or size, controls ordering.
struct ExplicitAlignment {
    double value;
    alignas(32) char tag;
    int count;
};
using Aligned = optimized_t<ExplicitAlignment>;
static_assert(alignof(Aligned) == 32);
static_assert(offsetof(Aligned, tag) == 0);
static_assert(offsetof(Aligned, value) < offsetof(Aligned, count));

// Different sizes with equal alignment must still retain declaration order.
struct Ties {
    char first;
    char second[8];
    char third;
};
using Stable = optimized_t<Ties>;
static_assert(offsetof(Stable, first) < offsetof(Stable, second));
static_assert(offsetof(Stable, second) < offsetof(Stable, third));

struct Sorted { double value; int count; char tag; };
using Unchanged = optimized_t<Sorted>;
static_assert(sizeof(Unchanged) == sizeof(Sorted));
static_assert(offsetof(Unchanged, value) == offsetof(Sorted, value));
static_assert(offsetof(Unchanged, count) == offsetof(Sorted, count));
static_assert(offsetof(Unchanged, tag) == offsetof(Sorted, tag));

struct Empty {};
struct Single { int value; };
static_assert(std::is_empty_v<optimized_t<Empty>>);
static_assert(sizeof(optimized_t<Single>) == sizeof(Single));
static_assert(offsetof(optimized_t<Single>, value) == 0);

union Union { int value; char tag; };
struct NonAggregate { NonAggregate() {} int value; };
template<typename T>
concept CanOptimize = requires { typename optimized_t<T>; };
static_assert(!CanOptimize<int>);
static_assert(!CanOptimize<Padded*>);
static_assert(!CanOptimize<Padded&>);
static_assert(!CanOptimize<Union>);
static_assert(!CanOptimize<NonAggregate>);

int main() {}
