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
#include <kstd/pack.hpp>
#include <kstd/utils.hpp>
#include <string>

#include "element_type.hpp"
#include "reflection_fwd.hpp"

namespace kstd::reflect {
    template<typename ET, typename R, typename... ARGS>
    struct MemberFunctionInfo : public FunctionInfo<R, ARGS...> {
        using EnclosingType = ET;
        using ReturnType = R;
        using ParameterTypes [[maybe_unused]] = Pack<ARGS...>;

        private:
        using Self = MemberFunctionInfo<EnclosingType, ReturnType, ARGS...>;

        protected:
        const RTTI* _enclosing_type;// NOLINT

        public:
        KSTD_DEFAULT_MOVE_COPY(MemberFunctionInfo, Self)

        MemberFunctionInfo(std::string mangled_type_name, std::string type_name, const RTTI* enclosing_type,
                           const std::string& name) noexcept :
                FunctionInfo<R, ARGS...>(std::move(mangled_type_name), std::move(type_name), name),
                _enclosing_type {enclosing_type} {
        }

        ~MemberFunctionInfo() noexcept override = default;

        [[nodiscard]] auto get_element_type() const noexcept -> ElementType override {
            return ElementType::MEMBER_FUNCTION;
        }

        [[nodiscard]] auto to_string() const noexcept -> std::string override {
            return fmt::format("{}::{}:{}", _enclosing_type->get_mangled_type_name(), this->_name,
                               this->get_mangled_type_name());
        }

        [[nodiscard]] auto is_same(const RTTI& other) const noexcept -> bool override {
            return other.get_element_type() == ElementType::MEMBER_FUNCTION &&
                   *(other.template as_member_function<EnclosingType, ReturnType, ARGS...>()) == *this;
        }

        [[nodiscard, maybe_unused]] inline auto get_enclosing_type() const noexcept -> const TypeInfo<EnclosingType>& {
            return *reinterpret_cast<const TypeInfo<EnclosingType>*>(_enclosing_type);// NOLINT
        }

        template<typename OET, typename OR, typename... OARGS>
        [[nodiscard]] inline auto operator==(const MemberFunctionInfo<OET, OR, OARGS...>& other) const noexcept
                -> bool {
            return _enclosing_type == other._enclosing_type &&
                   this->get_mangled_type_name() == other.get_mangled_type_name() && this->_name == other._name;
        }
    };
}// namespace kstd::reflect