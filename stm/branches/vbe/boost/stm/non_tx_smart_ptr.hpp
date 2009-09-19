//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_NON_TX_SMART_PTR__HPP
#define BOOST_STM_NON_TX_SMART_PTR__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <assert.h>
#include <map>
#include <boost/stm/transaction.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {

namespace non_tx {

template <typename T>
class rd_ptr;
template <typename T>
class wr_ptr;
template <typename T>
class upgrd_ptr;
    
//-----------------------------------------------------------------------------
// This class defines the transactional cache of a non transactional variable.
// There is a map from the address of the variable of type T to an instance of this class
//
//-----------------------------------------------------------------------------

namespace detail {

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
    
    inline T* get() const {
        if(ptr_!=0) return ptr_;
        else return value_;
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

}
//-----------------------------------------------------------------------------
// A rd_ptr<T> ("read pointer") points to an object that the current
// transaction has opened for read only access.
// You can only call a const method through a read pointer.
// A rd_ptr<T> is constructed from an tx_ptr<T> through an explicit constructor.
// Once a rd_ptr<T> has been constructed, an tx_ptr<T> can be opened for
// reading simply by assignment (operator=()) into the constructed rd_ptr<T>.
// It is not safe to derreference a rd_ptr<T> after having assigned the same
// tx_ptr<T> to a wr_ptr<T>. If this is the case the readen value do not match
// the writen one. If it is possible to write on the same transaction use
// upgrd_ptr instead which is safe.
//-----------------------------------------------------------------------------


template <typename T>
class rd_ptr {
    typedef rd_ptr<T> this_type;
public:
    mutable transaction& tx_;
    mutable detail::cache<T>* ptr_;

    inline rd_ptr(transaction &t, T const * ptr) : tx_(t),
        ptr_(&t.insert_and_return_read_memory(*detail::cache_map::get(ptr)))
    {}

    inline rd_ptr(transaction &t, T const & obj) : tx_(t),
        ptr_(&t.insert_and_return_read_memory(*detail::cache_map::get(&obj)))
    {}

    template<class Y>
    inline rd_ptr & operator=(Y const* ptr) { // never throws
        ptr_=tx_.insert_and_return_read_memory(*detail::cache_map::get(ptr));
        return *this;
    }
        
    template<class Y>
    inline rd_ptr & operator=(Y const& ref) { // never throws
        ptr_=tx_.insert_and_return_read_memory(*detail::cache_map::get(&ref));
        return *this;
    }

    const T* get() const {
        if (tx_.forced_to_abort()) {
            tx_.lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return ptr_->get();
    }

    inline const T & operator*() const { return *get(); }
    inline const T* operator->() const { return get(); }


    typedef detail::cache<T>* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }

};

template <typename T>
inline rd_ptr<T> make_rd_ptr(transaction& tx, T* ptr) {
    return rd_ptr<T>(tx, ptr);
}

template <typename T>
inline rd_ptr<T> make_rd_ptr(transaction& tx, T& ref) {
    return rd_ptr<T>(tx, ref);
}

template <typename T>
inline rd_ptr<T> make_rd_ptr(T* ptr) {
    transaction* tx = transaction::current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ptr);
}

template <typename T>
inline rd_ptr<T> make_rd_ptr(T& ref) {
    transaction* tx = transaction::current_transaction();
    assert(tx==0);
    return rd_ptr<T>(*tx, ref);
}

template <typename T>
inline void delete_ptr(rd_ptr<T> ptr) {
    if (ptr.ptr_==0) return;
    ptr.tx_->delete_tx_ptr(ptr.ptr_);
}



//-----------------------------------------------------------------------------
// A upgrd_ptr<T> ("upgradable pointer") points to an object that the current
// transaction has opened for read only access.
// You can only call a const method through a read pointer.
// A upgrd_ptr<T> is constructed from an tx_ptr<T> through a constructor
// having also the transaction as parameter.
// Once a rd_ptr<T> has been constructed, an tx_ptr<T> can be opened for
// reading simply by assignment (operator=()) into the constructed rd_ptr<T>.
// It is safe to derreference a rd_ptr<T> after having assigned the same
// tx_ptr<T> to a wr_ptr<T>.
// A upgrd_ptr<T> can be upgraded to a wr_ptr<T> through a constructor.
//-----------------------------------------------------------------------------

template <typename T>
class upgrd_ptr {
    typedef upgrd_ptr<T> this_type;
public:
    mutable transaction* tx_;
    mutable detail::cache<T>* ptr_;
    mutable bool written_;

    inline upgrd_ptr(transaction &t, T* ptr) : tx_(&t),
        ptr_(const_cast<detail::cache<T>*>(t.read_ptr(detail::cache_map::get(ptr)))), written_(false)    {}

    inline upgrd_ptr(transaction &t, T& ref) : tx_(&t),
        ptr_(const_cast<detail::cache<T>*>(t.read_ptr(detail::cache_map::get(&ref)))), written_(false)    {}

    template<class Y>
    inline upgrd_ptr & operator=(Y* ptr) { 
        ptr_=const_cast<detail::cache<T>*>(tx_->read_ptr(detail::cache_map::get(ptr)));
        return *this;
    }

    template<class Y>
    inline upgrd_ptr & operator=(Y& ref) { 
        ptr_=const_cast<detail::cache<T>*>(tx_->read_ptr(detail::cache_map::get(&ref)));
        return *this;
    }

    const T* get() const {
        if (tx_->forced_to_abort()) {
            tx_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }

        // we don't hold the written object and
        // we have alrerady written an object on this transaction maybe is this one
        if (!written_ && tx_->written()) {
            detail::cache<T>* temp = tx_->get_written(*ptr_);

            // if we found something, store this as the tx_ptr_
            if (0 != temp) {
                ptr_ = temp;
                written_ = true;
            }
        }

        return &ptr_->value;
    }

    inline T const & operator*() const { return *get(); }
    inline T const * operator->() const { return get(); }

    //operator const T*() const { return get(); }

    typedef detail::cache<T>* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
    inline void write_ptr(detail::cache<T>* ptr) {
        ptr_ = ptr;
        written_ = true;
    }

    T* write_ptr() {
        if (tx_->forced_to_abort()) {
            tx_->lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }

        // we are already holding the written object
        if (written_) return ptr_;

        detail::cache<T>* temp = tx_->get_written(ptr_);

        // if we found something, store this as the tx_ptr_
        if (0 != temp) {
            ptr_ = temp;
            written_ = true;
        } else {
            ptr_ = tx_->write_ptr(ptr_);
            written_ = true;
        }

        return ptr_;
    }

};

template <typename T>
inline void delete_ptr(upgrd_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    if (ptr.tx_==0) delete ptr.ptr_;
    ptr.tx_->delete_tx_ptr(ptr.ptr_);
}

template <typename T>
inline void delete_ptr(transaction& tx, upgrd_ptr<T> const& ptr) {
    if (ptr.ptr_==0) return;
    tx.delete_tx_ptr(ptr.ptr_);
}

template <typename T>
class wr_ptr {
    typedef wr_ptr<T> this_type;
public:
    mutable transaction& tx_;
    mutable detail::cache<T>* ptr_;

    inline wr_ptr(transaction &t, T* ptr) : tx_(t),
        ptr_(t.write_ptr(detail::cache_map::get(ptr)))
    {}

    inline wr_ptr(transaction &t, T& obj) : tx_(t),
        ptr_(t.write_ptr(detail::cache_map::get(&obj)))
    {}


    T* get() {
        if (tx_.forced_to_abort()) {
            tx_.lock_and_abort();
            throw aborted_transaction_exception("aborting transaction");
        }
        return ptr_->get();
    }

    inline T& operator*() { return *get(); }
    inline T* operator->() { return get(); }

    typedef detail::cache<T>* this_type::*unspecified_bool_type;

    inline operator unspecified_bool_type() const {
        return ptr_ == 0? 0: &this_type::ptr_;
    }
};


}
}}
#endif


