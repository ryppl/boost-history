//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_SET_ONCE_HPP
#define BOOST_INTERTHREADS_SET_ONCE_HPP

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <utility>

namespace boost {
namespace interthreads {


    template <typename T>
    class set_once {
            typedef set_once<T> this_type;
    public:
        typedef T value_type;
        set_once() : assigned_(false) {}

        void wait() {
            boost::unique_lock<boost::mutex> lock(mutex_);
            if (!assigned_) cond_.wait(lock);
        }
        value_type get() {
            boost::unique_lock<boost::mutex> lock(mutex_);
            if (!assigned_) cond_.wait(lock);
            return id_;
        }
        
        std::pair<bool,value_type> timed_get(const system_time& wait_until) {
            boost::unique_lock<boost::mutex> lock(mutex_);
            bool res = assigned_;
            if (!assigned_) res = cond_.timed_wait(lock, wait_until);
            return std::make_pair(res, id_);
        }

        bool set(value_type id) {
            boost::unique_lock<boost::mutex> lock(mutex_);
            if (!assigned_) { /*< first post assigns the current thread id >*/
            	assigned_=true;
            	id_ = id;
            	cond_.notify_all();
            	return true;
            } else { /*< the other post do nothing >*/
            	return false;
            }
        }
        
        template<typename F>
        static void decorator(this_type& once, T value, F fct) { 
    		fct();
    		once.set(value);
        }
        template<typename F>
        static boost::detail::thread_move_t<thread> make_thread(this_type& once, T value, F fct) { 
            thread tmp_thread(bind(decorator<F>, ref(once), value, fct));
            return tmp_thread;          
        }   

    private:
        mutex mutex_;
        condition_variable cond_;
        bool assigned_;
        value_type id_;
    };
}    
}   // namespace boost

#endif

