#ifndef BOOST_INTERTHREADS_KEEP_ALIVE_HPP
#define BOOST_INTERTHREADS_KEEP_ALIVE_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
//  copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <memory>

#include <boost/thread/thread.hpp>
namespace boost {
namespace interthreads {
    typedef void (*on_dead_thread_type)(thread::id, thread*); 
	namespace detail {
		struct thread_keep_alive_internal {
			bool 		enabled_;
			std::size_t periods_;
			std::size_t checkins_;
			std::size_t total_periods_;
			std::size_t required_checkins_;
			on_dead_thread_type on_dead_;
            thread* thread_ptr_;
			thread_keep_alive_internal();
		};
	}
namespace this_thread 	{

    class enable_keep_alive
    {
    	enable_keep_alive(const enable_keep_alive&);
    	enable_keep_alive& operator=(const enable_keep_alive&);
        
    	detail::thread_keep_alive_internal *backup_;
    	detail::thread_keep_alive_internal data_;
    public:
    	enable_keep_alive(std::size_t periods=2, std::size_t checkins=1);
        ~enable_keep_alive();
    };
    
    class disable_keep_alive
    {
        disable_keep_alive(const disable_keep_alive&);
        disable_keep_alive& operator=(const disable_keep_alive&);
        
    	detail::thread_keep_alive_internal *backup_;
    	detail::thread_keep_alive_internal data_;
    public:
        disable_keep_alive();
        ~disable_keep_alive();
    };

    void keep_alive_point();
    bool keep_alive_enabled();

    void set_on_dead_thread(on_dead_thread_type fct, thread* th=0); 
     
}

    bool keep_alive_enabled(thread::id);

	void set_on_dead_thread(thread::id, on_dead_thread_type fct, thread* th=0); 	

}
}

#endif

