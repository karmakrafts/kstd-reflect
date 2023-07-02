/**
 * @author Alexander Hinze
 * @since 23/03/2023
 */

#pragma once

namespace kstd::reflect {
    class RTTI;

    template<typename T>
    class TypeInfo;

    template<typename T>
    class VariableInfo;

    template<typename ET, typename T>
    class FieldInfo;

    template<typename R, typename... ARGS>
    class FunctionInfo;

    template<typename ET, typename R, typename... ARGS>
    class MemberFunctionInfo;

    template<typename T>
    [[nodiscard]] inline auto lookup() noexcept -> Result<const TypeInfo<T>&>;
}// namespace kstd::reflect