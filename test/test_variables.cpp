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

#include <gtest/gtest.h>
#include <kstd/reflect/reflection.hpp>

TEST(kstd_reflect, test_variables) {
    kstd::usize some_var = 1337;
    const auto info = KSTD_LOOKUP_VAR(some_var);

    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::VARIABLE);
    ASSERT_EQ(info->get_name(), "some_var");
    ASSERT_EQ(info->get_size(), sizeof(kstd::usize));
    ASSERT_EQ(info->get_alignment(), alignof(kstd::usize));
    ASSERT_EQ(info->is_primitive(), true);
}