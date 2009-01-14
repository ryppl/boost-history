#ifndef BOOST_INTERTHREADS_THREADER_DECORATOR__HPP
#define BOOST_INTERTHREADS_THREADER_DECORATOR__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Extension of the init class of the threadalert library of Roland Schwarz
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/asynchronous_executor_decorator.hpp>
#include <boost/interthreads/thread_decorator.hpp>
#include <boost/interthreads/threader.hpp>


#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    

    typedef asynchronous_executor_decorator<unique_threader,basic_thread_decorator> unique_threader_decorator;
    typedef asynchronous_executor_decorator<shared_threader,basic_thread_decorator> shared_threader_decorator;
    
}
}

#include <boost/config/abi_suffix.hpp>


#endif

