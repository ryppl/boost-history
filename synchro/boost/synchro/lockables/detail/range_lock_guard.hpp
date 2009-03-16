// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_RANGE_LOCK_GUARD_HPP
#define BOOST_SYNCHRO_LOCKABLES_RANGE_LOCK_GUARD_HPP

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {  
    namespace detail {
        template<typename Iterator>
        struct range_lock_guard
        {
            Iterator begin;
            Iterator end;
            
            range_lock_guard(Iterator begin_,Iterator end_):
                begin(begin_),end(end_)
            {
                lockables::lock(begin,end);
            }
            
            void release()
            {
                begin=end;
            }

            ~range_lock_guard()
            {
                for(;begin!=end;++begin)
                {
                    unlock(*begin);
                }
            }
        };
    }
}}}

#include <boost/config/abi_suffix.hpp>

#endif
