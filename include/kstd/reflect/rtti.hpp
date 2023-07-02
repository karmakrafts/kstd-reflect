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
    class RTTI {
        public:
        KSTD_DEFAULT_MOVE_COPY(RTTI)

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
            return reinterpret_cast<const TypeInfo<T>&>(*this);// NOLINT
        }

        template<typename T>
        [[nodiscard]] inline auto as_variable() const noexcept -> Result<const VariableInfo<T>&> {
            using namespace std::string_view_literals;
            const auto type = get_element_type();

            if(type != ElementType::VARIABLE && type != ElementType::FIELD) {
                return make_error<const VariableInfo<T>&>("Invalid element type"sv);
            }

            return make_ok<const VariableInfo<T>&>(reinterpret_cast<const VariableInfo<T>&>(*this));// NOLINT
        }

        template<typename ET, typename T>
        [[nodiscard]] inline auto as_field() const noexcept -> Result<const FieldInfo<ET, T>&> {
            using namespace std::string_view_literals;

            if(get_element_type() != ElementType::FIELD) {
                return make_error<const FieldInfo<ET, T>&>("Invalid element type"sv);
            }

            return make_ok<const FieldInfo<ET, T>&>(reinterpret_cast<const FieldInfo<ET, T>&>(*this));// NOLINT
        }

        template<typename R, typename... ARGS>
        [[nodiscard]] inline auto as_function() const noexcept -> Result<const FunctionInfo<R, ARGS...>&> {
            using namespace std::string_view_literals;
            const auto type = get_element_type();

            if(type != ElementType::FUNCTION && type != ElementType::MEMBER_FUNCTION) {
                return make_error<const FunctionInfo<R, ARGS...>&>("Invalid element type"sv);
            }

            return make_ok<const FunctionInfo<R, ARGS...>&>(
                    reinterpret_cast<const FunctionInfo<R, ARGS...>&>(*this));// NOLINT
        }

        template<typename ET, typename R, typename... ARGS>
        [[nodiscard]] inline auto as_member_function() const noexcept
                -> Result<const MemberFunctionInfo<ET, R, ARGS...>&> {
            using namespace std::string_view_literals;

            if(get_element_type() != ElementType::MEMBER_FUNCTION) {
                return make_error<const MemberFunctionInfo<ET, R, ARGS...>&>("Invalid element type"sv);
            }

            return make_ok<const MemberFunctionInfo<ET, R, ARGS...>&>(
                    reinterpret_cast<const MemberFunctionInfo<ET, R, ARGS...>&>(*this));// NOLINT
        }

        [[nodiscard]] inline auto operator==(const RTTI& other) const noexcept -> bool {
            return is_same(other);
        }
    };
}// namespace kstd::reflect