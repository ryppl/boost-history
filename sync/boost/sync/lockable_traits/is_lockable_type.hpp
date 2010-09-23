// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams
// (C) Copyright 2010 Vicente J. Botet Escriba

#ifndef BOOST_SYNC_IS_LOCKABLE_TYPE_HPP
#define BOOST_SYNC_IS_LOCKABLE_TYPE_HPP

#include <boost/config.hpp>
#include <boost/sync/lockable_traits/detail/has_members.hpp>

namespace boost
{

#ifndef BOOST_SYNC_NO_AUTO_DETECT_TYPES   

    template<typename T>
    struct is_mutex_type
    {
        BOOST_STATIC_CONSTANT(bool, value = 
            boost::sync::detail::has_member_lock<T>::value &&
            boost::sync::detail::has_member_unlock<T>::value &&
            boost::sync::detail::has_member_try_lock<T>::value);
        
    };
#else
    template<typename T>
    struct is_mutex_type
    {
        BOOST_STATIC_CONSTANT(bool, value = false);
    };
#endif    

    template<typename T>
    struct is_lockable_type
    {
        BOOST_STATIC_CONSTANT(bool, value=is_mutex_type<T>::value);
    };
    
    
}


#endif
