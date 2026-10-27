#include "type_identity.hpp"

void modify_from_other_translation_unit(SharedOptimized& value)
{
    value.count += 1;
    value.tag = 'b';
}
