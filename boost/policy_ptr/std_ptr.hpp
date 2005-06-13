//////////////////////////////////////////////////////////////////////////////
//
//  std_ptr.hpp
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
// ChangeLog(sorted latest->earliest):
//   2005-06-07 Larry Evans
//     WHAT:
//       1) renamed file to std_ptr.hpp
//       2) renamed outer class to std_ptr
//       3) added shared_tagged and shared_accepting.
//     WHY:
//       1) to reflect same naming convention as smart_ptr.hpp.
//       2) to reflect filename and make more descriptive and more like
//          template smart_ptr.
//       3) allow cycle collection via "tagged" and precise pointer
//          location.
//   2005-05-30 Larry Evans
//     WHAT:
//       1) changed subdirectory include files names.
//     WHY:
//       1) to reflect name changes in policy subdirectory.
//////////////////////////////////////////////////////////////////////////////
//
//  Inspired by Andrei Alexandrescu, this header provides a type generator for
//  the most common smart pointer configurations, including emulations of the
//  current Boost smart pointers.  See libs/policy_ptr/doc/index.html
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_POLICY_PTR_STD_PTR_HPP
#define BOOST_POLICY_PTR_STD_PTR_HPP

#include "detail/shared_count_sp_counted_accepting.hpp"
#include "detail/shared_count_sp_counted_tagged.hpp"
#include "smart_ptr.hpp"
#include "policy/std.hpp"
#include "policy/auto_copy.hpp"
#include "policy/no_copy.hpp"

//////////////////////////////////////////////////////////////////////////////
// Usage:
//   std_ptr<Widget>::std_auto
//   std_ptr<Widget>::shared
//   std_ptr<Widget>::intrusive
//////////////////////////////////////////////////////////////////////////////

namespace boost
{
    template <typename T>
    struct std_ptr
    {

#ifndef BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef T* raw;
      #if 0
        typedef smart_ptr<
            auto_copy, allow_conversion, assert_check
        >::to<T> std_auto;

        typedef smart_ptr<
            no_copy, allow_conversion, assert_check, scalar_storage
        >::to<T> scoped;
      #endif
        typedef smart_ptr<
            boost_ref, allow_conversion, assert_check, shared_storage<>
        >::to<T> shared;
        typedef smart_ptr<
            boost_ref, allow_conversion, assert_check, shared_storage
            <policy_ptr::detail::sp_counted_accepting>
        >::to<T> shared_accepting;
        typedef smart_ptr<
            boost_ref, allow_conversion, assert_check, shared_storage
            <policy_ptr::detail::sp_counted_tagged>
        >::to<T> shared_tagged;
        typedef smart_ptr<
            boost_ref, allow_conversion, assert_check, weak_storage<>
        >::to<T> weak;
        typedef smart_ptr<
            boost_ref, allow_conversion, assert_check, intrusive_storage
        >::to<T> intrusive;
      #if 0

        typedef smart_ptr<
            no_copy, allow_conversion, assert_check, array_storage
        >::to<T> scoped_array;
        // Need shared_array
        // weak_array? intrusive_array?

        typedef smart_ptr<
            com_ref_counted, allow_conversion, assert_check
        >::to<T> com;
      #endif

#else // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

        typedef T* raw;
        typedef BOOST_SMART_PTR(T,
            auto_copy, allow_conversion, assert_check
        ) std_auto;

        typedef BOOST_SMART_PTR(T,
            no_copy, allow_conversion, assert_check, default_storage
        ) scoped;
        typedef BOOST_SMART_PTR(T,
            boost_ref, allow_conversion, assert_check, shared_storage
        ) shared;
        typedef BOOST_SMART_PTR(T,
            boost_ref, allow_conversion, assert_check, weak_storage
        ) weak;
        typedef BOOST_SMART_PTR(T,
            boost_ref, allow_conversion, assert_check, intrusive_storage
        ) intrusive;

        typedef BOOST_SMART_PTR(T,
            no_copy, allow_conversion, assert_check, array_storage
        ) scoped_array;
        // Need shared_array
        // weak_array? intrusive_array?

        typedef BOOST_SMART_PTR(T,
            com_ref_counted, allow_conversion, assert_check
        ) com;

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

    };
    
}   // namespace boost

#endif // STD_PTRS_HPP
