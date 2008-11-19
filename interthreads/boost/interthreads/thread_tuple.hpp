//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERTHREADS_THREAD_TUPLE_HPP
#define BOOST_INTERTHREADS_THREAD_TUPLE_HPP

#include <boost/bind.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/thread/thread.hpp>
#include <utility>

namespace boost {
namespace interthreads {

    template <std::size_t N>
    class thread_tuple;
    
    template <std::size_t size_>
    class thread_tuple_base
    {
    public:
        // constructors/destructors
    	inline thread_tuple_base() 
            : data_(new thread_tuple_data())
        {}
        inline ~thread_tuple_base() { 
            detach();
        }

        void join() {
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

        void detach() {
            //if (data_.get()==0) return;
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
    	thread_tuple_base(thread_tuple_base&);
        thread_tuple_base& operator=(thread_tuple_base&);
    protected:
        struct  thread_tuple_data {
       	    thread threads_[size_];
        };
        shared_ptr<thread_tuple_data> data_;
        thread& threads(std::size_t i) {return data_->threads_[i];}
        
    };    
    
    template <>
    class thread_tuple<2>: public thread_tuple_base<2> {
        typedef thread_tuple_base<2> base_type;
        typedef thread_tuple<2> this_type;
    public:
    	inline thread_tuple()  {}
            
        template<typename F0, typename F1>
    	inline thread_tuple(F0 f0, F1 f1)  {
            data_->threads_[0] = thread(f0);
   			data_->threads_[1] = thread(f1); 
    	}

        // move support
#ifdef BOOST_HAS_RVALUE_REFS
        thread_tuple(this_type&& other) {
            data_.swap(other.data_);
        }
        this_type& operator=(this_type&& other) {
            data_=other.data_;
            other.data_.reset(new thread_tuple_data());
            return *this;
        }

        this_type&& move() {
            return static_cast<thread&&>(*this);
        }
#else        
        thread_tuple(boost::detail::thread_move_t<this_type> x) {
            data_=x->data_;
            x->data_.reset(new thread_tuple_data());
        }
            
        this_type& operator=(boost::detail::thread_move_t<this_type> x) {
            this_type new_thread_tuple(x);
            swap(new_thread_tuple);
            return *this;
        }

        operator boost::detail::thread_move_t<this_type>() {
            return move();
        }            

        boost::detail::thread_move_t<this_type> move() {
            return boost::detail::thread_move_t<this_type>(*this);
        }            
            
#endif        
        
        void swap(this_type& x)
        {
            data_.swap(x.data_);
        }
        
    };    

    template <>
    class thread_tuple<3>: public thread_tuple_base<3> {
        typedef thread_tuple_base<3> base_type;
        typedef thread_tuple<3> this_type;
    public:
    	thread_tuple()  {}
        template<typename F0, typename F1, typename F2>
    	inline thread_tuple(F0 f0, F1 f1, F2 f2) {
            data_->threads_[0] = thread(f0);
   			data_->threads_[1] = thread(f1); 
   			data_->threads_[2] = thread(f2); 
    	}
        // move support
#ifdef BOOST_HAS_RVALUE_REFS
        thread_tuple(this_type&& other) {
            data_.swap(other.data_);
        }
        this_type& operator=(this_type&& other) {
            data_=other.data_;
            other.data_.reset(new thread_tuple_data());
            return *this;
        }

        this_type&& move() {
            return static_cast<thread&&>(*this);
        }
#else        
        thread_tuple(boost::detail::thread_move_t<this_type> x) {
            data_=x->data_;
            x->data_.reset(new thread_tuple_data());
        }
            
        this_type& operator=(boost::detail::thread_move_t<this_type> x) {
            this_type new_thread_tuple(x);
            swap(new_thread_tuple);
            return *this;
        }

        operator boost::detail::thread_move_t<this_type>() {
            return move();
        }            

        boost::detail::thread_move_t<this_type> move() {
            return boost::detail::thread_move_t<this_type>(*this);
        }            
            
#endif        
        void swap(this_type& x)
        {
            data_.swap(x.data_);
        }
        
    };    

    template<typename F0, typename F1>
    thread_tuple<2> make_thread_tuple(F0 f0, F1 f1) {
        return thread_tuple<2>(f0, f1);
    }
    
    template<typename F0, typename F1, typename F2>
    thread_tuple<3> make_thread_tuple(F0 f0, F1 f1, F2 f2) {
        return thread_tuple<3>(f0, f1, f2);
         
    }
    

}    
}   // namespace boost


#endif
