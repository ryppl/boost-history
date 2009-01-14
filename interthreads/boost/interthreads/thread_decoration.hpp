#ifndef BOOST_INTERTHREADS_THREAD_DECORATION__HPP
#define BOOST_INTERTHREADS_THREAD_DECORATION__HPP

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

#include <boost/interthreads/detail/config.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/interthreads/detail/decorator_function.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    class thread_decoration {
        thread_decoration();  
        thread_decoration(const thread_decoration&);  
    public:
 	    template<typename Callable1>
	    thread_decoration(Callable1 setup)
 		    : setup_(new detail::decorator_function<Callable1>(setup))
            , cleanup_(0)
            , prev_(last_) {
 		    // the constructor is not thread-safe so it must only be used on
 		    // global objects which are constructed before any threads
 		    last_ = this;
 	    }
        
 	    template<typename Callable1,typename Callable2>
	    thread_decoration(Callable1 setup, Callable2 cleanup)
 		    : setup_(new detail::decorator_function<Callable1>(setup))
//            , cleanup_(new detail::decorator_function<Callable2>(cleanup))
            , cleanup_(cleanup)
            , prev_(last_) {
 		    // the constructor is not thread-safe so it must only be used on
 		    // global objects which are constructed before any threads
 		    last_ = this;
 	    }
        
 	    ~thread_decoration() {
            delete setup_;
        }
        static void BOOST_INTERTHREADS_DECL decorate();

    private:
        template <typename T>
        friend class basic_thread_decorator;
        detail::decorator_function_base<void>* setup_;
        boost::function<void ()> cleanup_;
        static thread_decoration* last_;
        thread_decoration* prev_;
    };

     
}
}

#include <boost/config/abi_suffix.hpp>


#endif

