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
//  See http://www.boost.org/ for most recent version, including documentation.
//
//////////////////////////////////////////////////////////////////////////////
//
//  Inspired by Andrei Alexandrescu, this header provides a type generator for
//  the most common smart pointer configurations, including emulations of the
//  current Boost smart pointers.  See libs/policy_ptr/doc/index.html
//
//////////////////////////////////////////////////////////////////////////////

#ifndef STD_PTRS_HPP
#define STD_PTRS_HPP

#include "smart_ptr.hpp"
#include "policy/boost_policies.hpp"
#include "policy/auto_copy.hpp"
#include "policy/no_copy.hpp"

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
      #if 0
        typedef smart_ptr<
            auto_copy, disallow_conversion, assert_check
        >::to<T> std_auto;

        typedef smart_ptr<
            no_copy, disallow_conversion, assert_check, scalar_storage
        >::to<T> scoped;
      #endif
        typedef smart_ptr<
            boost_ref, disallow_conversion, assert_check, shared_storage
        >::to<T> shared;
        typedef smart_ptr<
            boost_ref, disallow_conversion, assert_check, weak_storage
        >::to<T> weak;
        typedef smart_ptr<
            boost_ref, disallow_conversion, assert_check, intrusive_storage
        >::to<T> intrusive;
      #if 0

        typedef smart_ptr<
            no_copy, disallow_conversion, assert_check, array_storage
        >::to<T> scoped_array;
        // Need shared_array
        // weak_array? intrusive_array?

        typedef smart_ptr<
            com_ref_counted, disallow_conversion, assert_check
        >::to<T> com;
      #endif

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef T* raw;
        typedef BOOST_SMART_PTR(T,
            auto_copy, disallow_conversion, assert_check
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
