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

namespace kstd::reflect {
    struct RTTI;

    template<typename T>
    struct TypeInfo;

    template<typename T>
    struct VariableInfo;

    template<typename ET, typename T>
    struct FieldInfo;

    template<typename R, typename... ARGS>
    struct FunctionInfo;

    template<typename ET, typename R, typename... ARGS>
    struct MemberFunctionInfo;

    template<typename T>
    [[nodiscard]] inline auto lookup() noexcept -> Result<const TypeInfo<T>&>;
}// namespace kstd::reflect