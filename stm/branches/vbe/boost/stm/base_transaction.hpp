//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

/* The DRACO Research Group (rogue.colorado.edu/draco) */
/*****************************************************************************\
 *
 * Copyright Notices/Identification of Licensor(s) of
 * Original Software in the File
 *
 * Copyright 2000-2006 The Board of Trustees of the University of Colorado
 * Contact: Technology Transfer Office,
 * University of Colorado at Boulder;
 * https://www.cu.edu/techtransfer/
 *
 * All rights reserved by the foregoing, respectively.
 *
 * This is licensed software.  The software license agreement with
 * the University of Colorado specifies the terms and conditions
 * for use and redistribution.
 *
\*****************************************************************************/

#ifndef BOOST_STM_BASE_TRANSACTION_H
#define BOOST_STM_BASE_TRANSACTION_H


#include <pthread.h>
//#include <boost/thread/mutex.hpp>
//#include <boost/thread/locks.hpp>

#include <boost/stm/detail/memory_pool.hpp>
#include <stdarg.h>
#include <list>

#ifndef BOOST_STM_USE_BOOST_MUTEX
   typedef pthread_mutex_t Mutex;
#else
   typedef boost::mutex Mutex;
#endif

//-----------------------------------------------------------------------------
namespace boost { namespace stm {

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

template <class T> void cache_deallocate(T*);
template <class T> void cache_restore(const T* const ori, T* target);
template <class T> inline T* cache_new_copy_constructor(const T&);
class transaction;

#ifndef BOOST_STM_USE_BOOST_MUTEX
typedef pthread_mutex_t PLOCK;
#else
typedef boost::mutex PLOCK;
#endif

//-----------------------------------------------------------------------------
// boolean which is used to invoke "begin_transaction()" upon transaction
// object construction (so two lines of code aren't needed to make a
// transaction start, in case the client wants to start the transaction
// immediately).
//-----------------------------------------------------------------------------
bool const begin_transaction = true;

unsigned const kInvalidThread = 0xffffffff;

//-----------------------------------------------------------------------------
// The possible states a transaction can be in:
//
// e_no_state    - initial state of transaction.
// e_aborted     - aborted transaction.
// e_committed   - transaction has committed.
// e_hand_off    - transaction memory has been handed off to another
//                 transaction. This is the vital state for in-flight
//                 transactions which are composed.
// e_in_flight   - transaction currently in process.
//-----------------------------------------------------------------------------
enum transaction_state
{
   e_no_state = -1, // no state is -1
   e_aborted,      // ensure aborted = 0
   e_committed,    // committed is positive
   e_hand_off,      // so is handoff in case bool conversion
   e_in_flight
};

#if BUILD_MOVE_SEMANTICS
template <class T>
inline typename std::remove_reference<T>::type&& draco_move(T &&t)
{
   return t;
}

bool const kDracoMoveSemanticsCompiled = true;
#else
bool const kDracoMoveSemanticsCompiled = false;
#endif

class aborted_transaction_exception_no_unlocks {};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class aborted_transaction_exception : public std::exception
{
public:
   aborted_transaction_exception(char const * const what) : what_(what) {}

   //virtual char const * what() const { return what_; }

private:
   char const * const what_;
};

typedef aborted_transaction_exception aborted_tx;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef BOOST_STM_USE_BOOST_MUTEX

#if 0
   template <typename T>
   inline int lock(T *lock) { throw "unsupported lock type"; }

   template <typename T>
   inline int trylock(T *lock) { throw "unsupported lock type"; }

   template <typename T>
   inline int unlock(T *lock) { throw "unsupported lock type"; }

   template <>
   inline int lock(Mutex &lock) { return pthread_mutex_lock(&lock); }

   template <>
   inline int lock(Mutex *lock) { return pthread_mutex_lock(lock); }

   template <>
   inline int trylock(Mutex &lock) { return pthread_mutex_trylock(&lock); }

   template <>
   inline int trylock(Mutex *lock) { return pthread_mutex_trylock(lock); }

   template <>
   inline int unlock(Mutex &lock) { return pthread_mutex_unlock(&lock); }

   template <>
   inline int unlock(Mutex *lock) { return pthread_unlock(lock); }
#else
   inline int lock(PLOCK &lock) { return pthread_mutex_lock(&lock); }
   inline int lock(PLOCK *lock) { return pthread_mutex_lock(lock); }

   inline int trylock(PLOCK &lock) { return pthread_mutex_trylock(&lock); }
   inline int trylock(PLOCK *lock) { return pthread_mutex_trylock(lock); }

   inline int unlock(PLOCK &lock) { return pthread_mutex_unlock(&lock); }
   inline int unlock(PLOCK *lock) { return pthread_mutex_unlock(lock); }
#endif
#else
   inline void lock(PLOCK &lock) { lock.lock(); }
   inline void lock(PLOCK *lock) { lock->lock(); }

   inline bool trylock(PLOCK &lock) { return lock.try_lock(); }
   inline bool trylock(PLOCK *lock) { return lock->try_lock(); }

   inline void unlock(PLOCK &lock) { lock.unlock(); }
   inline void unlock(PLOCK *lock) { lock->unlock(); }
#endif

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

#if 0
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


//-----------------------------------------------------------------------------
// transaction object mixin
// Provides the definition of the virtual functions
//      copy_state: relaying on the cache_restore<T> generic function
//      move_state and
//      cache_deallocate: relaying on the cache_restore<T> generic function
// Defines in addition the functions new and delete when USE_STM_MEMORY_MANAGER is defined

// The parameter Base=base_transaction_object allows to mic transaction_object and polymorphism
// class B : transaction_object<B> {}
// class D : transaction_object<D, B> {}
// the single issue is the forward constructors from transaction_object<D, B> to B
//-----------------------------------------------------------------------------
template <class Derived, typename Base=base_transaction_object>
class transaction_object : public base_transaction_object
{
public:

    //--------------------------------------------------------------------------
    virtual base_transaction_object* clone() const {
        return cache_new_copy(*this);
    }

   //--------------------------------------------------------------------------
   virtual void copy_state(base_transaction_object const * const rhs)
   {
        boost::stm::cache_restore(static_cast<Derived const * const>(rhs), static_cast<Derived *>(this));
   }

#if BUILD_MOVE_SEMANTICS
   virtual void move_state(base_transaction_object * rhs)
   {
      static_cast<Derived &>(*this) = draco_move
         (*(static_cast<Derived*>(rhs)));
   }
#endif

#ifdef BOOST_STM_USE_MEMCOPY
    virtual void cache_deallocate() {
        boost::stm::cache_deallocate(this);
    }

#endif

#if USE_STM_MEMORY_MANAGER
   void* operator new(size_t size) throw ()
   {
      return retrieve_mem(size);
   }

   void operator delete(void* mem)
   {
      static Derived elem;
      static size_t elemSize = sizeof(elem);
      return_mem(mem, elemSize);
   }
#endif

};

template <typename T> class native_trans :
public transaction_object< native_trans<T> >
{
public:

   native_trans() : value_(T()) {}
   native_trans(T const &rhs) : value_(rhs) {}
   native_trans(native_trans const &rhs) : value_(rhs.value_) {}

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


//-----------------------------------------------------------------------------
// transactional object wrapper
// A transactional_object<T> is a base_transaction_object wrapping an instance of type T
// Provides the definition of the virtual functions
//      forward constructors to the wrapped type
//      copy_state: relaying on the cache_restore<T> generic function
//      move_state and
//      cache_deallocate: relaying on the cache_restore<T> generic function
// Defines in addition the functions new and delete when USE_STM_MEMORY_MANAGER is defined
//
// If a class D inherits from B we have that transactional_object<D> dont inherits from transactional_object<B>, but
// we can static/dynamic cast them robustly.
// Evidently the std::static_cast/std::dynamic_cast do not works. We need to define the specific cast
//
// transactional_object<D>* d=...;
// transactional_object<B>* b=tx_static_cast<B>(d);
//
// transactional_object<B>* b=...;
// transactional_object<D>* d=tx_dynamic_cast<B>(b);
//
//-----------------------------------------------------------------------------

template <typename T>
class transactional_object : public base_transaction_object {
public:
    T value;

    transactional_object() {}
    transactional_object(const T*ptr)
        : base_transaction_object()
        , value(*ptr) {}

    transactional_object(transactional_object<T> const & r)
        : base_transaction_object(r)
        , value(r.value) {}

    template <typename T1>
    transactional_object(T1 const &p1)
        : base_transaction_object()
        , value(p1) {}

    template <typename T1, typename T2>
    transactional_object(T1 const &p1, T2 const &p2)
        : base_transaction_object()
        , value(p1,p2) {}

    transactional_object & operator=(transactional_object const & r)  // =default never throws
    {
        value = r.value;
        return *this;
    }

    virtual base_transaction_object* clone() const {
        return cache_new_copy(*this);
    }

    virtual void copy_state(base_transaction_object const * const rhs) {
        cache_restore(static_cast<transactional_object<T> const * const>(rhs),
                      //static_cast<transactional_object<T> *>(this));
                      this);
    }

#ifdef BOOST_STM_USE_MEMCOPY
    virtual void cache_deallocate() {
        boost::stm::cache_deallocate(this);
    }
#endif

#if USE_STM_MEMORY_MANAGER
   void* operator new(size_t size) throw ()
   {
      return retrieve_mem(size);
   }

   void operator delete(void* mem)
   {
      return_mem(mem, sizeof(transactional_object<T>));
   }
#endif

};

//-----------------------------------------------------------------------------
// gets the transactional_object<T> pointer wrapping the T pointer
//-----------------------------------------------------------------------------
template <typename T>
static transactional_object<T>* tx_up_cast(T* ptr) {
    return reinterpret_cast<transactional_object<T>*>(reinterpret_cast<char*>(ptr)-offsetof(transactional_object<T>, value));
}

//-----------------------------------------------------------------------------
// static_cast two transactional_object's
//-----------------------------------------------------------------------------
template <typename T, typename U>
static transactional_object<T>* tx_static_cast(transactional_object<U>* ptr) {
    return tx_up_cast(static_cast<T*>(&ptr->value));
}

//-----------------------------------------------------------------------------
// dynamic_cast two transactional_object's
//-----------------------------------------------------------------------------
template <typename T, typename U>
static transactional_object<T>* tx_dynamic_cast(transactional_object<U>* ptr) {
    T* p = dynamic_cast<T*>(&ptr->value);
    if (p==0) return 0;
    return tx_up_cast(p);
}

//-----------------------------------------------------------------------------
// This class defines the transactional cache of a non transactional variable.
// There is a map from the address of the variable of type T to an instance of this class
//
//-----------------------------------------------------------------------------

template <typename T>
class transactional_reference_cache : public base_transaction_object {
public:
    T* const value_;
    mutable T* ptr_;

    transactional_reference_cache(T& ref)
        : base_transaction_object()
        , value_(&ref), ptr_(0) {}

    transactional_reference_cache(T* ptr)
        : base_transaction_object()
        , value_(ptr), ptr_(0) {}

    ~transactional_reference_cache() {
        delete ptr_;
    }
    T* get() const {
        if(ptr_!=0) return ptr_;
        else return value_;
    }

    virtual base_transaction_object* clone() const {
        transactional_reference_cache tmp = cache_new_copy(*this);
        if (tmp.value!=0) {
            tmp.ptr_ = new T(*value_);
        }
        return tmp;
    }

    virtual void copy_state(base_transaction_object const * const rhs) {
        if (value_==0) return;
        *value_= *(static_cast<transactional_reference_cache<T> const * const>(rhs)->ptr_);
        delete ptr_;
        ptr_=0;
    }

#ifdef BOOST_STM_USE_MEMCOPY
    virtual void cache_deallocate() {
        delete ptr_;
        ptr_=0;
        boost::stm::cache_deallocate(this);
    }
#endif

#if USE_STM_MEMORY_MANAGER
   void* operator new(size_t size) throw ()
   {
      return retrieve_mem(size);
   }

   void operator delete(void* mem)
   {
      return_mem(mem, sizeof(transactional_object<T>));
   }
#endif

private:
    transactional_reference_cache(transactional_reference_cache<T> const & r)
        : base_transaction_object(r)
        , value_(r.value_), ptr_(r.ptr_) {}


    transactional_reference_cache & operator=(transactional_reference_cache const & r)
    {
        value_ = r.value_;
        ptr_ = r.ptr_;
        return *this;
    }

};

//-----------------------------------------------------------------------------
class base_contention_manager
{
public:
   virtual void abort_on_new(transaction const &t) = 0;
   virtual void abort_on_delete(transaction const &t,
      base_transaction_object const &in) = 0;

   virtual void abort_on_read(transaction const &t,
      base_transaction_object const &in) = 0;
   virtual void abort_on_write(transaction &t,
      base_transaction_object const &in) = 0;

   virtual bool abort_before_commit(transaction const &t) = 0;

   virtual bool permission_to_abort
      (transaction const &lhs, transaction const &rhs) = 0;

   virtual bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
      bool txIsIrrevocable, transaction const &rhs) = 0;

   virtual int lock_sleep_time() { return 10; }

   virtual void perform_isolated_tx_wait_priority_promotion(transaction &) = 0;
   virtual void perform_irrevocable_tx_wait_priority_promotion(transaction &) = 0;

   virtual ~base_contention_manager() {};
};

//-----------------------------------------------------------------------------
class DefaultContentionManager : public base_contention_manager
{
public:
   //--------------------------------------------------------------------------
   void abort_on_new(transaction const &t);
   void abort_on_delete(transaction const &t,
      base_transaction_object const &in);

   void abort_on_read(transaction const &t,
      base_transaction_object const &in);
   void abort_on_write(transaction &t,
      base_transaction_object const &in);

   virtual bool abort_before_commit(transaction const &t)
   {
      return false;
   }

   virtual bool permission_to_abort
      (transaction const &lhs, transaction const &rhs)
   { return true; }

   virtual bool allow_lock_to_abort_tx(int const & lockWaitTime, int const &lockAborted,
      bool txIsIrrevocable, transaction const &rhs);

   virtual void perform_isolated_tx_wait_priority_promotion(transaction &);
   virtual void perform_irrevocable_tx_wait_priority_promotion(transaction &);
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
    template<typename Lockable>
    class lock_guard2
    {
    private:
        Lockable& m;
        //bool owns_;

        explicit lock_guard2(lock_guard2&);
        lock_guard2& operator=(lock_guard2&);
    public:
        inline explicit lock_guard2(Lockable& m_):
            m(m_)
        {
            lock();
        }
        inline ~lock_guard2()
        {
            //unlock();
        }
        //inline bool owns_lock() { return owns_;}
        inline void lock() {
            //if (owns_)
                stm::lock(m);
            //owns_=true;
        }
        inline void unlock() {
            //if (owns_)
                stm::unlock(m);
            //owns_=false;
        }
        //inline void release() {
        //    owns_=false;
        //}
    };


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template <typename T>
class var_auto_lock
{
public:

   //--------------------------------------------------------------------------
   typedef T lock_type;
   typedef std::list<lock_type*> lock_list;

   //--------------------------------------------------------------------------
   var_auto_lock(lock_type *l, ...)
   {
      va_list ap;
      va_start(ap, l);

      while (l)
      {
         lockList_.push_back(l);
         l = va_arg(ap, lock_type*);
      }

      va_end(ap);

      std::list<PLOCK*>::iterator i = lockList_.begin();

      for (; i != lockList_.end(); ++i)
      {
         lock(*i);
      }
   }

   //--------------------------------------------------------------------------
   ~var_auto_lock()
   {
      for (std::list<PLOCK*>::iterator i = lockList_.begin(); i != lockList_.end(); ++i)
      {
         unlock(*i);
      }
   }

private:
   std::list<lock_type*> lockList_;

};


} // namespace core
}
#endif // BASE_TRANSACTION_H


