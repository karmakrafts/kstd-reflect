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
#include <kstd/utils.hpp>
#include <string>

namespace kstd::reflect {
    template<typename ET, typename T>
    class FieldInfo : public VariableInfo<T> {
        protected:
        const RTTI* _enclosing_type;// NOLINT
        usize _offset;              // NOLINT

        public:
        KSTD_DEFAULT_MOVE_COPY(FieldInfo)

        FieldInfo(std::string mangled_type_name, std::string type_name, const RTTI* enclosing_type,
                  const std::string_view& name, usize offset) noexcept :
                VariableInfo<T>(utils::move(mangled_type_name), utils::move(type_name), name),
                _enclosing_type(enclosing_type),
                _offset(offset) {
        }

        ~FieldInfo() noexcept override = default;

        [[nodiscard]] auto get_element_type() const noexcept -> ElementType override {
            return ElementType::FIELD;
        }

        [[nodiscard]] auto to_string() const noexcept -> std::string override {
            return fmt::format("{}::{}:{}", _enclosing_type->get_mangled_type_name(), this->_name,
                               this->get_mangled_type_name());
        }

        [[nodiscard]] auto is_same(const RTTI& other) const noexcept -> bool override {
            return other.get_element_type() == ElementType::FIELD && *(other.template as_field<ET, T>()) == *this;
        }

        [[nodiscard]] inline auto get(const void* memory) const noexcept -> const T& {
            return *reinterpret_cast<const T*>(reinterpret_cast<const u8*>(memory) + _offset);// NOLINT
        }

        inline auto set(void* memory, const T& value) const noexcept {
            *reinterpret_cast<T*>(reinterpret_cast<u8*>(memory) + _offset) = value;// NOLINT
        }

        [[nodiscard, maybe_unused]] inline auto get_enclosing_type() const noexcept -> const TypeInfo<ET>& {
            return *reinterpret_cast<const TypeInfo<ET>*>(_enclosing_type);// NOLINT
        }

        [[nodiscard]] inline auto get_offset() const noexcept -> usize {
            return _offset;
        }

        template<typename OET, typename O>
        [[nodiscard]] inline auto operator==(const FieldInfo<OET, O>& other) const noexcept -> bool {
            return _enclosing_type == other._enclosing_type &&
                   this->get_mangled_type_name() == other.get_mangled_type_name() && this->_name == other._name;
        }
    };
}// namespace kstd::reflect