///////////////////////////////////////////////////////////////////////////////
//
// has_remove.hpp
//
/////////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef HAS_REMOVE_EN_14_12_2004
#define HAS_REMOVE_EN_14_12_2004

#include "./is_std_list.hpp"

namespace boost
{
    // Specialize this for user-defined types
    template<typename T>
    struct has_remove
        : is_std_list<T>
    {
    };
}

#endif
