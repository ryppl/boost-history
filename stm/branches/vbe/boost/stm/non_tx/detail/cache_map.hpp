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
//-----------------------------------------------------------------------------
#include <map>
#include <boost/stm/base_transaction_object.hpp>
#include <boost/stm/cache_fct.hpp>

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
class cache : public base_transaction_object {
public:
    T* const value_;
    mutable T* ptr_;

    inline cache(T& ref)
        : base_transaction_object()
        , value_(&ref), ptr_(0) {}

    inline cache(T* ptr)
        : base_transaction_object()
        , value_(ptr), ptr_(0) {}

    inline ~cache() {
        delete ptr_;
    }
    
    inline T const * get() const {
        if(ptr_!=0) return ptr_;
        else return value_;
    }

    inline T * get() {
        return ptr_;
    }
    virtual base_transaction_object* clone() const {
        cache* tmp = cache_clone(*this);
        if (tmp->value_!=0) {
            tmp->ptr_ = new T(*value_);
        }
        return tmp;
    }

    virtual void copy_state(base_transaction_object const * const rhs) {
        if (value_==0) return;
        *value_= *(static_cast<cache<T> const * const>(rhs)->ptr_);
        delete ptr_;
        ptr_=0;
    }

#ifdef BOOST_STM_USE_MEMCOPY
    virtual void cache_deallocate() {
        delete ptr_;
        ptr_=0;
        boost::stm::cache_deallocate(this);
    }
#endif

#if USE_STM_MEMORY_MANAGER
   void* operator new(size_t size) throw ()
   {
      return retrieve_mem(size);
   }

   void operator delete(void* mem)
   {
      return_mem(mem, sizeof(transactional_object<T>));
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
    static std::map<void*, base_transaction_object*> map_;
public:
    template <typename T>
    static cache<T>* get(T* ptr) {
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


