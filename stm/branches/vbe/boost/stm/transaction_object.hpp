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

#ifndef BOOST_STM_TRANSACTION_OBJECT__HPP
#define BOOST_STM_TRANSACTION_OBJECT__HPP

//-----------------------------------------------------------------------------
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
#include <new>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/base_transaction_object.hpp>
#include <boost/stm/cache_fct.hpp>
#include <boost/stm/datatypes.hpp>
#include <boost/stm/memory_managers/memory_manager.hpp>
#include <boost/stm/tx/deep_transaction_object.hpp>
#include <boost/stm/tx/trivial_transaction_object.hpp>
#include <boost/stm/tx/shallow_transaction_object.hpp>

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

namespace detail {
template <class Final, class Base,
   bool hasShallowCopySemantics,
   bool hasTrivialCopySemantics>
class transaction_object_aux;

template <class F, class B>
class transaction_object_aux<F, B, true, true>:
    public trivial_transaction_object<F, B> {};
template <class F, class B>
class transaction_object_aux<F, B, true, false>:
    public shallow_transaction_object<F, B> {};
template <class F, class B>
class transaction_object_aux<F, B, false, true>:
    public trivial_transaction_object<F, B> {};
template <class F, typename B>
class transaction_object_aux<F, B, false, false>:
    public deep_transaction_object<F, B> {};
}
#if 1

template <
    class Final,
    class Base=base_transaction_object
>
class transaction_object : public detail::transaction_object_aux<Final, Base,
    has_shallow_copy_semantics<Final>::value,
    has_trivial_copy_semantics<Final>::value
    >
{};
    #else
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// transaction object mixin
// Provides the definition of the virtual functions
//      make_cache: use copy constructor
//      copy_cache: use assignement
//      move_state and
//      delete_cache: use delete
// Defines in addition the functions new and delete when USE_STM_MEMORY_MANAGER is defined

// The parameter Base=base_transaction_object allows to mic transaction_object and polymorphism
// class B : transaction_object<B> {}
// class D : transaction_object<D, B> {}
// the single issue is the forward constructors from transaction_object<D, B> to B
//-----------------------------------------------------------------------------
template <class Final, typename Base=base_transaction_object>
class transaction_object : public
#ifdef USE_STM_MEMORY_MANAGER
    memory_manager<Final, Base>
#else
    Base
#endif
{
#ifdef USE_STM_MEMORY_MANAGER
    typedef memory_manager<Final, Base> base_type;
#else
    typedef Base base_type;
#endif
public:
    typedef transaction_object<Final, Base> this_type;

    //--------------------------------------------------------------------------
#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    virtual base_transaction_object* make_cache(transaction* t) const {
        Final* p = cache_allocate<Final>(t);
        if (p==0) {
            throw std::bad_alloc();
        }
        ::new (p) Final(*static_cast<Final const*>(this));
        return p;
    }
#else
    virtual base_transaction_object* make_cache(transaction*) const {
        Final* tmp = new Final(*static_cast<Final const*>(this));
        return tmp;
    }
#endif

   //--------------------------------------------------------------------------
#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    virtual void delete_cache() {
        static_cast<Final*>(this)->~Final();
        boost::stm::cache_deallocate(this);
    }
#else
    virtual void delete_cache() {
        delete this;
    }
#endif

   //--------------------------------------------------------------------------
   virtual void copy_cache(base_transaction_object const & rhs)
   {
       *static_cast<Final *>(this) = *static_cast<Final const *>(&rhs);
   }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      static_cast<Final &>(*this) = draco_move
         (*(static_cast<Final*>(rhs)));
   }
#endif

};
#endif
template <typename T> class native_trans :
public transaction_object< native_trans<T> >
{
public:

   native_trans() : value_(T()) {}
   native_trans(T const &rhs) : value_(rhs) {}
   native_trans(native_trans const &rhs) : value_(rhs.value_) {}
   ~native_trans() {}

   native_trans& operator=(T const &rhs) { value_ = rhs; return *this; }

   native_trans& operator--() { --value_; return *this; }
   native_trans operator--(int) { native_trans n = *this; --value_; return n; }

   native_trans& operator++() { ++value_; return *this; }
   native_trans operator++(int) { native_trans n = *this; ++value_; return n; }

   native_trans& operator+=(T const &rhs)
   {
      value_ += rhs;
      return *this;
   }

   native_trans operator+(native_trans const &rhs)
   {
      native_trans ret = *this;
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
   native_trans(native_trans &&rhs) { value_ = rhs.value_;}
   native_trans& operator=(native_trans &&rhs)
   { value_ = rhs.value_; return *this; }
#endif

   T& value() { return value_; }
   T const & value() const { return value_; }

private:
   T value_;
};

}}
#endif // BOOST_STM_TRANSACTION_OBJECT__HPP


