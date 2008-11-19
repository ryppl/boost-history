//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_THREAD_GROUP_ONCE_HPP
#define BOOST_INTERTHREADS_THREAD_GROUP_ONCE_HPP

#include <boost/bind.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interthreads/set_once.hpp>
#include <utility>

namespace boost {
namespace interthreads {

    class thread_group_once
    {
    public:
    	inline thread_group_once() {}
        inline ~thread_group_once() { 
#if 0            
            grp().detach();
#endif            
        }

        template<typename F>
        thread* create_thread(F threadfunc) {
            std::auto_ptr<thread> new_thread(new thread());
            *new_thread = set_once_type::make_thread(once(), new_thread.get(), threadfunc);          
            grp().add_thread(new_thread.get());
            return new_thread.release();
        }
        
        void remove_thread(thread* thrd) {
            grp().remove_thread(thrd);
        }

        void join() {
        	once().wait();
            grp().join_all();
        }

        inline void join_all() { join(); }
        
#if 0        
        bool joinable() const {
        	return grp.joinable();
        }
#endif        

#if 0 
        bool timed_join(const system_time& wait_until) {
        	return grp.timed_join(wait_until);
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
#endif
        thread* join_first() {
        	return once().get();
        }

        std::pair<bool,thread*> timed_join_first(const system_time& wait_until) {
        	return once().timed_get(wait_until);
        }

        template<typename TimeDuration>
        inline std::pair<bool,thread*> timed_join_first(TimeDuration const& rel_time)
        {
            return timed_join_first(get_system_time()+rel_time);
        }

#if 0        
        void detach() {
        	grp().detach();
        }

        inline void detach_all() { detach(); }
#endif
            
        void interrupt() {
        	grp().interrupt_all();
        }

        inline void interrupt_all() { interrupt(); }
        
#if 0        
        bool interruption_requested() const {
        	grp().interruption_requested();
        }
#endif
        
        inline std::size_t size() const { return grp().size(); }
        
    private:
    	thread_group_once(thread_group_once&);
        thread_group_once& operator=(thread_group_once&);
    protected:
        typedef set_once<thread*> set_once_type;
        struct  thread_group_once_data {
       	    set_once_type once_;
       	    boost::thread_group grp_;
        };
        thread_group_once_data data_;
        set_once_type& once() { return data_.once_; }
        boost::thread_group& grp() {return data_.grp_;}
        const boost::thread_group& grp() const {return data_.grp_;}
        
    };    

}    
}   // namespace boost


#endif

