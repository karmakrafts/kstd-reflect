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
#include <string>

#include "reflection_fwd.hpp"
#include "rtti.hpp"

namespace kstd::reflect {
    template<typename T>
    class TypeInfo : public RTTI {
        protected:
        std::string _mangled_type_name;// NOLINT
        std::string _type_name;        // NOLINT

        public:
        KSTD_DEFAULT_MOVE_COPY(TypeInfo)

        TypeInfo(std::string mangled_type_name, std::string type_name) noexcept :
                _mangled_type_name(std::move(mangled_type_name)),
                _type_name(std::move(type_name)) {
        }

        ~TypeInfo() noexcept override = default;

        [[nodiscard]] auto get_mangled_type_name() const noexcept -> const std::string& final {
            return _mangled_type_name;
        }

        [[nodiscard]] auto get_type_name() const noexcept -> const std::string& final {
            return _type_name;
        }

        [[nodiscard]] auto get_element_type() const noexcept -> ElementType override {
            return ElementType::TYPE;
        }

        [[nodiscard]] auto is_same(const RTTI& other) const noexcept -> bool override {
            return other.get_element_type() == ElementType::TYPE && other.get_mangled_type_name() == _mangled_type_name;
        }

        [[nodiscard]] inline constexpr auto is_primitive() const noexcept -> bool {
            return std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_same_v<T, bool>;
        }

        [[nodiscard]] inline constexpr auto get_size() const noexcept -> usize {
            return sizeof(T);
        }

        [[nodiscard]] inline constexpr auto get_alignment() const noexcept -> usize {
            return alignof(T);
        }

        [[nodiscard, maybe_unused]] inline constexpr auto is_default_constructible() const noexcept -> bool {
            return std::is_default_constructible_v<T>;
        }

        [[nodiscard]] inline constexpr auto is_destructible() const noexcept -> bool {
            return std::is_destructible_v<T>;
        }

        template<typename... ARGS>
        [[nodiscard, maybe_unused]] inline constexpr auto is_constructible() const noexcept -> bool {
            return std::is_constructible_v<T, ARGS...>;
        }

        template<typename S>
        [[nodiscard, maybe_unused]] inline constexpr auto is_sub_type() const noexcept -> bool {
            return std::is_base_of_v<S, T>;
        }

        template<typename S>
        [[nodiscard, maybe_unused]] inline constexpr auto is_sub_type(const TypeInfo<S>&) const noexcept// NOLINT
                -> bool {
            return std::is_base_of_v<S, T>;
        }

        template<typename S>
        [[nodiscard, maybe_unused]] inline constexpr auto is_super_type() const noexcept -> bool {
            return std::is_base_of_v<T, S>;
        }

        template<typename S>
        [[nodiscard, maybe_unused]] inline constexpr auto is_super_type(const TypeInfo<S>&) const noexcept// NOLINT
                -> bool {
            return std::is_base_of_v<T, S>;
        }
    };
}// namespace kstd::reflect