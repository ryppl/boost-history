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

#ifndef BOOST_STM_MEMORY_MANAGER__HPP
#define BOOST_STM_MEMORY_MANAGER__HPP

//-----------------------------------------------------------------------------
//#include <stdarg.h>
//~ #include <pthread.h>
#include <boost/thread.hpp>

//-----------------------------------------------------------------------------
#include <list>
#include <exception>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/base_transaction_object.hpp>
#include <boost/stm/datatypes.hpp>
#include <boost/stm/memory_managers/base_memory_manager.hpp>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// memory_manager mixin
// Functions new and delete

// The parameter Base allows to mix memory_manager and polymorphism
// class B : memory_manager<B> {}
// class D : memory_manager<D, B> {}
// the single issue is the forward constructors from memory_manager<D, B> to B
//-----------------------------------------------------------------------------
template <class Derived, typename Base>
class memory_manager : public Base
{
public:
    typedef memory_manager<Derived, Base> this_type;

    void* operator new(std::size_t size, const std::nothrow_t&) throw () {
        return base_memory_manager::retrieve_mem(size);
    }

    void* operator new(std::size_t size) throw (std::bad_alloc) {
        void* ptr= base_memory_manager::retrieve_mem(size);
        if (ptr==0) throw std::bad_alloc();
        return ptr;
    }

    void operator delete(void* mem) throw () {
        base_memory_manager::return_mem(mem, sizeof(Derived));
   }
};

}}
#endif // BOOST_STM_MEMORY_MANAGER__HPP


