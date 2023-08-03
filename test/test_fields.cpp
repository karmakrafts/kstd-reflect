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

TEST(kstd_reflect, test_fields_by_ref) {
    foo::TestStruct value {};
    const auto info = KSTD_LOOKUP_FIELD(value, data1);

    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::FIELD);
    ASSERT_EQ(info->get_name(), "data1");
    ASSERT_EQ(info->get_offset(), 0);
    ASSERT_EQ(info->get_size(), sizeof(void*));
    ASSERT_EQ(info->get_alignment(), alignof(void*));
    ASSERT_EQ(info->is_primitive(), false);

    kstd::usize some_value = 1337;
    info->set(&value, &some_value);

    ASSERT_EQ(info->get(&value), &some_value);
}

TEST(kstd_reflect, test_fields_by_type) {
    const auto& info = KSTD_LOOKUP_FIELD_T(foo::TestStruct, data2);

    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::FIELD);
    ASSERT_EQ(info->get_name(), "data2");
    ASSERT_EQ(info->get_offset(), sizeof(void*));
    ASSERT_EQ(info->get_size(), sizeof(void*));
    ASSERT_EQ(info->get_alignment(), alignof(void*));
    ASSERT_EQ(info->is_primitive(), false);

    foo::TestStruct value {};
    kstd::usize some_value = 1337;
    info->set(&value, &some_value);

    ASSERT_EQ(info->get(&value), &some_value);
}