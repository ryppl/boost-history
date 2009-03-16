// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_IS_MUTEX_TYPE_WRAPPER__HPP
#define BOOST_SYNCHRO_LOCKABLES_IS_MUTEX_TYPE_WRAPPER__HPP

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {
        
    namespace detail
    {
        template<bool x>
        struct is_mutex_type_wrapper
        {};
    }
}}}

#include <boost/config/abi_suffix.hpp>

#endif
