//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_THREAD_TUPLE_ONCE_HPP
#define BOOST_INTERTHREADS_THREAD_TUPLE_ONCE_HPP

#include <boost/bind.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interthreads/set_once.hpp>
#include <utility>

namespace boost {
namespace interthreads {

    template <std::size_t N>
    class thread_tuple_once;
    
    template <std::size_t size_>
    class thread_tuple_once_base
    {
    public:
    	inline thread_tuple_once_base() 
            : data_(new thread_tuple_once_data())
        {}
        inline ~thread_tuple_once_base() { 
            detach();
        }

        void join() {
        	once().wait();
        	for (std::size_t i=0; i<size_; i++) {
        		threads(i).join();
        	}
        }

        inline void join_all() { join(); }
        bool joinable() const {
        	for (std::size_t i=0; i<size_; i++) {
        		if (!threads(i).joinable()) return false;
        	}
        	return true;
        }

        bool timed_join(const system_time& wait_until) {
        	std::pair<bool,std::size_t> succeed = once().timed_get(wait_until);
            if (!succeed.first) return false;
        	for (std::size_t i=0; i<size_; i++) {
        		if (!threads(i).timed_join(wait_until)) return false;
        	}
        	return true;
        }
        inline bool timed_join_all(const system_time& wait_until) { 
            return timed_join(wait_until);
        }

        template<typename TimeDuration>
        inline bool timed_join(TimeDuration const& rel_time)
        {
            return timed_join(get_system_time()+rel_time);
        }

        template<typename TimeDuration>
        inline bool timed_join_all(TimeDuration const& rel_time) { 
            return timed_join(rel_time);
        }

        std::size_t join_first() {
        	return once().get();
        }
        std::pair<bool,std::size_t> timed_join_first(const system_time& wait_until) {
        	return once().timed_get(wait_until);
        }

        template<typename TimeDuration>
        inline std::pair<bool,std::size_t> timed_join_first(TimeDuration const& rel_time)
        {
            return timed_join_first(get_system_time()+rel_time);
        }

        void detach() {
        	for (std::size_t i=0; i<size_; i++) {
        		threads(i).detach();
        	}
        }
        inline void detach_all() { detach(); }
            
        void interrupt() {
        	for (std::size_t i=0; i<size_; i++) {
        		threads(i).interrupt();
        	}
        }

        inline void interrupt_all() { interrupt(); }
        
        bool interruption_requested() const {
        	for (std::size_t i=0; i<size_; i++) {
        		if (threads(i).interruption_requested()) return true;
        	}
        	return false;
        }
        
        inline std::size_t size() const { return size_; }
        
        inline thread& operator[](std::size_t i) const {
        	return *threads(i);
        }

    private:
    	thread_tuple_once_base(thread_tuple_once_base&);
        thread_tuple_once_base& operator=(thread_tuple_once_base&);
    protected:
        typedef set_once<std::size_t> set_once_type;

        struct  thread_tuple_once_data {
       	    set_once_type once_;
       	    thread threads_[size_];
        };
        shared_ptr<thread_tuple_once_data> data_;
        set_once_type& once() { return data_->once_; }
        thread& threads(std::size_t i) {return data_->threads_[i];}
        
    };    
    
    template <>
    class thread_tuple_once<2>: public thread_tuple_once_base<2> {
        typedef thread_tuple_once_base<2> base_type;
    public:
        template<typename F0, typename F1>
    	inline thread_tuple_once(F0 f0, F1 f1)  {
            data_->threads_[0] = base_type::set_once_type::make_thread(once(), 0, f0);
   			data_->threads_[1] = base_type::set_once_type::make_thread(once(), 1, f1); 
    	}
    };    

    template <>
    class thread_tuple_once<3>: public thread_tuple_once_base<3> {
        typedef thread_tuple_once_base<3> base_type;
    public:
        template<typename F0, typename F1, typename F2>
    	inline thread_tuple_once(F0 f0, F1 f1, F2 f2) {
            data_->threads_[0] = base_type::set_once_type::make_thread(once(), 0, f0);
   			data_->threads_[1] = base_type::set_once_type::make_thread(once(), 1, f1); 
   			data_->threads_[2] = base_type::set_once_type::make_thread(once(), 2, f2); 
    	}
    };    


}    
}   // namespace boost


#endif
