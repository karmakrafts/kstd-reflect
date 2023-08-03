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

#include "foo_types.hpp"
#include <gtest/gtest.h>
#include <kstd/reflect/reflection.hpp>

TEST(kstd_reflect, test_global_functions) {
    const auto info = KSTD_LOOKUP_FUN(foo::test_function);

    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::FUNCTION);
    ASSERT_EQ(info->get_name(), "test_function");
    ASSERT_EQ(info->is_primitive(), false);

    ASSERT_EQ(info->get_param<0>(), *KSTD_LOOKUP_TYPE(kstd::i32));
    ASSERT_EQ(info->get_param<1>(), *KSTD_LOOKUP_TYPE(void*));
    ASSERT_EQ(info->get_param<2>(), *KSTD_LOOKUP_TYPE(bool));
    ASSERT_EQ(info->get_return_type(), *KSTD_LOOKUP_TYPE(kstd::u32));
}

TEST(kstd_reflect, test_member_functions) {
    const auto info = KSTD_LOOKUP_FUN_M(foo::TestStruct::member_function);

    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::MEMBER_FUNCTION);
    ASSERT_EQ(info->get_name(), "member_function");
    ASSERT_EQ(info->is_primitive(), false);

    ASSERT_EQ(info->get_param<0>(), *KSTD_LOOKUP_TYPE(void*));
    ASSERT_EQ(info->get_param<1>(), *KSTD_LOOKUP_TYPE(kstd::i32&));
    ASSERT_EQ(info->get_return_type(), *KSTD_LOOKUP_TYPE(kstd::u32));
}