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
//~ #include <pthread.h>
#include <boost/thread.hpp>

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
//#include <boost/stm/tx/trivial_transaction_object.hpp>
#include <boost/stm/tx/shallow_transaction_object.hpp>

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

template <class B>
struct virtually : virtual B {
    // forward constructors
};

namespace detail {
template <class Final, class Base,
   bool hasShallowCopySemantics
//, bool hasTrivialCopySemantics
>
class transaction_object_aux;

//~ template <class F, class B>
//~ class transaction_object_aux<F, B, true, true>:
    //~ public trivial_transaction_object<F, B> {};
template <class F, class B>
class transaction_object_aux<F, B, true>:
    public shallow_transaction_object<F, B> {};
//~ template <class F, class B>
//~ class transaction_object_aux<F, B, false, true>:
    //~ public trivial_transaction_object<F, B> {};
template <class F, class B>
class transaction_object_aux<F, B, false>:
    public deep_transaction_object<F, B> {};

template <class Final, class Base1,class Base2,
   bool hasShallowCopySemantics
//~ , bool hasTrivialCopySemantics
>
class transaction_object2_aux;

//~ template <class F, class B1, class B2>
//~ class transaction_object2_aux<F, B1, B2, true, true>:
    //~ public trivial_transaction_object2<F, B1, B2> {};
template <class F, class B1, class B2>
class transaction_object2_aux<F, B1, B2, true>:
    public shallow_transaction_object2<F, B1, B2> {};
//~ template <class F, class B1, class B2>
//~ class transaction_object2_aux<F, B1, B2, false, true>:
    //~ public trivial_transaction_object2<F, B1,B2> {};
template <class F, class B1, class B2>
class transaction_object2_aux<F, B1, B2, false>:
    public deep_transaction_object2<F, B1, B2> {};

}

template <
    class Final,
    class Base=base_transaction_object
>
class transaction_object : public detail::transaction_object_aux<Final, Base,
    has_shallow_copy_semantics<Final>::value
//~ , has_trivial_copy_semantics<Final>::value
    >
{};

template <
    class Final,
    class Base1,
    class Base2
>
class transaction_object2 : public detail::transaction_object2_aux<Final, Base1, Base2,
    has_shallow_copy_semantics<Final>::value
//~ ,   has_trivial_copy_semantics<Final>::value
    >
{};

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


