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

#ifndef BOOST_STM_TX_SHALLOW_TRANSACTION_OBJECT__HPP
#define BOOST_STM_TX_SHALLOW_TRANSACTION_OBJECT__HPP

//-----------------------------------------------------------------------------
//#include <stdarg.h>
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/base_transaction_object.hpp>
#include <boost/stm/cache_fct.hpp>
#include <boost/stm/datatypes.hpp>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class transaction;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// transaction object mixin making shallow copy
// Provides the definition of the virtual functions
//      clone: use copy constructor
//      copy_state: use assignement
//      move_state and
//      cache_deallocate: use delete
// Defines in addition the functions new and delete when USE_STM_MEMORY_MANAGER is defined

// The parameter Base=base_transaction_object allows to mic transaction_object and polymorphism
// class B : transaction_object<B> {}
// class D : transaction_object<D, B> {}
// the single issue is the forward constructors from transaction_object<D, B> to B
//-----------------------------------------------------------------------------

template <class Derived, typename Base=base_transaction_object>
class shallow_transaction_object : public base_transaction_object
{
public:

    //--------------------------------------------------------------------------
    virtual base_transaction_object* clone(transaction* t) const {
        Derived* tmp = cache_clone(t, *static_cast<Derived const*>(this));
        return tmp;
    }

   //--------------------------------------------------------------------------
    virtual void cache_deallocate() {
        boost::stm::cache_deallocate(this);
    }

   //--------------------------------------------------------------------------
   virtual void copy_state(base_transaction_object const * const rhs)
   {
        boost::stm::cache_copy(static_cast<Derived const * const>(rhs), static_cast<Derived *>(this));
   }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      static_cast<Derived &>(*this) = draco_move
         (*(static_cast<Derived*>(rhs)));
   }
#endif


#if USE_STM_MEMORY_MANAGER
   void* operator new(std::size_t size, const std::nothrow_t&) throw ()
   {
      return retrieve_mem(size);
   }
    void* operator new(std::size_t size) throw (std::bad_alloc)
    {
        void* ptr= retrieve_mem(size);
        if (ptr==0) throw std::bad_alloc;
        return ptr;
    }

   void operator delete(void* mem) throw ()
   {
      static Derived elem;
      static std::size_t elemSize = sizeof(elem);
      return_mem(mem, elemSize);
   }
#endif

};

template <typename T> class shallow_native_trans :
public shallow_transaction_object< shallow_native_trans<T> >
{
public:

   shallow_native_trans() : value_(T()) {}
   shallow_native_trans(T const &rhs) : value_(rhs) {}
   shallow_native_trans(shallow_native_trans const &rhs) : value_(rhs.value_) {}
   ~shallow_native_trans() {}

   shallow_native_trans& operator=(T const &rhs) { value_ = rhs; return *this; }

   shallow_native_trans& operator--() { --value_; return *this; }
   shallow_native_trans operator--(int) { shallow_native_trans n = *this; --value_; return n; }

   shallow_native_trans& operator++() { ++value_; return *this; }
   shallow_native_trans operator++(int) { shallow_native_trans n = *this; ++value_; return n; }

   shallow_native_trans& operator+=(T const &rhs)
   {
      value_ += rhs;
      return *this;
   }

   shallow_native_trans operator+(shallow_native_trans const &rhs)
   {
      shallow_native_trans ret = *this;
      ret.value_ += rhs.value_;
      return ret;
   }

   //template <>
   operator T() const
   {
      return this->value_;
   }

#if BUILD_MOVE_SEMANTICS
   //--------------------------------------------------
   // move semantics
   //--------------------------------------------------
   shallow_native_trans(shallow_native_trans &&rhs) { value_ = rhs.value_;}
   shallow_native_trans& operator=(shallow_native_trans &&rhs)
   { value_ = rhs.value_; return *this; }
#endif

   T& value() { return value_; }
   T const & value() const { return value_; }

private:
   T value_;
};

}}
#endif // BOOST_STM_SHALLOW_TRANSACTION_OBJECT__HPP


