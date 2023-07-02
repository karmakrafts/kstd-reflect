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
 * @since 02/07/2023
 */

#pragma once

#include <kstd/types.hpp>

namespace foo {
    struct TestStruct final {
        [[maybe_unused]] volatile void* data1 = nullptr;
        [[maybe_unused]] volatile void* data2 = nullptr;
        [[maybe_unused]] volatile kstd::u32 value1 = 0;
        [[maybe_unused]] volatile kstd::f32 value2 = 0.0F;
        [[maybe_unused]] volatile std::string value3;

        auto member_function([[maybe_unused]] void* param1, [[maybe_unused]] kstd::i32& param2) const noexcept// NOLINT
                -> kstd::u32;
    };

    auto test_function([[maybe_unused]] kstd::i32 param1, [[maybe_unused]] void* param2,
                       [[maybe_unused]] bool param3) noexcept -> kstd::u32;
}// namespace foo