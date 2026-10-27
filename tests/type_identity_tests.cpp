#include "type_identity.hpp"

#include <iostream>

static_assert(std::is_same_v<SharedOptimized,
                           struct_reordering::optimized_t<SharedData>>);
static_assert(std::is_same_v<SharedOptimized,
                           struct_reordering::optimized_t<const volatile SharedData>>);
struct DifferentData { char tag; double value; int count; };
static_assert(!std::is_same_v<SharedOptimized,
                            struct_reordering::optimized_t<DifferentData>>);

int main()
{
    SharedOptimized value{.value = 3.5, .count = 41, .tag = 'a'};
    modify_from_other_translation_unit(value);
    if (value.value != 3.5 || value.count != 42 || value.tag != 'b') {
        std::cerr << "Generated type is inconsistent across translation units\n";
        return 1;
    }
}
