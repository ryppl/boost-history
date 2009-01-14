#ifndef BOOST_INTERTHREADS_THREAD_SPECIFIC_SHARED_PTR_HPP
#define BOOST_INTERTHREADS_THREAD_SPECIFIC_SHARED_PTR_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Roland Schwarz 2006. 
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
//  copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
// Based on the thmemptr from the threadalert library of Roland Schwarz 
//
//////////////////////////////////////////////////////////////////////////////


//#include <boost/thread/detail/config.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread.hpp>
#include <map>

#include <boost/config/abi_prefix.hpp>

namespace boost {
namespace interthreads {
    
    template <typename T>
    class thread_specific_shared_ptr {
    public:
    	typedef shared_ptr<T>				shared_ptr_type;
    	typedef std::map<thread::id, shared_ptr_type> map_type;
    	typedef mutex 						mutex_type;
    	typedef unique_lock<mutex_type>	lock_type;
    private:
    	typedef condition_variable		    condition_type;
        
    	thread_specific_shared_ptr(thread_specific_shared_ptr&);
    	thread_specific_shared_ptr& operator=(thread_specific_shared_ptr&);

		typedef thread_specific_shared_ptr<T> 					this_type;
		struct tss_type
		{
			explicit tss_type(this_type& type, void (*cleanup)(shared_ptr_type)) 
			: type_(type)
			, shptr_()
            , cleanup_(cleanup)
			{}
			thread_specific_shared_ptr<T>& type_;
			shared_ptr_type shptr_;
            void (*cleanup_)(shared_ptr_type);
		};
		typedef thread_specific_ptr<tss_type> 					tss_ptr_type;
    	
    	mutable mutex_type 	    monitor_;
    	mutable condition_type 	mapchanged_;
		mutable tss_ptr_type 	    tss_ptr_;
		map_type 				    tmap_;
        void (*cleanup_)(shared_ptr_type);
       
        struct run_custom_cleanup_function
        {
            void (*cleanup_function_)(T*);
            
            explicit run_custom_cleanup_function(
            			void (*cleanup_function)(shared_ptr_type)):
                cleanup_function_(cleanup_function)
            {}
            
            void operator()(tss_type* ptr)
            {
                if(cleanup_function_) {
                	cleanup_function_(ptr->shptr_);
                }
            	ptr->type_.detach_from_current_thread(); 
                delete ptr;

            }
        };
        
		void detach_from_current_thread() {
			lock_type lock(monitor_);
			tmap_.erase(boost::this_thread::get_id());
		}
        
		tss_type*  create_if_not_exists() {
			tss_type* tss_ptr=tss_ptr_.get();
			if (tss_ptr==0) {
				tss_ptr = new tss_type(*this, cleanup_);
				tss_ptr_.reset(tss_ptr);
            }
			return tss_ptr;
		}

		void update_map(tss_type* tss_ptr) {
			// update the map
			lock_type lock(monitor_);
			tmap_[boost::this_thread::get_id()] = tss_ptr->shptr_;
			// and notify the change
			mapchanged_.notify_all();
		}
       
    public:
    	thread_specific_shared_ptr()
        :  tss_ptr_(detach_and_delete_cleanup_function) 
        , cleanup_(0) {}
        explicit thread_specific_shared_ptr(void (*cleanup)(shared_ptr_type))
        : tss_ptr_(detach_and_delete_cleanup_function)
        , cleanup_(cleanup) {}

        static void detach_and_delete_cleanup_function(tss_type* ptr) {
            if (ptr->cleanup_!=0) (*(ptr->cleanup_))(ptr->shptr_);
            ptr->type_.detach_from_current_thread(); 
            delete ptr;
        }
            
        ~thread_specific_shared_ptr()
        {}
        
		T* get() const {
			tss_type* tss_ptr=tss_ptr_.get();
			if (tss_ptr==0) { return 0; }
			else { return tss_ptr->shptr_.get(); }
		}
		
		T* operator->() const { 
            return get();
		}
		
		T& operator*() const { 
            return *get();
		}
#if 0        
		shared_ptr_type release()
        {
        	// get the tss
			tss_type* tss_ptr=tss_ptr_.get();

			if (tss_ptr==0) { return shared_ptr_type(); }
			
			// copy the shared pointer
        	shared_ptr_type temp= tss_ptr->shptr_;
        	// release the ptr 
			tss_ptr->shptr_.release();
			{
				// erase from the map
				lock_type lock(monitor_);
				tmap_.erase(boost::this_thread::get_id());
			}
			// return the copied shared pointer 
            return temp;
        }
#endif        
        void reset()
        {
			tss_type* tss_ptr=create_if_not_exists();
			tss_ptr->shptr_.reset();
			update_map(tss_ptr);
        }

        template<class Y> void reset(Y * p)
        {
			tss_type* tss_ptr=create_if_not_exists();
			tss_ptr->shptr_.reset(p);
			update_map(tss_ptr);
        }

        template<class Y, class D> void reset(Y * p, D d)
        {
			tss_type* tss_ptr=create_if_not_exists();
			tss_ptr->shptr_.reset(p, d);
			update_map(tss_ptr);
        }

        template<class Y, class D, class A> void reset(Y * p, D d, A a)
        {
			tss_type* tss_ptr=create_if_not_exists();
			tss_ptr->shptr_.reset(p, d, a);
			update_map(tss_ptr);
        }

//        template<class Y> void reset(shared_ptr<Y> const & r, T * p)
                

        shared_ptr_type wait_and_get(thread::id id) const {
			lock_type lock(monitor_);
        	typename map_type::const_iterator i;
			while ((i = tmap_.find(id)) == tmap_.end())
				mapchanged_.wait(lock);
			return i->second;
		}

        shared_ptr_type operator[](thread::id id) const {
			lock_type lock(monitor_);
        	typename map_type::const_iterator i(tmap_.find(id));
			if ( i == tmap_.end()) {
                mapchanged_.wait(lock);

				return shared_ptr_type();
			} else {
				return i->second;
			}
		}

    	mutex_type& 			get_mutex() {return monitor_;}
        
		const map_type& get_map(lock_type& lock) {
			return tmap_;
		}
        
    };
}
}

#include <boost/config/abi_suffix.hpp>

#endif
