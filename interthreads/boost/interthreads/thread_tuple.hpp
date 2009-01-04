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
#ifdef BOOST_HAS_MOVE_LIB
#include <boost/move/move.hpp>
#endif
#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chono/chono.hpp>
#endif
#include <boost/thread/thread_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <utility>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {

    template <std::size_t N>
    class thread_tuple;
    
namespace detail {
    
    template <std::size_t size_>
    class thread_tuple_storage_stack {
        typedef thread_tuple_storage_stack<size_> this_type;
    public:
        struct  thread_tuple_data {
       	    array<thread, size_> threads_;
        };
        thread_tuple_data data_;
        thread_tuple_data* data() {return &data_;}
        thread& threads(std::size_t i) {return data_.threads_[i];}

        // constructors/destructors
    	inline thread_tuple_storage_stack() 
            : data_() 
        {}
            
#ifdef BOOST_HAS_RVALUE_REFS
        thread_tuple_storage_stack(this_type&& other) 
            : data_()
        {
        	for (std::size_t i=0; i<size_; i++) {
        		threads(i) = boost::move(other.data_.threads(i));
        	}
        }

        this_type&& move() {
            return static_cast<this_type&&>(*this);
        }
#else       
#ifdef BOOST_HAS_MOVE_LIB
        thread_tuple_storage_stack(move_from<this_type> other) 
            : data_()
        {
        	for (std::size_t i=0; i<size_; i++) {
        		this->threads(i) = boost::move(other.data_.threads(i));
        	}
        }

        operator move_from<this_type>() {
            return move();
        }            

        move_from<this_type> move() {
            return move_from<this_type>(*this);
        }            
#else        
        thread_tuple_storage_stack(boost::detail::thread_move_t<this_type> other) 
            : data_()
        {
        	for (std::size_t i=0; i<size_; i++) {
        		this->threads(i) = boost::move(other.data_.threads(i));
        	}
        }

        operator boost::detail::thread_move_t<this_type>() {
            return move();
        }            

        boost::detail::thread_move_t<this_type> move() {
            return boost::detail::thread_move_t<this_type>(*this);
        }            
#endif        
#endif        
        inline ~thread_tuple_storage_stack() { 
        }
    };

    template <std::size_t size_>
    class thread_tuple_storage_heap {
        typedef thread_tuple_storage_heap<size_> this_type;
    public:
        struct  thread_tuple_data {
       	    array<thread, size_> threads_;
        };
        shared_ptr<thread_tuple_data> data_;
        thread_tuple_data* data() {return data_.get();}
        thread& threads(std::size_t i) {return data_->threads_[i];}

        // constructors/destructors
    	inline thread_tuple_storage_heap() 
            : data_(new thread_tuple_data())
        {}
#ifdef BOOST_HAS_RVALUE_REFS
        thread_tuple_storage_heap(this_type&& other) 
            : data_(other->data_)
        {
            other->data_.reset();
        }

        this_type&& move() {
            return static_cast<this_type&&>(*this);
        }
#else        
        thread_tuple_storage_heap(boost::detail::thread_move_t<this_type> other) 
            : data_(other->data_)
        {
            other->data_.reset();
        }

        operator boost::detail::thread_move_t<this_type>() {
            return move();
        }

        boost::detail::thread_move_t<this_type> move() {
            return boost::detail::thread_move_t<this_type>(*this);
        }
#endif

        inline ~thread_tuple_storage_heap() {
        }
        void swap(this_type& x)
        {
            data_.swap(x.data_);
        }
        
    };
    
    template <std::size_t size_, class STORAGE>
    class thread_tuple_base : protected STORAGE
    {
        typedef STORAGE base_type;
        typedef thread_tuple_base<size_, STORAGE> this_type;
        
    public:
        // constructors/destructors
    	inline thread_tuple_base() 
            : base_type()
        {}

    public:
#ifdef BOOST_HAS_RVALUE_REFS
        thread_tuple_base(this_type&& other) {
            data_.swap(other.data_);
        }

        this_type&& move() {
            return static_cast<this_type&&>(*this);
        }
#else        
#ifdef BOOST_HAS_MOVE_LIB
        thread_tuple_base(move_from<this_type> x) 
            : base_type(boost::move(*(static_cast<base_type*>(&(x.member))))) {}

        operator move_from<this_type>() {
            return move();
        }            

        move_from move() {
            return move_from<this_type>(*this);
        }           
#else        
        thread_tuple_base(boost::detail::thread_move_t<this_type> x) 
            : base_type(boost::move(*(static_cast<base_type*>(&(x.t))))) {}

        operator boost::detail::thread_move_t<this_type>() {
            return move();
        }            

        boost::detail::thread_move_t<this_type> move() {
            return boost::detail::thread_move_t<this_type>(*this);
        }           
#endif
#endif
        inline ~thread_tuple_base() { 
            if (this->data()!=0) detach_all();
        }

        bool joinables() const {
        	for (std::size_t i=0; i<size_; i++) {
        		if (!this->threads(i).joinable()) return false;
        	}
        	return true;
        }

        void join_all() {
        	for (std::size_t i=0; i<size_; i++) {
        		this->threads(i).join();
        	}
        }

        bool join_all_until(const system_time& wait_until) {
        	for (std::size_t i=0; i<size_; i++) {
        		if (!this->threads(i).timed_join(wait_until)) return false;
        	}
        	return true;
        }

        template<typename TimeDuration>
        inline bool join_all_for(TimeDuration const& rel_time)
        {
            return join_all_until(get_system_time()+rel_time);
        }

        void detach_all() {
        	for (std::size_t i=0; i<size_; i++) {
        		this->threads(i).detach();
        	}
        }
            
        void interrupt_all() {
        	for (std::size_t i=0; i<size_; i++) {
        		this->threads(i).interrupt();
        	}
        }

        bool interruption_requested() const {
        	for (std::size_t i=0; i<size_; i++) {
        		if (!this->threads(i).interruption_requested()) return false;
        	}
        	return true;
        }
        
        inline std::size_t size() const { return size_; }
        
        inline thread& operator[](std::size_t i) const {
        	return this->threads(i);
        }

    private:
    	thread_tuple_base(thread_tuple_base&);
        thread_tuple_base& operator=(thread_tuple_base&);
//    protected:
//        struct  thread_tuple_data {
//       	    array<thread, size_> threads_;
//        };
//        shared_ptr<thread_tuple_data> data_;
//        thread& threads(std::size_t i) {return data_->threads_[i];}
        
    };    
}

    template <>
    class thread_tuple<2>: public detail::thread_tuple_base<2, detail::thread_tuple_storage_heap<2> > {
        typedef detail::thread_tuple_base<2, detail::thread_tuple_storage_heap<2> > base_type;
        typedef thread_tuple<2> this_type;
    public:
    	inline thread_tuple()  {}
            
        template<typename F0, typename F1>
    	inline thread_tuple(F0 f0, F1 f1)  {
            this->threads(0) = thread(f0);
   			this->threads(1) = thread(f1); 
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
            return static_cast<this_type&&>(*this);
        }
#else
#ifdef BOOST_HAS_MOVE_LIB
        thread_tuple(move_from<this_type> x) 
            : base_type(boost::move(static_cast<base_type*>(&(x.member)))) {}

        this_type& operator=(this_type& rhs) {
            this_type new_thread_tuple(rhs);
            swap(new_thread_tuple);
            return *this;
        }

        this_type& operator=(move_from<this_type> rhs) {
            this_type new_thread_tuple(rhs);
            swap(new_thread_tuple);
            return *this;
        }

        operator move_from<this_type>() {
            return move();
        }            

        move_from<this_type> move() {
            return move_from<this_type>(*this);
        }            
#else
        thread_tuple(boost::detail::thread_move_t<this_type> x) 
            : base_type(boost::move(*(static_cast<base_type*>(&(x.t))))) {}

        this_type& operator=(this_type& rhs) {
            this_type new_thread_tuple(boost::move(rhs));
            swap(new_thread_tuple);
            return *this;
        }

        this_type& operator=(boost::detail::thread_move_t<this_type> rhs) {
            this_type new_thread_tuple(rhs);
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
#endif
        
        void swap(this_type& x) { base_type::swap(x); }
        
    };    

    template <>
    class thread_tuple<3>: public detail::thread_tuple_base<3, detail::thread_tuple_storage_heap<3> > {
        typedef detail::thread_tuple_base<3, detail::thread_tuple_storage_heap<3> > base_type;
        typedef thread_tuple<3> this_type;
    public:
    	thread_tuple()  {}
        template<typename F0, typename F1, typename F2>
    	inline thread_tuple(F0 f0, F1 f1, F2 f2) 
            {
            this->threads(0) = thread(f0);
   			this->threads(1) = thread(f1); 
   			this->threads(2) = thread(f2); 
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

#include <boost/config/abi_suffix.hpp>

#endif
