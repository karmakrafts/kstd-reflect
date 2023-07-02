/**
 * @author Alexander Hinze
 * @since 23/03/2023
 */

#pragma once

#include <kstd/types.hpp>

namespace kstd::reflect {
    enum class ElementType : u8 {
        TYPE,
        VARIABLE,
        FIELD,
        FUNCTION,
        MEMBER_FUNCTION
    };
}// namespace kstd::reflect