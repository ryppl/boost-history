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

#ifndef BOOST_STM_DETAIL_DELETERS__HPP
#define BOOST_STM_DETAIL_DELETERS__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/base_transaction.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/datatypes.hpp>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm { namespace detail {
struct deleter {
    virtual void reset()=0;
    virtual void release()=0;
};

template <typename T>
struct base_transaction_object_deleter  : deleter {
    T* ptr_;   
    base_transaction_object_deleter(T* ptr) : ptr_(ptr) {}
    virtual void reset() {
        static_cast<base_transaction_object*>(ptr_)->transaction_thread(kInvalidThread);
    };
    virtual void release() {
        delete ptr_;
    };
};

template <typename T>
struct base_transaction_object_array_deleter  : deleter {
    T* ptr_;   
    std::size_t size_;
    base_transaction_object_array_deleter (T* ptr, std::size_t size) : ptr_(ptr), size_(size) {}
    virtual void reset() {
        for (std::size_t i =0; i< size_; ++i) {
            static_cast<base_transaction_object*>(ptr_+i)->transaction_thread(kInvalidThread);
            static_cast<base_transaction_object*>(ptr_+i)->new_memory(0);
        }
    };
    virtual void release() {
        delete [] ptr_;
    };
};

template <typename T>
struct non_transaction_object_deleter  : deleter {
    T* ptr_;   
    typedef typename T::binds_list binds_type;
    typedef typename T::binds_list::iterator binds_iterator;

    non_transaction_object_deleter(T* ptr) : ptr_(ptr){}
    virtual void reset() {
        binds_type& binds= ptr_->binds();
        for (binds_iterator it =binds.begin(); it!= binds.end(); ++it) {
            static_cast<base_transaction_object*>(*it)->transaction_thread(kInvalidThread);
            static_cast<base_transaction_object*>(*it)->new_memory(0);
        }
    };
    virtual void release() {
        delete ptr_;
    };
};

// For array of non transactional object types 

template <typename T>
struct non_transaction_object_array_deleter : deleter {
    T* ptr_;   
    std::size_t size_;
    typedef typename T::binds_list binds_type;
    typedef typename T::binds_list::iterator binds_iterator;

    non_transaction_object_array_deleter(T* ptr, std::size_t size) : ptr_(ptr), size_(size) {}
    virtual void reset() {
        for (std::size_t i =0; i< size_; ++i) {
            binds_type& binds= (ptr_+i)->binds();
            for (binds_iterator it =binds.begin(); it!= binds.end(); ++it) {
                static_cast<base_transaction_object*>(*it)->transaction_thread(kInvalidThread);
                static_cast<base_transaction_object*>(*it)->new_memory(0);
            }
        }
    };
    virtual void release() {
        delete [] ptr_;
    };
};


#ifndef BOOST_STM_ALLOWS_DELETERS
    typedef base_transaction_object deleter_type;
    inline void reset(deleter_type* ptr) {
        ptr->transaction_thread(kInvalidThread);
        ptr->new_memory(0);
    }
    inline void release(deleter_type* ptr) {
        delete ptr;
    }
    
    inline deleter_type* make(base_transaction_object* p) {
        return p;
    }
    inline deleter_type* make(base_transaction_object& r) {
        return &r;
    }
    inline deleter_type* make(base_transaction_object const* p) {
        return const_cast<deleter_type*>(p);
    }
    inline deleter_type* make(base_transaction_object const & r) {
        return const_cast<deleter_type*>(&r);
    }

    inline deleter_type* make_array(base_transaction_object* p, std::size_t size) {
        return p;
    }
    inline deleter_type* make_array(base_transaction_object& r, std::size_t size) {
        return &r;
    }
    inline deleter_type* make_array(base_transaction_object const* p, std::size_t size) {
        return const_cast<deleter_type*>(p);
    }
    inline deleter_type* make_array(base_transaction_object const & r, std::size_t size) {
        return const_cast<deleter_type*>(&r);
    }
    
#else
    typedef detail::deleter deleter_type;
    inline void reset(deleter_type* ptr) {
        ptr->reset();
        delete ptr;
    }
    inline void release(deleter_type* ptr) {
        ptr->release();
        delete ptr;
    }
    
    template <typename T>
    inline deleter_type* make(T* p) {
        return new base_transaction_object_deleter<T>(p);
    }
    template <typename T>
    inline deleter_type* make(T& r) {
        return new base_transaction_object_deleter<T>(&r);
    }
    template <typename T>
    inline deleter_type* make(T const* p) {
        return new base_transaction_object_deleter<T>(const_cast<T*>(p));
    }
    template <typename T>
    inline deleter_type* make(T const& r) {
        return new base_transaction_object_deleter<T>(const_cast<T*>(&r));
    }
    
    template <typename T>
    inline deleter_type* make_array(T* p, std::size_t size) {
        return new base_transaction_object_array_deleter<T>(p, size);
    }
    template <typename T>
    inline deleter_type* make_array(T& r, std::size_t size) {
        return new base_transaction_object_array_deleter<T>(&r, size);
    }
    template <typename T>
    inline deleter_type* make_array(T const* p, std::size_t size) {
        return new base_transaction_object_array_deleter<T>(const_cast<T*>(p), size);
    }
    template <typename T>
    inline deleter_type* make_array(T const& r, std::size_t size) {
        return new base_transaction_object_array_deleter<T>(const_cast<T*>(&r, size));
    }
    
#endif
}}}
///////////////////////////////////////////////////////////////////////////////
#endif // BOOST_STM_DETAIL_DELETERS__HPP


