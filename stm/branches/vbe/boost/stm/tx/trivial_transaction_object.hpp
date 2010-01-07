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

#ifndef BOOST_STM_TX_TRIVIAL_TRANSACTION_OBJECT__HPP
#define BOOST_STM_TX_TRIVIAL_TRANSACTION_OBJECT__HPP

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
#include <boost/stm/memory_managers/memory_manager.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/has_trivial_copy.hpp>
#include <boost/type_traits/has_trivial_assign.hpp>

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

template <class T>
struct has_trivial_copy_semantics :
   boost::mpl::and_<
        boost::has_trivial_copy_constructor<T>,
        boost::has_trivial_assign<T> 
  >
 {};

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class transaction;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// transaction object mixin making a trivial memcpy copy
// Provides the definition of the virtual functions
//      make_cache: use memcpy on cache_allocated
//      copy_state: use memcpy 
//      move_state and
//      delete_cache: use cache_allocated
// Defines in addition the functions new and delete when USE_STM_MEMORY_MANAGER is defined

// The parameter Base=base_transaction_object allows to mic transaction_object and polymorphism
// class B : transaction_object<B> {}
// class D : transaction_object<D, B> {}
// the single issue is the forward constructors from transaction_object<D, B> to B
//-----------------------------------------------------------------------------

template <class Final, typename Base=base_transaction_object>
class trivial_transaction_object : public
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

    //--------------------------------------------------------------------------
    virtual base_transaction_object* make_cache(transaction* t) const {
        Final* p = cache_allocate<Final>(t);
        boost::stm::cache_copy(static_cast<Final const*>(this), p);
        return p;
    }

   //--------------------------------------------------------------------------
    virtual void delete_cache() {
        boost::stm::cache_deallocate(this);
    }

   //--------------------------------------------------------------------------
   virtual void copy_state(base_transaction_object const * const rhs)
   {
        boost::stm::cache_copy(static_cast<Final const * const>(rhs), static_cast<Final *>(this));
   }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      static_cast<Final &>(*this) = draco_move
         (*(static_cast<Final*>(rhs)));
   }
#endif

};


}}
#endif // BOOST_STM_TX_TRIVIAL_TRANSACTION_OBJECT__HPP


