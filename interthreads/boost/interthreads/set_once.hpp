//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. Distributed under the Boost
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
#include <boost/utility/result_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <utility>
#include <boost/config/abi_prefix.hpp>

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
            assigned_=false;
        }
        bool wait_until(const system_time& until) {
            boost::unique_lock<boost::mutex> lock(mutex_);
            bool res = assigned_;
            if (!assigned_) res = cond_.timed_wait(lock, until);
            assigned_=false;
            return res;
        }
        value_type get() {
            boost::unique_lock<boost::mutex> lock(mutex_);
            if (!assigned_) cond_.wait(lock);
            assigned_=false;
            return val_;
        }
        
        std::pair<bool,value_type> get_until(const system_time& until) {
            boost::unique_lock<boost::mutex> lock(mutex_);
            bool res = assigned_;
            if (!assigned_) res = cond_.timed_wait(lock, until);
            assigned_=false;
            return std::make_pair(res, val_);
        }

        bool set_value_if_unassigned(value_type val) {
            boost::unique_lock<boost::mutex> lock(mutex_);
            if (!assigned_) { /*< first post assigns the current thread id >*/
            	assigned_=true;
            	val_ = val;
            	cond_.notify_all();
            	return true;
            } else { /*< the other settings are ignored >*/
            	return false;
            }
        }
        
        template<typename F>
        //static typename boost::enable_if<is_void<typename result_of<F()>::type>,void>::type 
        void
        decorator(this_type& once, T value, F fct) { 
    		fct();
    		once.set_value_if_unassigned(value);
        }

        template<typename F>
        //static typename boost::disable_if<is_void<typename result_of<F()>::type>, typename result_of<F()>::type>::type 
        typename result_of<F()>::type
        decoratorT(this_type& once, T value, F fct) { 
    		typename result_of<F()>::type res =fct();
    		once.set_value_if_unassigned(value);
            return res;
        }
        template<typename F>
        boost::detail::thread_move_t<thread> fork(T value, F fct) { 
            thread tmp_thread(bind(decorator<F>, ref(*this), value, fct));
            return tmp_thread;          
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
        value_type val_;
    };
}    
}   // namespace boost
#include <boost/config/abi_suffix.hpp>

#endif

