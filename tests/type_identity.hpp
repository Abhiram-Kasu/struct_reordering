#pragma once

#include <struct_reordering.hpp>

struct SharedData { char tag; double value; int count; };
using SharedOptimized = struct_reordering::optimized_t<SharedData>;

void modify_from_other_translation_unit(SharedOptimized& value);
