//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_NON_TX_DETAIL_CACHE_MAP__HPP
#define BOOST_STM_NON_TX_DETAIL_CACHE_MAP__HPP

//-----------------------------------------------------------------------------
#include <map>
//-----------------------------------------------------------------------------
#include <boost/synchro.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/base_transaction_object.hpp>
#include <boost/stm/cache_fct.hpp>
#include <boost/stm/memory_managers/memory_manager.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

namespace non_tx { namespace detail {

//-----------------------------------------------------------------------------
// This class defines the transactional cache of a non transactional variable.
// There is a map from the address of the variable of type T to an instance of this class
//
//-----------------------------------------------------------------------------

template <typename T>
class cache : public
#ifdef USE_STM_MEMORY_MANAGER2
    memory_manager<cache<T>, base_transaction_object>
#else
    base_transaction_object
#endif
{
#ifdef USE_STM_MEMORY_MANAGER2
    typedef memory_manager<cache<T>, base_transaction_object> base_type;
#else
    typedef base_transaction_object base_type;
#endif
public:
    T* const value_;
    mutable T* ptr_;

    inline cache(T& ref)
        : base_type()
        , value_(&ref), ptr_(0) {}

    inline cache(T* ptr)
        : base_type()
        , value_(ptr), ptr_(0) {}

    inline ~cache() {
        delete ptr_;
    }

    inline T const * get() const {
        if(ptr_!=0) return ptr_;
        else return value_;
    }

    inline T * get() {
        if(ptr_!=0) return ptr_;
        else return value_;
    }

#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    virtual base_transaction_object* clone(transaction* t) const {
        cache<T>* p = cache_allocate<cache<T> >(t);
        if (p==0) {
            throw std::bad_alloc();
        }
        ::new (p) cache<T>(*static_cast<cache<T> const*>(this));
        return p;
#else
    virtual base_transaction_object* clone(transaction*) const {
        cache* p = new cache<T>(*this);
#endif
        if (p->value_!=0) {
            p->ptr_ = new T(*value_);
        }
        return p;
    }

#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    virtual void cache_deallocate() {
        delete ptr_;
        ptr_=0;
        this->~cache<T>();
        boost::stm::cache_deallocate(this);
    }
#else
    virtual void cache_deallocate() {
        delete ptr_;
        ptr_=0;
        delete this;
    }
#endif
    virtual void copy_state(base_transaction_object const * const rhs) {
        if (value_==0) return;
        *value_= *(static_cast<cache<T> const * const>(rhs)->ptr_);
        delete ptr_;
        ptr_=0;
    }

#if USE_STM_MEMORY_MANAGER
   void* operator new(std::size_t size, const std::nothrow_t&) throw ()
   {
      return base_memory_manager::retrieve_mem(size);
   }
    void* operator new(std::size_t size) throw (std::bad_alloc)
    {
        void* ptr= base_memory_manager::retrieve_mem(size);
        if (ptr==0) throw std::bad_alloc();
        return ptr;
    }

   void operator delete(void* mem) throw ()
   {
      base_memory_manager::return_mem(mem, sizeof(cache<T>));
   }
#endif
   
private:
    //cache(cache<T> const & r);

        //: base_transaction_object(r)
        //, value_(r.value_), ptr_(r.ptr_) {}


    //cache & operator=(cache const & r);
    //{
    //    value_ = r.value_;
    //    ptr_ = r.ptr_;
    //    return *this;
    //}

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class cache_map {
    typedef std::map<void*, base_transaction_object*> map_type;
    static Mutex mtx_;
    static std::map<void*, base_transaction_object*> map_;
public:
    template <typename T>
    static cache<T>* get(T* ptr) {
        synchro::lock_guard<Mutex> lk(mtx_);
        map_type::iterator it = map_.find(ptr);
        cache<T>* res=0;
        if (it == map_.end()) {
            res= new cache<T>(ptr);
            map_.insert(std::make_pair(ptr, res));
        } else {
            res=static_cast<cache<T>*>(it->second);
        }
        return res;
    }
    template <typename T>
    static cache<T>* get(T const* ptr) {
        synchro::lock_guard<Mutex> lk(mtx_);
        map_type::iterator it = map_.find(const_cast<T*>(ptr));
        cache<T>* res=0;
        if (it == map_.end()) {
            res= new cache<T>(const_cast<T*>(ptr));
            map_.insert(std::make_pair(const_cast<T*>(ptr), res));
        } else {
            res=static_cast<cache<T>*>(it->second);
        }
        return res;
    }
};

}}
}}
#endif


