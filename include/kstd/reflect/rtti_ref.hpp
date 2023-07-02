/**
 * @author Alexander Hinze
 * @since 23/03/2023
 */

#pragma once

#include "reflection_fwd.hpp"
#include "rtti.hpp"

namespace kstd::reflect {
    class RTTIRef final {
        const RTTI& value;

        public:
        RTTIRef(const RTTI& ref) noexcept :// NOLINT - allow implicit conversions
                value(ref) {
        }

        [[nodiscard]] inline operator const RTTI&() const noexcept {// NOLINT - allow implicit conversions
            return value;
        }

        [[nodiscard]] inline auto operator*() const noexcept -> const RTTI& {
            return value;
        }

        [[nodiscard]] inline auto operator==(const RTTIRef& other) const noexcept -> bool {
            return other.value == value;
        }
    };
}// namespace kstd::reflect

// Standard hash implementation to make all RTTI types hashable
template<>
struct std::hash<kstd::reflect::RTTIRef> final {
    auto operator()(kstd::reflect::RTTIRef const& value) const noexcept -> std::size_t {
        return std::hash<std::string>()((*value).to_string());
    }
};