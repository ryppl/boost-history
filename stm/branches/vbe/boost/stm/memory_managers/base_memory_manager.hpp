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

#ifndef BOOST_STM_BASE_MEMORY_MANAGER__HPP
#define BOOST_STM_BASE_MEMORY_MANAGER__HPP

//-----------------------------------------------------------------------------
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/datatypes.hpp>
#include <boost/stm/synchro.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/memory_pool.hpp>
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
// this class provides two functions (retrieve_mem and return_mem) and  
// to manage a pool of memory
//-----------------------------------------------------------------------------

class base_memory_manager
{
    base_memory_manager();
public:
    static void alloc_size(std::size_t size) { memory_.alloc_size(size); }

    static void return_mem(void *mem, std::size_t size)
    {
        synchro::lock_guard<Mutex> lock(transactionObjectMutex_);
        memory_.returnChunk(mem, size);
    }

    static void* retrieve_mem(std::size_t size)
    {
        synchro::lock_guard<Mutex> lock(transactionObjectMutex_);
        void *mem = memory_.retrieveChunk(size);
        return mem;
    }

private:

    static Mutex transactionObjectMutex_;
    static MemoryPool<void*> memory_;
};

} // namespace core
}
#endif // BOOST_STM_BASE_MEMORY_MANAGER__HPP


