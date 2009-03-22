// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_IS_LOCKABLE__HPP
#define BOOST_SYNCHRO_IS_LOCKABLE__HPP

#include <boost/detail/workaround.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { 
    
    template<typename T>
    struct is_lockable
    {
        BOOST_STATIC_CONSTANT(bool, value = false);
    };
    template<typename T>
    struct is_mutex_type : is_lockable<T>
    {
    };

    
}}

#include <boost/config/abi_suffix.hpp>

#endif
