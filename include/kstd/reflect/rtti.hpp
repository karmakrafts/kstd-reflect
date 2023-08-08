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

#include <kstd/defaults.hpp>
#include <kstd/result.hpp>
#include <string>

#include "element_type.hpp"
#include "reflection_fwd.hpp"

namespace kstd::reflect {
    struct RTTI {
        KSTD_DEFAULT_MOVE_COPY(RTTI, RTTI)

        RTTI() noexcept = default;

        virtual ~RTTI() noexcept = default;

        [[nodiscard]] virtual auto get_mangled_type_name() const noexcept -> const std::string& = 0;

        [[nodiscard, maybe_unused]] virtual auto get_type_name() const noexcept -> const std::string& = 0;

        [[nodiscard]] virtual auto get_element_type() const noexcept -> ElementType = 0;

        [[nodiscard]] virtual auto is_same(const RTTI& other) const noexcept -> bool = 0;

        [[nodiscard]] virtual auto to_string() const noexcept -> std::string {
            return get_mangled_type_name();// Copy
        }

        template<typename T>
        [[nodiscard, maybe_unused]] inline auto as_type() const noexcept -> const TypeInfo<T>& {
            return static_cast<const TypeInfo<T>&>(*this);// NOLINT
        }

        template<typename T>
        [[nodiscard]] inline auto as_variable() const noexcept -> Result<const VariableInfo<T>&> {
            using namespace std::string_literals;
            const auto type = get_element_type();

            if(type != ElementType::VARIABLE && type != ElementType::FIELD) {
                return Error {"Invalid element type"s};
            }

            return static_cast<const VariableInfo<T>&>(*this);// NOLINT
        }

        template<typename ET, typename T>
        [[nodiscard]] inline auto as_field() const noexcept -> Result<const FieldInfo<ET, T>&> {
            using namespace std::string_literals;

            if(get_element_type() != ElementType::FIELD) {
                return Error {"Invalid element type"s};
            }

            return static_cast<const FieldInfo<ET, T>&>(*this);// NOLINT
        }

        template<typename R, typename... ARGS>
        [[nodiscard]] inline auto as_function() const noexcept -> Result<const FunctionInfo<R, ARGS...>&> {
            using namespace std::string_literals;
            const auto type = get_element_type();

            if(type != ElementType::FUNCTION && type != ElementType::MEMBER_FUNCTION) {
                return Error {"Invalid element type"s};
            }

            return static_cast<const FunctionInfo<R, ARGS...>&>(*this);// NOLINT
        }

        template<typename ET, typename R, typename... ARGS>
        [[nodiscard]] inline auto as_member_function() const noexcept
                -> Result<const MemberFunctionInfo<ET, R, ARGS...>&> {
            using namespace std::string_literals;

            if(get_element_type() != ElementType::MEMBER_FUNCTION) {
                return Error {"Invalid element type"s};
            }

            return static_cast<const MemberFunctionInfo<ET, R, ARGS...>&>(*this);// NOLINT
        }

        [[nodiscard]] inline auto operator==(const RTTI& other) const noexcept -> bool {
            return is_same(other);
        }
    };
}// namespace kstd::reflect