// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling 
// Copyright (c) 2010 Boris Schaeling, Felipe Tanus
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

/** 
 * \file boost/process/detail/helper_functions.hpp 
 * 
 * Includes the declaration of helper functions for the operations.
 * It's for internal purposes.
 *
 */ 

#if defined(BOOST_POSIX_API) 
        #include <unistd.h>
        #include <sys/stat.h>
        #include <fcntl.h>
#endif
#include <map>
#include <string.h>
#include <boost/optional.hpp>
#include <boost/process/stream_behavior.hpp>



#ifndef BOOST_PROCESS_HELPER_FUNCTIONS_HPP 
#define BOOST_PROCESS_HELPER_FUNCTIONS_HPP 
namespace boost{
namespace process{
namespace detail{


}
}
}


#endif
