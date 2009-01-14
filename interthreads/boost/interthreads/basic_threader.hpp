#ifndef BOOST_INTERTHREADS_BASIC_THREADER__HPP
#define BOOST_INTERTHREADS_BASIC_THREADER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread.hpp>
//#include <boost/thread/tss.hpp>

#include <boost/interthreads/fork.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {


class basic_threader {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
private:
    thread::native_handle_attr_type attr_;
public:
    thread::native_handle_attr_type& attr() {
        return attr_;
    }
#endif  

public: 
    template <typename T>
    struct handle {
        typedef thread type;
    };

    template <typename F>
    thread 
    fork(F f) {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        thread th(attr(), f);
#else
        thread th(f);
#endif        
        return boost::move(th);        
        //return thread(boost::detail::thread_move_t<thread>(th));
    }   
};


}
}


#include <boost/config/abi_suffix.hpp>

#endif

