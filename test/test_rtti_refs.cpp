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
#include <unordered_map>
#include <unordered_set>

TEST(kstd_reflect, test_rtti_ref_set) {
    std::unordered_set<kstd::reflect::RTTIRef> types;

    types.insert(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data1));
    types.insert(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data2));
    types.insert(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value1));
    types.insert(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value2));
    types.insert(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value3));

    ASSERT_EQ(types.size(), 5);
    ASSERT_TRUE(types.find(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data1)) != types.end());
    ASSERT_TRUE(types.find(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data2)) != types.end());
    ASSERT_TRUE(types.find(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value1)) != types.end());
    ASSERT_TRUE(types.find(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value2)) != types.end());
    ASSERT_TRUE(types.find(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value3)) != types.end());
}

TEST(kstd_reflect, test_rtti_ref_map) {
    std::unordered_map<kstd::reflect::RTTIRef, std::string> types;

    types.insert_or_assign(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data1), "foo");
    types.insert_or_assign(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data2), "bar");
    types.insert_or_assign(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value1), "foobar");
    types.insert_or_assign(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value2), "hello");
    types.insert_or_assign(*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value3), "world");

    ASSERT_EQ(types.size(), 5);

    ASSERT_EQ((types[*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data1)]), "foo");
    ASSERT_EQ((types[*KSTD_LOOKUP_FIELD_T(foo::TestStruct, data2)]), "bar");
    ASSERT_EQ((types[*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value1)]), "foobar");
    ASSERT_EQ((types[*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value2)]), "hello");
    ASSERT_EQ((types[*KSTD_LOOKUP_FIELD_T(foo::TestStruct, value3)]), "world");
}