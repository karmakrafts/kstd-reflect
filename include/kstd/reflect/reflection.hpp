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
  * @since 29/12/2022
  */

#pragma once

#include <kstd/result.hpp>
#include <kstd/types.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>

#include <fmt/format.h>

#ifndef COMPILER_MSVC
#include <cxxabi.h>
#endif

#include "element_type.hpp"
#include "field_info.hpp"
#include "function_info.hpp"
#include "member_function_info.hpp"
#include "reflection_fwd.hpp"
#include "rtti.hpp"
#include "rtti_ref.hpp"
#include "type_info.hpp"
#include "variable_info.hpp"

// Expressions
#define KSTD_LOOKUP_EXPR(e) kstd::reflect::lookup(e)
// Types
#define KSTD_LOOKUP_TYPE(t) kstd::reflect::lookup<t>()
// Variables
#define KSTD_LOOKUP_VAR(v) kstd::reflect::lookup_variable(v, #v)
// Member functions
#define KSTD_LOOKUP_FUN_M(f) kstd::reflect::lookup_function(&f, #f)
// Global functions
#define KSTD_LOOKUP_FUN(f) kstd::reflect::lookup_function(f, #f)
// Fields by reference
#define KSTD_LOOKUP_FIELD(r, f) kstd::reflect::lookup_field<decltype(r)>(r.f, #f, offsetof(decltype(r), f))
// Fields by type
#define KSTD_LOOKUP_FIELD_T(t, f) kstd::reflect::lookup_field<t, decltype(t::f)>(#f, offsetof(t, f))

namespace kstd::reflect {
    namespace {
        template<typename T>
        [[nodiscard]] inline auto get_mangled_type_name() noexcept -> std::string {
#ifdef COMPILER_MSVC
            return typeid(T).raw_name();// MSVC provides its own field for the mangled name
#else
            return typeid(T).name();
#endif
        }

        template<typename T>
        [[nodiscard]] inline auto get_type_name() noexcept -> Result<std::string> {
            using namespace std::string_view_literals;

#ifdef COMPILER_MSVC
            return typeid(T).name();// MSVC uses the regular name field for the unmangled name
#else
            i32 status = 0;
            const auto mangled_name = get_mangled_type_name<T>();
            auto* ptr = abi::__cxa_demangle(mangled_name.c_str(), nullptr, nullptr, &status);

            switch(status) {
                case -1: return make_error<std::string>("Could not allocate memory to demangle"sv);
                case -2: return make_error<std::string>("Not a valid mangled name"sv);
                case -3: return make_error<std::string>("Invalid argument"sv);
                default: break;
            }

            auto result = std::string(ptr);
            free(ptr);             // Since __cxa_demangle malloc's it's result, we need to free it
            return make_ok(result);// MoR
#endif
        }

        std::unordered_map<std::string, std::unique_ptr<RTTI>> _types;// NOLINT

        template<typename T, typename RI, typename... ARGS>
        [[nodiscard]] inline auto lookup_named(const std::string& key, ARGS&&... args) noexcept -> Result<const RI&> {
            static_assert(std::is_convertible_v<RI*, RTTI*>, "Pointer types are not polymorphically convertible");

            if(_types.find(key) == _types.end()) {
                auto result = get_type_name<T>();

                if(!result) {
                    return result.template forward_error<const RI&>();
                }

                _types[key] = std::make_unique<RI>(get_mangled_type_name<T>(), *result, std::forward<ARGS>(args)...);
            }

            return make_ok<const RI&>(*reinterpret_cast<const RI*>(_types[key].get()));// NOLINT
        }

        template<typename T, typename RI, typename... ARGS>
        [[nodiscard]] inline auto lookup_default(ARGS&&... args) noexcept -> Result<const RI&> {
            static_assert(std::is_convertible_v<RI*, RTTI*>, "Pointer types are not polymorphically convertible");
            return lookup_named<T, RI, ARGS...>(get_mangled_type_name<T>(), std::forward<ARGS>(args)...);
        }
    }// namespace

    template<typename T>
    [[nodiscard]] inline auto lookup() noexcept -> Result<const TypeInfo<T>&> {
        return lookup_default<T, TypeInfo<T>>();
    }

    template<typename T>
    [[nodiscard]] inline auto lookup(T*) noexcept -> Result<const TypeInfo<T>&> {// NOLINT
        return lookup_default<T, TypeInfo<T>>();
    }

    template<typename T>
    [[nodiscard]] inline auto lookup(T&) noexcept -> Result<const TypeInfo<T>&> {// NOLINT
        return lookup_default<T, TypeInfo<T>>();
    }

    template<typename T>
    [[nodiscard]] inline auto lookup(T&&) noexcept -> Result<const TypeInfo<T>&> {// NOLINT
        return lookup_default<T, TypeInfo<T>>();
    }

    template<typename R, typename... ARGS>
    [[nodiscard]] inline auto lookup_function(R(ARGS...), const std::string_view& name) noexcept// NOLINT
            -> Result<const FunctionInfo<R, ARGS...>&> {
        const auto key = fmt::format("{}({})", name, get_mangled_type_name<R(ARGS...)>());
        return lookup_named<R(ARGS...), FunctionInfo<R, ARGS...>>(key, name);
    }

    template<typename ET, typename R, typename... ARGS>
    [[nodiscard]] inline auto lookup_function(R (ET::*)(ARGS...), const std::string_view& name) noexcept// NOLINT
            -> Result<const MemberFunctionInfo<ET, R, ARGS...>&> {
        const auto enclosing_type_result = lookup<ET>();

        if(!enclosing_type_result) {
            return enclosing_type_result.template forward_error<const MemberFunctionInfo<ET, R, ARGS...>&>();
        }

        const auto key = fmt::format("{}::{}({})", enclosing_type_result->get_mangled_type_name(), name,
                                     get_mangled_type_name<R (ET::*)(ARGS...) const>());
        return lookup_named<R (ET::*)(ARGS...), MemberFunctionInfo<ET, R, ARGS...>>(
                key, &enclosing_type_result.borrow(), name);
    }

    template<typename ET, typename R, typename... ARGS>
    [[nodiscard]] inline auto lookup_function(R (ET::*)(ARGS...) const, const std::string_view& name) noexcept// NOLINT
            -> Result<const MemberFunctionInfo<ET, R, ARGS...>&> {
        const auto enclosing_type_result = lookup<ET>();

        if(!enclosing_type_result) {
            return enclosing_type_result.template forward_error<const MemberFunctionInfo<ET, R, ARGS...>&>();
        }

        const auto key = fmt::format("{}::{}({})", enclosing_type_result->get_mangled_type_name(), name,
                                     get_mangled_type_name<R (ET::*)(ARGS...) const>());
        return lookup_named<R (ET::*)(ARGS...) const, MemberFunctionInfo<ET, R, ARGS...>>(
                key, &enclosing_type_result.borrow(), name);
    }

    template<typename T>
    [[nodiscard]] inline auto lookup_variable(T&, const std::string_view& name) noexcept// NOLINT
            -> Result<const VariableInfo<T>&> {
        return lookup_named<T, VariableInfo<T>>(std::string(name), name);
    }

    template<typename ET, typename T>
    [[nodiscard]] inline auto lookup_field(T&, const std::string_view& name, usize offset) noexcept// NOLINT
            -> Result<const FieldInfo<ET, T>&> {
        const auto enclosing_type_result = lookup<ET>();

        if(!enclosing_type_result) {
            return enclosing_type_result.template forward_error<const FieldInfo<ET, T>&>();
        }

        const auto key = fmt::format("{}::{}", enclosing_type_result->get_mangled_type_name(), name);
        return lookup_named<T, FieldInfo<ET, T>>(key, &enclosing_type_result.borrow(), name, offset);
    }

    template<typename ET, typename T>
    [[nodiscard]] inline auto lookup_field(const std::string_view& name, usize offset) noexcept
            -> Result<const FieldInfo<ET, T>&> {
        const auto enclosing_type_result = lookup<ET>();

        if(!enclosing_type_result) {
            return enclosing_type_result.template forward_error<const FieldInfo<ET, T>&>();
        }

        const auto key = fmt::format("{}::{}", enclosing_type_result->get_mangled_type_name(), name);
        return lookup_named<T, FieldInfo<ET, T>>(key, &enclosing_type_result.borrow(), name, offset);
    }

    // Reflective instantiation

    template<typename T, typename... ARGS>
    [[nodiscard]] inline auto make(const TypeInfo<T>&, ARGS&&... args) noexcept -> T {// NOLINT
        static_assert(std::is_constructible_v<T, ARGS...>, "Type is not constructible with given arguments");
        return T {std::forward<ARGS>(args)...};
    }

    template<typename T, typename... ARGS>
    [[nodiscard]] inline auto make_shared(const TypeInfo<T>&, ARGS&&... args) noexcept -> std::shared_ptr<T> {// NOLINT
        static_assert(std::is_constructible_v<T, ARGS...>, "Type is not constructible with given arguments");
        return std::make_shared<T, ARGS...>(std::forward<ARGS>(args)...);
    }

    template<typename T, typename... ARGS>
    [[nodiscard]] inline auto make_unique(const TypeInfo<T>&, ARGS&&... args) noexcept -> std::unique_ptr<T> {// NOLINT
        static_assert(std::is_constructible_v<T, ARGS...>, "Type is not constructible with given arguments");
        return std::make_unique<T, ARGS...>(std::forward<ARGS>(args)...);
    }
}// namespace kstd::reflect