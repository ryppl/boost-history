// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_FAT_POINTER_HPP_INCLUDED
#define BOOST_IDL_DETAIL_FAT_POINTER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace interfaces { namespace detail {

// Used to cirumvent access control. Needed only by INTERFACE_END.
// A solution not using reinterpret_cast caused some interface 
// instance bloat on VC and Intel; use of reinterpret_cast can
// be thought of as a non-portable optimization.
struct fat_pointer {
    fat_pointer() : pv(0), table(0) { }
    template<typename T>
    fat_pointer(const T& t) 
        : pv(reinterpret_cast<const fat_pointer&>(t).pv),
          table(reinterpret_cast<const fat_pointer&>(t).table)
        { }
    void*    pv;
    fn_ptr*  table;
};

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_FAT_POINTER_HPP_INCLUDED
