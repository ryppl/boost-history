/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_POSIC_TIME_TRAITS_HPP_
#define BOOST_LUID_POSIC_TIME_TRAITS_HPP_

#include "boost/date_time/posix_time/posix_time_types.hpp" 

namespace boost {
namespace luid {
namespace iccl {    
    
struct posix_time_traits {
    typedef boost::posix_time::ptime                    time_type;
    typedef boost::posix_time::time_duration    duration_type;
    static time_type now() { 
        return boost::posix_time::second_clock::local_time(); 
    }
    static const duration_type def() { return  duration_type(0,0,0,1);}
};

}
}
}
#endif /*BOOST_LUID_POSIC_TIME_TRAITS_HPP_*/
