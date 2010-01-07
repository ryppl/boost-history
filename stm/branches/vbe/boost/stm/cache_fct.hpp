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

#ifndef BOOST_STM_CACHE_FCT__HPP
#define BOOST_STM_CACHE_FCT__HPP

//-----------------------------------------------------------------------------
#include <stdarg.h>
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
//#include <boost/stm/exceptions.hpp>
//-----------------------------------------------------------------------------
//#include <boost/stm/detail/memory_pool.hpp>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

class base_transaction_object;
class transaction;

template <class T> T* cache_clone(const T& val);
template <class T> void cache_copy(const T* const ori, T* target);
void cache_release(base_transaction_object* ptr);


template <class T> T* cache_allocate(transaction*);
template <class T> void cache_deallocate(T*);


} // namespace core
}
#endif // BOOST_STM_CACHE_FCT__HPP


