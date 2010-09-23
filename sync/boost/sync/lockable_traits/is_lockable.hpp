// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2010 Vicente J. Botet Escriba

#ifndef BOOST_SYNC_IS_LOCKABLE_HPP
#define BOOST_SYNC_IS_LOCKABLE_HPP

#include <boost/config.hpp>
#include <boost/sync/lockable_traits/detail/has_members.hpp>
#include <boost/type_traits/integral_constant.hpp>

namespace boost
{

    template<typename T>
    struct is_basic_lockable : integral_constant<bool, 
            sync::detail::has_member_lock<T>::value &&
            sync::detail::has_member_unlock<T>::value
        >
    {
    };
    
    template<typename T>
    struct is_try_lockable : integral_constant<bool, 
            is_basic_lockable<T>::value &&
            sync::detail::has_member_try_lock<T>::value
        >
        
    {
    };
    
    template<typename T>
    struct is_timed_lockable: integral_constant<bool, 
            is_try_lockable<T>::value &&
            sync::detail::has_member_try_lock_until<T>::value &&
            sync::detail::has_member_try_lock_for<T>::value
        >
        
    {
    };
    
}


#endif
