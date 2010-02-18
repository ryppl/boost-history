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

#ifndef BOOST_STM_TX_DEEP_TRANSACTION_OBJECT__HPP
#define BOOST_STM_TX_DEEP_TRANSACTION_OBJECT__HPP


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
// transaction object mixin
// Provides the definition of the virtual functions
//      make_cache: use copy constructor
//      copy_cache: use assignement
//      move_state and
//      delete_cache: use delete
// Defines in addition the functions new and delete when USE_STM_MEMORY_MANAGER is defined

// The parameter Base=base_transaction_object allows to mic transaction_object and polymorphism
// class B : deep_transaction_object<B> {}
// class D : deep_transaction_object<D, B> {}
// the single issue is the forward constructors from transaction_object<D, B> to B
//-----------------------------------------------------------------------------
template <class Final, typename Base=base_transaction_object>
class deep_transaction_object : public
#ifdef USE_STM_MEMORY_MANAGER
    memory_manager<Final, Base>
#else
    Base
#endif
{
    Final* final() {
        return static_cast<Final*>(this); }
    const Final* final() const {
        return static_cast<Final const*>(this); }
public:

    //--------------------------------------------------------------------------
#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    static  Final* make_cache(Final const* rhs, transaction& t) {
        Final* p = cache_allocate<Final>(t);
        ::new (p) Final(*rhs);
        return p;
    }
    virtual base_transaction_object* make_cache(transaction& t) const {
        return make_cache(final(), t);
    }
#else
    static Final* make_cache(Final const* rhs, transaction& ) {
        Final* tmp = new Final(*rhs);
        return tmp;
    }
    virtual base_transaction_object* make_cache(transaction& t) const {
        //~ Final* tmp = new Final(*boost::safe_polymorphic_downcast<Final const*>(this));
        //~ Final* tmp = new Final(*boost::safe_polymorphic_downcast_2<Final const*, Base const>(this));
        Final* tmp = new Final(*final());
        return tmp;
    }
#endif

   //--------------------------------------------------------------------------
#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    virtual void delete_cache() {
        final()->~Final();
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
       *final() =
       *boost::safe_polymorphic_downcast_2<Final const *, Base const>(&rhs);
   }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      *final() = draco_move
         (*(boost::safe_polymorphic_downcast_2<Final*, Base>(rhs)));
   }
#endif

};

#if 0

#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
template <class Final, typename Base>
Final*
deep_transaction_object<Final,Base>::
make_cache(Final const* rhs, transaction& t) {
        Final* p = cache_allocate<Final>(t);
        ::new (p) Final(*rhs);
        return p;
    }
template <class Final, typename Base>
base_transaction_object*
deep_transaction_object<Final,Base>::
make_cache(transaction& t) const {
        Final const* f=boost::safe_polymorphic_downcast_2<Final const*, Base const>(this);
        return make_cache(f, t);
    }
#else
template <class Final, typename Base>
Final*
deep_transaction_object<Final,Base>::
make_cache(Final const* rhs, transaction& ) {
        Final* tmp = new Final(*rhs);
        return tmp;
    }
template <class Final, typename Base>
base_transaction_object*
deep_transaction_object<Final,Base>::
make_cache(transaction& t) const {
        Final* tmp = new Final(*boost::safe_polymorphic_downcast_2<Final const*, Base const>(this));
        return tmp;
    }
#endif
#endif



template <class Final, class Base1, class Base2>
class deep_transaction_object2 :
#ifdef USE_STM_MEMORY_MANAGER
    public memory_manager<Final, Base1>, public Base2
#else
    public Base1, public Base2
#endif
{
public:

    //--------------------------------------------------------------------------
#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    static  Final* make_cache(Final const& rhs, transaction& t) {
        Final* p = cache_allocate<Final>(t);
        ::new (p) Final(*boost::safe_polymorphic_downcast_3<Final const*, Base1 const, Base2 const>(&rhs));
        return p;
    };
    virtual base_transaction_object* make_cache(transaction& t) const {
        Final const& f=*boost::safe_polymorphic_downcast_3<Final const*, Base1 const, Base2 const>(this);
        return make_cache(f, t);
    }
#else
    static Final* make_cache(Final const& rhs, transaction& ) {
        Final* tmp = new Final(*boost::safe_polymorphic_downcast_3<Final const*, Base1 const, Base2 const>(&rhs));
        return tmp;
    };
    virtual base_transaction_object* make_cache(transaction& t) const {
        Final* tmp = new Final(*boost::safe_polymorphic_downcast_3<Final const*, Base1 const, Base2 const>(this));
        return tmp;
    }
#endif

   //--------------------------------------------------------------------------
#if BOOST_STM_USE_SPECIFIC_TRANSACTION_MEMORY_MANAGER
    virtual void delete_cache() {
        boost::safe_polymorphic_downcast_3<Final*, Base1, Base2>(this)->~Final();
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
       *boost::safe_polymorphic_downcast_3<Final *, Base1, Base2>(this) = *boost::safe_polymorphic_downcast_3<Final const *, Base1 const, Base2 const>(&rhs);
   }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      *boost::safe_polymorphic_downcast_3<Final *, Base1, Base2>(this) = draco_move
         (*(boost::safe_polymorphic_downcast_3<Final*, Base1, Base2>(rhs)));
   }
#endif

};


}}

//-----------------------------------------------------------------------------
#endif // BOOST_STM_TX_DEEP_TRANSACTION_OBJECT__HPP


