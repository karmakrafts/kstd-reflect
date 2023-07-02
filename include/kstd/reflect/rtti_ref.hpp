// Copyright 2023 Karma Krafts & associates
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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