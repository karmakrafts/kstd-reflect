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

#include <fmt/format.h>
#include <kstd/defaults.hpp>
#include <string>

#include "element_type.hpp"
#include "reflection_fwd.hpp"
#include "rtti.hpp"

namespace kstd::reflect {
    template<typename T>
    class VariableInfo : public TypeInfo<T> {
        protected:
        std::string _name;// NOLINT

        [[nodiscard]] inline auto strip_name(const std::string_view& name) noexcept -> std::string {
            std::string result(name);

            if(name.find(".") != std::string::npos) {// Strip field refs
                const auto index = result.find_last_of('.');
                result = result.substr(index + 1);
            }

            if(name.find("::") != std::string::npos) {// Name contains namespace qualifier(s)
                const auto index = result.find_last_of("::");
                result = result.substr(index + 1);// Strip off namespace qualifier(s)
            }

            return result;
        }

        public:
        KSTD_DEFAULT_MOVE_COPY(VariableInfo)

        VariableInfo(std::string mangled_type_name, std::string type_name, const std::string_view& name) noexcept :
                TypeInfo<T>(std::move(mangled_type_name), std::move(type_name)),
                _name(std::move(strip_name(name))) {
        }

        ~VariableInfo() noexcept override = default;

        [[nodiscard]] auto get_element_type() const noexcept -> ElementType override {
            return ElementType::VARIABLE;
        }

        [[nodiscard]] auto to_string() const noexcept -> std::string override {
            return fmt::format("{}:{}", _name, this->get_mangled_type_name());
        }

        [[nodiscard]] auto is_same(const RTTI& other) const noexcept -> bool override {
            return other.get_element_type() == ElementType::VARIABLE && *(other.template as_variable<T>()) == *this;
        }

        [[nodiscard]] inline auto get_name() const noexcept -> const std::string& {
            return _name;
        }

        template<typename O>
        [[nodiscard]] inline auto operator==(const VariableInfo<O>& other) const noexcept -> bool {
            return this->get_mangled_type_name() == other.get_mangled_type_name() && _name == other._name;
        }
    };
}// namespace kstd::reflect