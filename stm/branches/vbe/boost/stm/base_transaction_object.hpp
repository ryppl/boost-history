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
#include <pthread.h>
//-----------------------------------------------------------------------------
#include <list>
//-----------------------------------------------------------------------------
#include <boost/synchro.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
//-----------------------------------------------------------------------------
#include <boost/stm/datatypes.hpp>

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
//      cache_deallocate()
// copy_state is used to copy the backup/working copy to the shared transactional object when the roolback/commit is done direct/defered policy is used
// move_state is used to move the backup/working copy to the shared transactional object when the roolback/commit is done direct/defered policy is used
// cache_deallocate is used to release the backup/working copy when the transaction ends if direct/defered policy is used
// when USE_STM_MEMORY_MANAGER is defined this class provides two functions (retrieve_mem and return_mem) and  to manage a pool of memory
//-----------------------------------------------------------------------------

class base_transaction_object
{
public:

    base_transaction_object()
        : transactionThread_(invalid_thread_id())
        , newMemory_(0)
#if PERFORMING_VALIDATION
        , version_(0)
#endif
#if BOOST_STM_ALLOWS_EMBEDEEDS
        , embeddeds_()
#endif
   {}

#if 1
    base_transaction_object(const base_transaction_object &t)
        : transactionThread_(invalid_thread_id())
        , newMemory_(0)
#if PERFORMING_VALIDATION
        , version_(0)
#endif
#if BOOST_STM_ALLOWS_EMBEDEEDS
        , embeddeds_()
#endif
    {}
#endif

    virtual base_transaction_object* clone(transaction* t) const = 0;
    virtual void copy_state(base_transaction_object const * const rhs) = 0;
#if BUILD_MOVE_SEMANTICS
    virtual void move_state(base_transaction_object * rhs) = 0;
#else
    virtual void move_state(base_transaction_object * rhs) {};
#endif
    virtual ~base_transaction_object() {};
    virtual void cache_deallocate()=0;

    void transaction_thread(thread_id_t rhs) const { transactionThread_ = rhs; }
    thread_id_t const & transaction_thread() const { return transactionThread_; }

    void new_memory(new_memory_t rhs) const { newMemory_ = rhs; }
    new_memory_t const & new_memory() const { return newMemory_; }

#if PERFORMING_VALIDATION
    version_t version_;
#endif

#if BOOST_STM_ALLOWS_EMBEDEEDS
    std::list<base_transaction_object*>& binds() {return embeddeds_;}
    void bind(base_transaction_object* bto) {embeddeds_.push_back(bto);}
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
    mutable thread_id_t transactionThread_;
    mutable new_memory_t newMemory_;
#if BOOST_STM_ALLOWS_EMBEDEEDS
    std::list<base_transaction_object*> embeddeds_;
#endif
};

} // namespace core
}
#endif // BOOST_STM_BASE_TRANSACTION_OBJECT__HPP


