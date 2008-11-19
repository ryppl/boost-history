//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
//  copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threadalert library of Roland Schwarz
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/thread_decorator.hpp>
#include <boost/thread/thread.hpp>

namespace boost {
namespace interthreads {
//boost::interthreads::
    thread_decoration* boost::interthreads::thread_decoration::last_ = 0;

    void thread_decoration::decorate() {
	    thread_decoration* p = thread_decoration::last_;
	    while (0 != p) {
    		(*(p->setup_))();
            if (!p->cleanup_.empty()) {
                boost::this_thread::at_thread_exit(p->cleanup_);
            }
		    p = p->prev_;
	    }
    }

    void thread_decorator::operator()() const {
        decorate();
        (*func_)();
    }
  
}
}



