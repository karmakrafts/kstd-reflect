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

#include "foo_types.hpp"
#include <gtest/gtest.h>
#include <kstd/reflect/reflection.hpp>

TEST(kstd_reflect, test_types) {
    const auto info = KSTD_LOOKUP_TYPE(foo::TestStruct);
    ASSERT_TRUE(info);
    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::TYPE);
    ASSERT_EQ(info->is_primitive(), false);
}