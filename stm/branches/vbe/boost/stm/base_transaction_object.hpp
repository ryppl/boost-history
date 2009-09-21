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

#ifndef BOOST_STM_BASE_TRANSACTION_OBJECT__HPP
#define BOOST_STM_BASE_TRANSACTION_OBJECT__HPP

//-----------------------------------------------------------------------------
//#include <stdarg.h>
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
// this is the base class of all the transactional objects.
// it tracks:
//      transactionThread_: the thread identifier holding the write acces to this transactional object
//      version: the version when performing validation
//      newMemory_: states whether this object is a new object
// transactional objets must specialize the pure virtual functions
//      copy_state(base_transaction_object const * const rhs)
//      move_state(base_transaction_object * rhs) if BUILD_MOVE_SEMANTICS
//      cache_deallocate() if BOOST_STM_USE_MEMCOPY
// copy_state is used to copy the backup/working copy to the shared transactional object when the roolback/commit is done direct/defered policy is used
// move_state is used to move the backup/working copy to the shared transactional object when the roolback/commit is done direct/defered policy is used
// cache_deallocate is used to release the backup/working copy when the transaction ends if direct/defered policy is used
// when USE_STM_MEMORY_MANAGER is defined this class provides two functions (retrieve_mem and return_mem) and  to manage a pool of memory
//-----------------------------------------------------------------------------

class base_transaction_object
{
public:

   base_transaction_object() : transactionThread_(kInvalidThread),
      newMemory_(0)
#if PERFORMING_VALIDATION
      ,version_(0)
#endif
   {}

#if 1
    base_transaction_object(const base_transaction_object &t)
        : transactionThread_(kInvalidThread)
        , newMemory_(0)
#if PERFORMING_VALIDATION
        , version_(0)
#endif
   {}
#endif

   virtual base_transaction_object* clone() const = 0;
   virtual void copy_state(base_transaction_object const * const rhs) = 0;
#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs) = 0;
#else
   virtual void move_state(base_transaction_object * rhs) {};
#endif
   virtual ~base_transaction_object() {};
#ifdef BOOST_STM_USE_MEMCOPY
    virtual void cache_deallocate()=0;
#endif

   void transaction_thread(size_t rhs) const { transactionThread_ = rhs; }
   size_t const & transaction_thread() const { return transactionThread_; }

#if USE_STM_MEMORY_MANAGER
   static void alloc_size(size_t size) { memory_.alloc_size(size); }
#else
   static void alloc_size(size_t size) { }
#endif

   void new_memory(size_t rhs) const { newMemory_ = rhs; }
   size_t const & new_memory() const { return newMemory_; }

#if PERFORMING_VALIDATION
   size_t version_;
#endif

//protected:

#if USE_STM_MEMORY_MANAGER
    static void return_mem(void *mem, size_t size)
    {
#ifndef BOOST_STM_USE_BOOST_MUTEX
      lock(&transactionObjectMutex_);
        memory_.returnChunk(mem, size);
      unlock(&transactionObjectMutex_);
#else
        boost::lock_guard<boost::mutex> lock(transactionObjectMutex_);
        memory_.returnChunk(mem, size);
#endif
    }

   static void* retrieve_mem(size_t size)
   {
#ifndef BOOST_STM_USE_BOOST_MUTEX
        lock(&transactionObjectMutex_);
        void *mem = memory_.retrieveChunk(size);
        unlock(&transactionObjectMutex_);
#else
        boost::lock_guard<boost::mutex> lock(transactionObjectMutex_);
        void *mem = memory_.retrieveChunk(size);
#endif

      return mem;
   }
#endif

private:

   //--------------------------------------------------------------------------
   // int instead of bool for architecture word-boundary alignment
   //
   // transactionThread_ means a transaction is writing to this memory.
   //
   // in direct environments, this flag means memory being written to directly
   //
   // in deferred environments, this flag means this is memory that was copied
   // and being written to off to the side
   //
   // it's important to note the differences between as direct reads and writes
   // and deferred reads and writes behave very differently when using this
   // flag.
   //--------------------------------------------------------------------------
   mutable size_t transactionThread_;

   mutable size_t newMemory_;
#if USE_STM_MEMORY_MANAGER
   static Mutex transactionObjectMutex_;
   static MemoryPool<base_transaction_object> memory_;
#endif
};

} // namespace core
}
#endif // BOOST_STM_BASE_TRANSACTION_OBJECT__HPP


