//////////////////////////////////////////////////////////////////////////////
//
//  std_ptrs.hpp
//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) David Held 2002.
//
//  Permission to copy, use, modify, sell and distribute this software is
//  granted provided this  copyright notice  appears in all copies.  This
//  software is provided "as is" without express or implied warranty, and
//  with no claim as to its suitability for any purpose.
//
//////////////////////////////////////////////////////////////////////////////
//
//  Inspired by Andrei Alexandrescu
//
//////////////////////////////////////////////////////////////////////////////

#ifndef STD_PTRS_HPP
#define STD_PTRS_HPP

#include "mi_smart_ptr.hpp"

//////////////////////////////////////////////////////////////////////////////
// Usage:
//   ptr<Widget>::std_auto
//   ptr<Widget>::shared
//   ptr<Widget>::intrusive
//////////////////////////////////////////////////////////////////////////////

namespace boost
{
    template <typename T>
    struct ptr
    {

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef T* raw;
        typedef smart_ptr<T,
            destructive_copy, disallow_conversion, assert_check
        > std_auto;

        typedef smart_ptr<T,
            no_copy, disallow_conversion, assert_check, default_storage
        > scoped;
        typedef smart_ptr<T,
            boost_ref, disallow_conversion, assert_check, shared_storage
        > shared;
        typedef smart_ptr<T,
            boost_ref, disallow_conversion, assert_check, weak_storage
        > weak;
        typedef smart_ptr<T,
            boost_ref, disallow_conversion, assert_check, intrusive_storage
        > intrusive;

        typedef smart_ptr<T,
            no_copy, disallow_conversion, assert_check, array_storage
        > scoped_array;
        // Need shared_array
        // weak_array? intrusive_array?

        typedef smart_ptr<T,
            com_ref_counted, disallow_conversion, assert_check
        > com;

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef T* raw;
        typedef BOOST_SMART_PTR(T,
            destructive_copy, disallow_conversion, assert_check
        ) std_auto;

        typedef BOOST_SMART_PTR(T,
            no_copy, disallow_conversion, assert_check, default_storage
        ) scoped;
        typedef BOOST_SMART_PTR(T,
            boost_ref, disallow_conversion, assert_check, shared_storage
        ) shared;
        typedef BOOST_SMART_PTR(T,
            boost_ref, disallow_conversion, assert_check, weak_storage
        ) weak;
        typedef BOOST_SMART_PTR(T,
            boost_ref, disallow_conversion, assert_check, intrusive_storage
        ) intrusive;

        typedef BOOST_SMART_PTR(T,
            no_copy, disallow_conversion, assert_check, array_storage
        ) scoped_array;
        // Need shared_array
        // weak_array? intrusive_array?

        typedef BOOST_SMART_PTR(T,
            com_ref_counted, disallow_conversion, assert_check
        ) com;

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    };
    
}   // namespace boost

#endif // STD_PTRS_HPP
