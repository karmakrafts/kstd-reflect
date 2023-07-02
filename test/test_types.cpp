/**
  * @author Alexander Hinze
  * @since 29/12/2022
  */

#include "foo_types.hpp"
#include <gtest/gtest.h>
#include <kstd/reflect/reflection.hpp>

TEST(kstd_reflect, TestTypes) {
    const auto info = KSTD_LOOKUP_TYPE(foo::TestStruct);
    ASSERT_TRUE(info);
    ASSERT_EQ(info->get_element_type(), kstd::reflect::ElementType::TYPE);
    ASSERT_EQ(info->is_primitive(), false);
}