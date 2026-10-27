#pragma once

#include <algorithm>
#include <string_view>
#include <meta>
#include <type_traits>
#include <vector>

namespace struct_reordering {

inline constexpr std::string_view version = "0.1.0";

namespace detail {

template<typename T>
consteval std::meta::info make_optimized_type()
{
    struct Result;
    consteval {
        constexpr auto context = std::meta::access_context::unchecked();
        static_assert(std::meta::bases_of(^^T, context).empty(),
                      "optimized_t requires an aggregate without base classes");
        auto members = std::meta::nonstatic_data_members_of(^^T, context);
        for (auto member : members) {
            if (std::meta::is_bit_field(member)) {
                throw "optimized_t does not support bit-fields";
            }
            if (std::meta::is_mutable_member(member)) {
                throw "optimized_t does not support mutable members";
            }
        }

        // Sort ordinary values: GCC's algorithms cannot sort reflection values directly.
        struct MemberOrder {
            std::size_t alignment;
            std::size_t index;
        };
        std::vector<MemberOrder> order;
        for (std::size_t i = 0; i < members.size(); ++i) {
            order.push_back({std::meta::alignment_of(members[i]), i});
        }
        std::sort(order.begin(), order.end(), [](auto lhs, auto rhs) {
            return lhs.alignment != rhs.alignment
                ? lhs.alignment > rhs.alignment
                : lhs.index < rhs.index;
        });

        std::vector<std::meta::info> specifications;
        for (auto entry : order) {
            auto member = members[entry.index];
            specifications.push_back(std::meta::data_member_spec(
                std::meta::type_of(member),
                {.name = std::meta::identifier_of(member),
                 .alignment = static_cast<int>(std::meta::alignment_of(member))}));
        }
        std::meta::define_aggregate(^^Result, specifications);
    }
    return ^^Result;
}

} // namespace detail


template<typename T>
requires (std::is_class_v<T> && std::is_aggregate_v<T>)
using optimized_t = [:detail::make_optimized_type<std::remove_cv_t<T>>() :];

} // namespace struct_reordering
