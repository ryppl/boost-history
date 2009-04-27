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



#ifndef BOOST_STM_TRANSACTION__HPP
#define BOOST_STM_TRANSACTION__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <boost/stm/detail/config.hpp>
#include <boost/stm/detail/datatypes.hpp>
#include <boost/stm/detail/transaction_bookkeeping.hpp>
#include <boost/stm/base_transaction.hpp>
#include <boost/stm/detail/bloom_filter.hpp>
#include <boost/stm/detail/vector_map.hpp>
#include <boost/stm/detail/vector_set.hpp>
#include <assert.h>
#include <string>
#include <iostream>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <pthread.h>

#if BUILD_MOVE_SEMANTICS
#include <type_traits>
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace boost { namespace stm {

   enum LatmType
   {
      kMinLatmType = 0,
      eFullLatmProtection = kMinLatmType,
      eTmConflictingLockLatmProtection,
      eTxConflictingLockLatmProtection,
      kMaxLatmType
   };

   enum TxType
   {
      kMinIrrevocableType = 0,
      eNormalTx = kMinIrrevocableType,
      eIrrevocableTx,
      eIrrevocableAndIsolatedTx,
      kMaxIrrevocableType
   };

   typedef std::pair<base_transaction_object*, base_transaction_object*> tx_pair;

///////////////////////////////////////////////////////////////////////////////
// transaction Class
///////////////////////////////////////////////////////////////////////////////
class transaction
{
public:
   //--------------------------------------------------------------------------
   // transaction typedefs
   //--------------------------------------------------------------------------

#if PERFORMING_VALIDATION
   typedef std::map<base_transaction_object*, size_t> ReadContainer;
#else
   typedef std::set<base_transaction_object*> ReadContainer;
#endif
   typedef std::map<size_t, ReadContainer*> ThreadReadContainer;

#ifdef MAP_WRITE_CONTAINER
   typedef std::map<base_transaction_object*, base_transaction_object*> WriteContainer;
#else
   typedef boost::stm::vector_map<base_transaction_object*, base_transaction_object*> WriteContainer;
#endif

#ifndef MAP_NEW_CONTAINER
   typedef boost::stm::vector_set<base_transaction_object*> MemoryContainerList;
#else
   typedef std::list<base_transaction_object*> MemoryContainerList;
#endif

   typedef std::map<size_t, WriteContainer*> ThreadWriteContainer;
   typedef std::map<size_t, TxType*> ThreadTxTypeContainer;

   typedef std::set<transaction*> TContainer;
   typedef std::set<transaction*> InflightTxes;

   typedef std::multimap<size_t, MemoryContainerList > DeletionBuffer;

#if 0
#ifndef BOOST_STM_USE_BOOST_MUTEX
   typedef pthread_mutex_t Mutex;
#else
   typedef boost::mutex Mutex;
#endif
#endif
    typedef std::set<Mutex*> MutexSet;

   typedef std::set<size_t> ThreadIdSet;

   typedef std::map<size_t, MemoryContainerList*> ThreadMemoryContainerList;

   typedef std::pair<size_t, Mutex*> thread_mutex_pair;
#ifndef MAP_THREAD_MUTEX_CONTAINER
   typedef boost::stm::vector_map<size_t, Mutex*> ThreadMutexContainer;
#else
   typedef std::map<size_t, Mutex*> ThreadMutexContainer;
#endif

   typedef std::map<size_t, MutexSet* > ThreadMutexSetContainer;
   typedef std::map<size_t, boost::stm::bloom_filter*> ThreadBloomFilterList;
   typedef std::map<size_t, boost::stm::bit_vector*> ThreadBitVectorList;

   typedef std::pair<size_t, int*> thread_bool_pair;
#ifndef MAP_THREAD_BOOL_CONTAINER
   typedef boost::stm::vector_map<size_t, int*> ThreadBoolContainer;
#else
   typedef std::map<size_t, int*> ThreadBoolContainer;
#endif

   typedef std::map<Mutex*, ThreadIdSet > MutexThreadSetMap;
   typedef std::map<Mutex*, size_t> MutexThreadMap;

   typedef std::set<transaction*> LockedTransactionContainer;

   typedef InflightTxes in_flight_transaction_container;
   typedef in_flight_transaction_container in_flight_trans_cont;

   struct tx_context
   {
      MemoryContainerList newMem;
      MemoryContainerList delMem;
      WriteContainer writeMem;
      bloom_filter wbloom;
      bloom_filter bloom;
      TxType txType;

      int abort;
   };

   typedef std::map<size_t, tx_context*> ThreadMemContainer;

   //--------------------------------------------------------------------------
   // transaction static methods
   //--------------------------------------------------------------------------
   static void initialize();
   static void initialize_thread();
   static void terminate_thread();
   static void contention_manager(base_contention_manager *rhs) { delete cm_; cm_ = rhs; }
   static base_contention_manager* get_contention_manager() { return cm_; }

   static void enableLoggingOfAbortAndCommitSetSize() { bookkeeping_.setIsLoggingAbortAndCommitSize(true); }
   static void disableLoggingOfAbortAndCommitSetSize() { bookkeeping_.setIsLoggingAbortAndCommitSize(false); }

   static const transaction_bookkeeping & bookkeeping() { return bookkeeping_; }

   static bool early_conflict_detection() { return !directLateWriteReadConflict_ && direct_updating(); }
   static bool late_conflict_detection() { return directLateWriteReadConflict_ || !direct_updating(); }

   static bool enable_dynamic_priority_assignment()
   {
      return dynamicPriorityAssignment_ = true;
   }

   static bool disable_dynamic_priority_assignment()
   {
      return dynamicPriorityAssignment_ = false;
   }

   static bool using_move_semantics() { return usingMoveSemantics_; }
   static bool using_copy_semantics() { return !using_move_semantics(); }

   static void enable_move_semantics()
   {
      if (!kDracoMoveSemanticsCompiled) 
         throw "move semantics off - rebuild with #define BUILD_MOVE_SEMANTICS 1";
      usingMoveSemantics_ = true;
   }

   static void disable_move_semantics()
   {
      usingMoveSemantics_ = false;
   }

   static bool doing_dynamic_priority_assignment()
   {
      return dynamicPriorityAssignment_;
   }

   static bool do_early_conflict_detection()
   {
      if (transactionsInFlight_.size() > 0) return false;
      if (deferred_updating()) return false;
      else return !(directLateWriteReadConflict_ = false);
   }

   static bool do_late_conflict_detection()
   {
      if (transactionsInFlight_.size() > 0) return false;
      else return directLateWriteReadConflict_ = true;
   }

   static std::string consistency_checking_string()
   {
      if (validating()) return "val";
      else return "inval";
   }

   static bool validating() 
   {
#ifdef PERFORMING_VALIDATION
      return true;
#endif
      return false;
   }

   static bool invalidating() { return !validating(); }

   static bool direct_updating() { return directUpdating_; }
   static bool deferred_updating() { return !directUpdating_; }

   //--------------------------------------------------------------------------
   // make all transactions direct as long as no transactions are in flight
   //--------------------------------------------------------------------------
   static bool do_direct_updating()
   {
      if (transactionsInFlight_.size() > 0) return false;
      else return directUpdating_ = true;
   }

   //--------------------------------------------------------------------------
   // make all transactions deferred as long as no transactions are in flight
   //--------------------------------------------------------------------------
   static bool do_deferred_updating()
   {
      if (transactionsInFlight_.size() > 0) return false;
      else directUpdating_ = false;
      return true;
   }

   static std::string update_policy_string()
   {
      return directUpdating_ ? "dir" : "def";
   }

   //--------------------------------------------------------------------------
   // Lock Aware Transactional Memory support methods
   //--------------------------------------------------------------------------
   static LatmType const latm_protection() { return eLatmType_; }
   static std::string const latm_protection_str();
   static void do_full_lock_protection();
   static void do_tm_lock_protection();
   static void do_tx_lock_protection();

   static bool doing_full_lock_protection();
   static bool doing_tm_lock_protection();
   static bool doing_tx_lock_protection();
   
 
#ifdef WIN32
   template <typename T>
   static int lock_(T *lock) { throw "unsupported lock type"; }

   template <typename T>
   static int trylock_(T *lock) { throw "unsupported lock type"; }

   template <typename T>
   static int unlock_(T *lock) { throw "unsupported lock type"; }

   template <>
   static int lock_(Mutex &lock) { return pthread_lock(&lock); }

   template <>
   static int lock_(Mutex *lock) { return pthread_lock(lock); }

   template <>
   static int trylock_(Mutex &lock) { return pthread_trylock(&lock); }

   template <>
   static int trylock_(Mutex *lock) { return pthread_trylock(lock); }

   template <>
   static int unlock_(Mutex &lock) { return pthread_unlock(&lock); }

   template <>
   static int unlock_(Mutex *lock) { return pthread_unlock(lock); }
#else
   static int lock_(PLOCK &lock) { return pthread_lock(&lock); }
   static int lock_(PLOCK *lock) { return pthread_lock(lock); }

   static int trylock_(PLOCK &lock) { return pthread_trylock(&lock); }
   static int trylock_(PLOCK *lock) { return pthread_trylock(lock); }

   static int unlock_(PLOCK &lock) { return pthread_unlock(&lock); }
   static int unlock_(PLOCK *lock) { return pthread_unlock(lock); }
#endif
   
   static int pthread_lock(Mutex *lock);
   static int pthread_trylock(Mutex *lock);
   static int pthread_unlock(Mutex *lock);
   


   //--------------------------------------------------------------------------
#if PERFORMING_LATM
   //--------------------------------------------------------------------------
   static void tm_lock_conflict(Mutex &lock)
   {
      tm_lock_conflict(&lock);
   }
   static void tm_lock_conflict(Mutex *lock);

   static void clear_tm_conflicting_locks();
   static MutexSet get_tm_conflicting_locks() { return tmConflictingLocks_; }

   void must_be_in_conflicting_lock_set(Mutex *inLock);
   static void must_be_in_tm_conflicting_lock_set(Mutex *inLock);

#if USING_TRANSACTION_SPECIFIC_LATM
   void see_if_tx_must_block_due_to_tx_latm();

   void lock_conflict(Mutex &lock)
   { add_tx_conflicting_lock(&lock); }

   void lock_conflict(Mutex *lock)
   { add_tx_conflicting_lock(lock); }

   void add_tx_conflicting_lock(Mutex &lock)
   {
      add_tx_conflicting_lock(&lock);
   }
   void add_tx_conflicting_lock(Mutex *lock);

   void clear_tx_conflicting_locks();
   MutexSet get_tx_conflicting_locks() { return conflictingMutexRef_; }
#endif

   void add_to_obtained_locks(Mutex* );
   static void unblock_conflicting_threads(Mutex *mutex);
   static bool mutex_is_on_obtained_tx_list(Mutex *mutex);
   static void unblock_threads_if_locks_are_empty();
   void clear_latm_obtained_locks();

   void add_to_currently_locked_locks(Mutex* m);
   void remove_from_currently_locked_locks(Mutex *m);
   bool is_currently_locked_lock(Mutex *m);
   bool is_on_obtained_locks_list(Mutex *m);
#endif

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   transaction();
   ~transaction();

   int const forced_to_abort() const { return *forcedToAbortRef_; }

   bool check_throw_before_restart() const
   {
      // if this transaction is in-flight or it committed, just ignore it
      if (in_flight() || committed()) return true;

      //-----------------------------------------------------------------------
      // if there is another in-flight transaction from this thread, and we
      // are using closed nesting with flattened transactions, we should throw
      // an exception here because restarting the transactions will cause it to
      // infinitely fail
      //-----------------------------------------------------------------------
      lock_inflight_access();
      if (other_in_flight_same_thread_transactions())
      {
         unlock_inflight_access();
         throw aborted_transaction_exception("closed nesting throw");
      }
      unlock_inflight_access();

      return true;
   }

   bool committed() const { return state() == e_committed || state() == e_hand_off; }
   bool aborted() const { return state() == e_aborted; }
   bool in_flight() const { return state() == e_in_flight; }

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   template <typename T>
   T& find_original(T& in)
   {
      //-----------------------------------------------------------------------
      // if transactionThread_ is not invalid it means this is the original, so
      // we can return it. Otherwise, we need to search for the original in
      // our write set
      //-----------------------------------------------------------------------
      if (1 == in.new_memory()) return in;
      if (in.transaction_thread() == boost::stm::kInvalidThread) return in;

      base_transaction_object *inPtr = (base_transaction_object*)&in;

      for (WriteContainer::iterator i = writeList().begin(); i != writeList().end(); ++i)
      {
         if (i->second == (inPtr))
         {
            return *static_cast<T*>(i->first);
         }
      }

      //-----------------------------------------------------------------------
      // if it's not in our original / new list, then we need to except
      //-----------------------------------------------------------------------
      throw aborted_transaction_exception("original not found");
   }

   //--------------------------------------------------------------------------
   // The below methods change their behavior based on whether the transaction
   // is direct or deferred:
   //
   //    read           - deferred_read or direct_read
   //    write          - deferred_write or direct_write
   //    delete_memory  - deferred_delete_memory or direct_delete_memory
   //    commit         - deferred_commit or direct_commit
   //    abort          - deferred_abort or direct_abort
   //
   // In addition, a number of commit or abort methods must be made deferred or
   // direct as their version / memory management is entirely different if the
   // transaction is direct or deferred.
   //
   //--------------------------------------------------------------------------
#ifndef DISABLE_READ_SETS
   size_t const read_set_size() const { return readListRef_.size(); }
#endif

   size_t const writes() const { return writeListRef_->size(); }
   size_t const reads() const { return reads_; }

   template <typename T> T const * read_ptr(T const * in) { return &read(*in); }
   template <typename T> T const & r(T const & in) { return read(in); }

   //--------------------------------------------------------------------------
   // attempts to find the written value of T based on 
   //--------------------------------------------------------------------------
   template <typename T>
   T* get_written(T const & in)
   {
      WriteContainer::iterator i = writeList().find
         ((base_transaction_object*)(&in));
      if (i == writeList().end()) return NULL;
      else return static_cast<T*>(i->second);
   }

   //--------------------------------------------------------------------------
   template <typename T>
   bool has_been_read(T const & in)
   {
#ifndef DISABLE_READ_SETS
      ReadContainer::iterator i = readList().find
         (static_cast<base_transaction_object*>(&in));
      //----------------------------------------------------------------
      // if this object is already in our read list, bail
      //----------------------------------------------------------------
      return i != readList().end();
#else
      return bloom().exists((size_t)&in);
#endif
   }

   //--------------------------------------------------------------------------
   template <typename T>
   T const & read(T const & in)
   {
      if (directUpdating_)
      {
#if PERFORMING_VALIDATION
         throw "direct updating not implemented for validation yet";
#else
         return direct_read(in);
#endif
      }
      else
      {
         return deferred_read(in);
      }
   }

   //--------------------------------------------------------------------------
   template <typename T> T* write_ptr(T* in) { return &write(*in); }
   template <typename T> T& w(T& in) { return write(in); }

   //--------------------------------------------------------------------------
   template <typename T>
   T& write(T& in)
   {
      if (directUpdating_)
      {
#if PERFORMING_VALIDATION
         throw "direct updating not implemented for validation yet";
#else
         return direct_write(in);
#endif
      }
      else
      {
         return deferred_write(in);
      }
   }

   //--------------------------------------------------------------------------
   template <typename T>
   void delete_memory(T &in)
   {
      if (directUpdating_)
      {
#if PERFORMING_VALIDATION
         throw "direct updating not implemented for validation yet";
#else
         direct_delete_memory(in);
#endif
      }
      else
      {
         deferred_delete_memory(in);
      }
   }

   //--------------------------------------------------------------------------
   // allocation of new memory behaves the same for both deferred and direct
   // transaction implementations
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   template <typename T>
   T* new_memory(T const &rhs)
   {
      if (forced_to_abort()) 
      {
         if (!directUpdating_) 
         {
            deferred_abort(true);
            throw aborted_tx("");
         }

#ifndef DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
         cm_->abort_on_new(*this);
#endif
      }
      T *newNode = new T();
      newNode->transaction_thread(threadId_);
      newNode->new_memory(1);
      newMemoryList().push_front(newNode);

      return newNode;
   }

   //--------------------------------------------------------------------------
   template <typename T>
   T* new_memory_copy(T const &rhs)
   {
      if (forced_to_abort()) 
      {
         if (!directUpdating_) 
         {
            deferred_abort(true);
            throw aborted_tx("");
         }
#ifndef DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
         cm_->abort_on_new(*this);
#endif
      }
      T *newNode = new T(rhs);
      newNode->transaction_thread(threadId_);
      newNode->new_memory(1);
      newMemoryList().push_back(newNode);

      return newNode;
   }

   void begin();
   bool restart();

   bool restart_if_not_inflight()
   {
      if (in_flight()) return true;
      else return restart();
   }

   void end();
   void no_throw_end();

   void force_to_abort() 
   { 
      *forcedToAbortRef_ = true; 

#ifdef PERFORMING_COMPOSITION
#ifndef USING_SHARED_FORCED_TO_ABORT
      // now set all txes of this threadid that are in-flight to force to abort
      for (InflightTxes::iterator j = transactionsInFlight_.begin(); 
      j != transactionsInFlight_.end(); ++j)
      {
         transaction *t = (transaction*)*j;

         // if this is a parent or child tx, it must abort too
         if (t->threadId_ == this->threadId_) t->forcedToAbortRef_ = true;
      }
#endif
#endif
   }

   void unforce_to_abort() { *forcedToAbortRef_ = false; }

   //--------------------------------------------------------------------------
   void lock_and_abort();

   size_t writeListSize() const { return writeListRef_->size(); }

   size_t const &priority() const { return priority_; }
   void set_priority(uint32 const &rhs) const { priority_ = rhs; }
   void raise_priority() 
   { 
      if (priority_ < size_t(-1)) 
      {
         ++priority_; 
      }
   }

   static InflightTxes const & in_flight_transactions() { return transactionsInFlight_; }

   void make_irrevocable();
   void make_isolated();
   bool irrevocable() const;
   bool isolated() const;

   size_t const & thread_id() const { return threadId_; }

private:

#ifdef LOGGING_BLOCKS
   static std::string outputBlockedThreadsAndLockedLocks();
#endif
   //--------------------------------------------------------------------------
   int const& hasLock() const { return hasMutex_; }
   void lock_tx();
   void unlock_tx();

   static void lock_latm_access();
   static void unlock_latm_access();

   static void lock_inflight_access();
   static void unlock_inflight_access();

   static void lock_general_access();
   static void unlock_general_access();

   static PLOCK* latm_lock() { return &latmMutex_; }
   static PLOCK* general_lock() { return &transactionMutex_; }
   static PLOCK* inflight_lock() { return &transactionsInFlightMutex_; }

#if PERFORMING_LATM
   void block() { blockedRef_ = true; }
   void unblock() { blockedRef_ = false; }
#endif

   bool irrevocableTxInFlight();
   bool isolatedTxInFlight();
   void commit_deferred_update_tx();

   bool canAbortAllInFlightTxs();
   bool abortAllInFlightTxs();
   void put_tx_inflight();
   bool can_go_inflight();
   static transaction* get_inflight_tx_of_same_thread(bool);

#if !PERFORMING_VALIDATION
   //--------------------------------------------------------------------------
   //                      DIRECT UPDATING SECTION
   //--------------------------------------------------------------------------
   template <typename T>
   T const & direct_read(T const & in)
   {
      if (in.transaction_thread() == threadId_) return in;

      if (forced_to_abort())
      {
#ifndef DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
         // let the contention manager decide to throw or not
         cm_->abort_on_write(*this, (base_transaction_object&)(in));
#endif
      }

      //--------------------------------------------------------------------
      // otherwise, see if its in our read list
      //--------------------------------------------------------------------
#ifndef DISABLE_READ_SETS
      ReadContainer::iterator i = readList().find((base_transaction_object*)&in);
      if (i != readList().end()) return in;
#endif
#if USE_BLOOM_FILTER
      if (bloom().exists((size_t)&in)) return in;
#endif

      //--------------------------------------------------------------------
      // if we want direct write-read conflict to be done late (commit time)
      // perform the following section of code
      //--------------------------------------------------------------------
      if (directLateWriteReadConflict_)
      {
         //--------------------------------------------------------------------
         // if transaction_thread() is not invalid (and not us), we get original
         // object from the thread that is changing it
         //--------------------------------------------------------------------
         lock(&transactionMutex_);
         lock_tx();

         if (in.transaction_thread() != boost::stm::kInvalidThread)
         {
            lockThreadMutex(in.transaction_thread());

            ThreadWriteContainer::iterator writeIter = 
               threadWriteLists_.find(in.transaction_thread());
         
            WriteContainer::iterator readMem = writeIter->second->find((base_transaction_object*)&in);

            if (readMem == writeIter->second->end()) 
            {
               std::cout << "owner did not contain item in write list" << std::endl;
            }

#ifndef DISABLE_READ_SETS
            readList().insert((base_transaction_object*)readMem->second);
#endif
#if USE_BLOOM_FILTER
            bloom().insert((size_t)readMem->second);
#endif
            unlock(&transactionMutex_);
            unlock_tx();
            unlockThreadMutex(in.transaction_thread());

            ++reads_;
            return *static_cast<T*>(readMem->second);
         }

         // already have locked us above - in both if / else
#ifndef DISABLE_READ_SETS
         readList().insert((base_transaction_object*)&in);
#endif
#if USE_BLOOM_FILTER
         bloom().insert((size_t)&in);
#endif
         unlock(&transactionMutex_);
         unlock_tx();
         ++reads_;
         return in;
      }
      else 
      {
         //--------------------------------------------------------------------
         // if we want direct write-read conflict to be done early, bail
         // if someone owns this
         //--------------------------------------------------------------------
         if (in.transaction_thread() != boost::stm::kInvalidThread)
         {
            // let the contention manager decide to throw or not
            cm_->abort_on_write(*this, (base_transaction_object&)(in));
         }

         lock_tx();
         // already have locked us above - in both if / else
#ifndef DISABLE_READ_SETS
         readList().insert((base_transaction_object*)&in);
#endif
#if USE_BLOOM_FILTER
         bloom().insert((size_t)&in);
#endif
         unlock_tx();
         ++reads_;
      }
      return in;
   }

   //--------------------------------------------------------------------------
   template <typename T>
   T& direct_write(T& in)
   {
      // if this is our memory (new or mod global) just return
      if (in.transaction_thread() == threadId_) return in;

      if (forced_to_abort())
      {
#ifndef DELAY_INVALIDATION_DOOMED_TXS_UNTIL_COMMIT
         cm_->abort_on_write(*this, static_cast<base_transaction_object&>(in));
#endif
      }
      //-----------------------------------------------------------------------
      // first we globally lock all threads before we poke at this global 
      // memory - since we need to ensure other threads don't try to 
      // manipulate this at the same time we are going to
      //-----------------------------------------------------------------------
      lock(&transactionMutex_);

      // we currently don't allow write stealing in direct update. if another 
      // tx beat us to the memory, we abort
      if (in.transaction_thread() != boost::stm::kInvalidThread)
      {
         unlock(&transactionMutex_);
         throw aborted_tx("direct writer already exists.");
      }

      in.transaction_thread(threadId_);
      writeList().insert(tx_pair((base_transaction_object*)&in, new T(in)));
#if USE_BLOOM_FILTER
      bloom().insert((size_t)&in);
#endif
      unlock(&transactionMutex_);
      return in;
   }

   //--------------------------------------------------------------------------
   template <typename T>
   void direct_delete_memory(T &in)
   {
      if (in.transaction_thread() == threadId_)
      {
         //deletedMemoryList().push_back(&(T)in);
         deletedMemoryList().push_back((base_transaction_object*)&in);
         return;
      }

      //-----------------------------------------------------------------------
      // if we're here this item isn't in our writeList - get the global lock
      // and see if anyone else is writing to it. if not, we add the item to
      // our write list and our deletedList
      //-----------------------------------------------------------------------
      lock(&transactionMutex_);

      if (in.transaction_thread() != boost::stm::kInvalidThread)
      {
         unlock(&transactionMutex_);
         cm_->abort_on_write(*this, (base_transaction_object&)(in));
      }
      else 
      {
         in.transaction_thread(threadId_);
         unlock(&transactionMutex_);
         // is this really necessary? in the deferred case it is, but in direct it
         // doesn't actually save any time for anything
         //writeList()[(base_transaction_object*)&in] = NULL;

         deletedMemoryList().push_back((base_transaction_object*)&in);
      }
   }
#endif


   //--------------------------------------------------------------------------
   //                      DEFERRED UPDATING SECTION
   //--------------------------------------------------------------------------
   template <typename T>
   T const & deferred_read(T const & in)
   {
      if (forced_to_abort()) 
      {
         deferred_abort(true);
         throw aborted_tx("");
      }

      //----------------------------------------------------------------
      // always check if the writeList size is 0 first, since if it is 
      // it can save construction of an iterator and search setup.
      //----------------------------------------------------------------
#if PERFORMING_WRITE_BLOOM
      if (0 == writeList().size() || 
         (writeList().size() > 16 && 
         !wbloom().exists((size_t)&in))) return insert_and_return_read_memory(in);
#else
      if (0 == writeList().size()) return insert_and_return_read_memory(in);
#endif

      WriteContainer::iterator i = writeList().find
         ((base_transaction_object*)(&in));
      //----------------------------------------------------------------
      // always check to see if read memory is in write list since it is 
      // possible to have already written to memory being read now
      //----------------------------------------------------------------
      if (i == writeList().end()) return insert_and_return_read_memory(in);
      else return *static_cast<T*>(i->second);
   }

   //-------------------------------------------------------------------
   template <typename T>
   T& insert_and_return_read_memory(T& in)
   {
#ifndef DISABLE_READ_SETS
      ReadContainer::iterator i = readList().find
         (static_cast<base_transaction_object*>(&in));
      //----------------------------------------------------------------
      // if this object is already in our read list, bail
      //----------------------------------------------------------------
      if (i != readList().end()) return in;
#else
      if (bloom().exists((size_t)&in)) return in;
#endif
      lock_tx();
#ifndef DISABLE_READ_SETS
#if PERFORMING_VALIDATION
      readList()[(base_transaction_object*)&in] = in.version_;
#else
      readList().insert((base_transaction_object*)&in);
#endif
#endif
#if USE_BLOOM_FILTER
      bloom().insert((size_t)&in);
#endif
      unlock_tx();
      ++reads_;
      return in;
   }

   template <typename T>
   T& deferred_write(T& in)
   {
      if (forced_to_abort()) 
      {
         deferred_abort(true);
         throw aborted_tx("");
      }
      //----------------------------------------------------------------
      // if transactionThread_ is not invalid, then already writing to 
      // non-global memory - so succeed.
      //----------------------------------------------------------------
      if (in.transaction_thread() != boost::stm::kInvalidThread) return in;

      WriteContainer::iterator i = writeList().find
         (static_cast<base_transaction_object*>(&in));
      //----------------------------------------------------------------
      // if !in write set, add. lock first, for version consistency
      //----------------------------------------------------------------
      if (i == writeList().end())
      {
         // get the lock before we make a copy of this object
         lock_tx();
#if USE_BLOOM_FILTER
         bloom().insert((size_t)&in);
         unlock_tx();
#endif
#if PERFORMING_WRITE_BLOOM
         wbloom().set_bv1(bloom().h1());
         wbloom().set_bv2(bloom().h2());
         //sm_wbv().set_bit((size_t)&in % sm_wbv().size());
#endif
         base_transaction_object* returnValue = new T(in);
         returnValue->transaction_thread(threadId_);
         writeList().insert(tx_pair((base_transaction_object*)&in, returnValue));
#ifndef USE_BLOOM_FILTER
         unlock_tx();
#endif
         return *static_cast<T*>(returnValue);
      }
      else return *static_cast<T*>(i->second);
   }

   //--------------------------------------------------------------------------
   template <typename T>
   void deferred_delete_memory(T &in)
   {
      if (forced_to_abort()) 
      {
         deferred_abort(true);
         throw aborted_tx("");
      }
      //-----------------------------------------------------------------------
      // if this memory is true memory, not transactional, we add it to our 
      // deleted list and we're done
      //-----------------------------------------------------------------------
      if (in.transaction_thread() != boost::stm::kInvalidThread)
      {
         lock_tx();
         bloom().insert((size_t)&in);
         unlock_tx();
         writeList().insert(tx_pair((base_transaction_object*)&in, NULL));
      }
      //-----------------------------------------------------------------------
      // this isn't real memory, it's transactional memory. But the good news is,
      // the real version has to be in our write list somewhere, find it, add 
      // both items to the deletion list and exit
      //-----------------------------------------------------------------------
      else
      {
         lock_tx();
         bloom().insert((size_t)&in);
         unlock_tx();
         // check the ENTIRE write container for this piece of memory in the
         // second location. If it's there, it means we made a copy of a piece
         for (WriteContainer::iterator j = writeList().begin(); writeList().end() != j; ++j)
         {
            if (j->second == &(T)in)
            {
               writeList().insert(tx_pair(j->first, NULL));
               deletedMemoryList().push_back(j->first);
            }
         }
      }

      deletedMemoryList().push_back((base_transaction_object *)&in);
   }

   //--------------------------------------------------------------------------
   void verifyReadMemoryIsValidWithGlobalMemory();
   void verifyWrittenMemoryIsValidWithGlobalMemory();

   //--------------------------------------------------------------------------
   void abort() throw() { directUpdating_ ? direct_abort() : deferred_abort(); }
   void deferred_abort(bool const &alreadyRemovedFromInflightList = false) throw();
   void direct_abort(bool const &alreadyRemovedFromInflightList = false) throw();

   //--------------------------------------------------------------------------
   void validating_deferred_commit();
   void invalidating_deferred_commit();
   void validating_direct_commit();
   void invalidating_direct_commit();

   //--------------------------------------------------------------------------
   void lockThreadMutex(size_t threadId);
   void unlockThreadMutex(size_t threadId);
   static void lock_all_mutexes_but_this(size_t threadId);
   static void unlock_all_mutexes_but_this(size_t threadId);

   //--------------------------------------------------------------------------
   // side-effect: this unlocks all mutexes including its own. this is a slight
   // optimization over unlock_all_mutexes_but_this() as it doesn't have an 
   // additional "if" to slow down performance. however, as it will be 
   // releasing its own mutex, it must reset hasMutex_
   //--------------------------------------------------------------------------
   void lock_all_mutexes();
   void unlock_all_mutexes();

   transaction_state const & state() const { return state_; }

#if PERFORMING_LATM
   int const blocked() const { return blockedRef_; }
#endif
   WriteContainer& writeList() { return *writeListRef_; }
#ifndef DISABLE_READ_SETS
   ReadContainer& readList() { return readListRef_; }
#endif

   bloom_filter& bloom() { return *bloomRef_; }
#if PERFORMING_WRITE_BLOOM
   bloom_filter& wbloom() { return *wbloomRef_; }
   //bit_vector& sm_wbv() { return sm_wbv_; }
#endif

#ifndef DISABLE_READ_SETS
   bool isReading() const { return 0 != readListRef_.size(); }
#endif
   bool isWriting() const { return 0 != writeListRef_->size(); }
   bool is_only_reading() const { return !isWriting(); }

   MemoryContainerList& newMemoryList() { return *newMemoryListRef_; }
   MemoryContainerList& deletedMemoryList() { return *deletedMemoryListRef_; }

   TxType const tx_type() const { return *txTypeRef_; }
   void tx_type(TxType const &rhs) { *txTypeRef_ = rhs; }

   Mutex * mutex() { return mutexRef_; }

   // undefined and hidden - never allow these - bad things would happen
   transaction& operator=(transaction const &);
   transaction(transaction const &);

   bool otherInFlightTransactionsWritingThisMemory(base_transaction_object *obj);
   bool other_in_flight_same_thread_transactions() const throw();
   bool otherInFlightTransactionsOfSameThreadNotIncludingThis(transaction const * const);

   void removeAllSharedValuesOfThread(base_transaction_object *key);
   void copyOtherInFlightComposableTransactionalMemoryOfSameThread();
   bool setAllInFlightComposableTransactionsOfThisThreadToAbort();

   //--------------------------------------------------------------------------
   void forceOtherInFlightTransactionsWritingThisWriteMemoryToAbort();
   void forceOtherInFlightTransactionsReadingThisWriteMemoryToAbort();
   bool forceOtherInFlightTransactionsAccessingThisWriteMemoryToAbort(bool, transaction* &);

   void unlockAllLockedThreads(LockedTransactionContainer &);

   //--------------------------------------------------------------------------
   // direct and deferred transaction method for version / memory management
   //--------------------------------------------------------------------------
   void directCommitTransactionDeletedMemory() throw();
   size_t earliest_start_time_of_inflight_txes();
   void doIntervalDeletions();

   void deferredCommitTransactionDeletedMemory() throw();
   void directCommitTransactionNewMemory() { deferredCommitTransactionNewMemory(); }
   void deferredCommitTransactionNewMemory();

   void directAbortTransactionDeletedMemory() throw();
   void deferredAbortTransactionDeletedMemory() throw() { deletedMemoryList().clear(); }
   void directAbortTransactionNewMemory() throw() { deferredAbortTransactionNewMemory(); }
   void deferredAbortTransactionNewMemory() throw();

   void directCommitWriteState();
   void deferredCommitWriteState();

#ifndef DISABLE_READ_SETS
   void directCommitReadState() { readList().clear(); }
   void deferredCommitReadState() { readList().clear(); }
#endif

   void directAbortWriteList();
   void deferredAbortWriteList() throw();

#ifndef DISABLE_READ_SETS
   void directAbortReadList() { readList().clear(); }
   void deferredAbortReadList() throw() { readList().clear(); }
#endif

   void validating_direct_end_transaction();
   void invalidating_direct_end_transaction();

   void validating_deferred_end_transaction();
   void invalidating_deferred_end_transaction();

   //--------------------------------------------------------------------------
   //
   // Lock Aware Transactional Memory (LATM) support methods
   //
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   // deferred updating methods
   //--------------------------------------------------------------------------
   static bool def_do_core_tm_conflicting_lock_pthread_lock_mutex
      (Mutex *mutex, int lockWaitTime, int lockAborted);
   static bool def_do_core_tx_conflicting_lock_pthread_lock_mutex
      (Mutex *mutex, int lockWaitTime, int lockAborted, bool txIsIrrevocable);
   static bool def_do_core_full_pthread_lock_mutex
      (Mutex *mutex, int lockWaitTime, int lockAborted);

   //--------------------------------------------------------------------------
   // direct updating methods
   //--------------------------------------------------------------------------
   static bool dir_do_core_tm_conflicting_lock_pthread_lock_mutex
      (Mutex *mutex, int lockWaitTime, int lockAborted);
   static bool dir_do_core_tx_conflicting_lock_pthread_lock_mutex
      (Mutex *mutex, int lockWaitTime, int lockAborted, bool txIsIrrevocable);
   static bool dir_do_core_full_pthread_lock_mutex
      (Mutex *mutex, int lockWaitTime, int lockAborted);

   static int thread_id_occurance_in_locked_locks_map(size_t threadId);

   static void wait_until_all_locks_are_released(bool);

   //--------------------------------------------------------------------------
   // deferred updating locking methods
   //--------------------------------------------------------------------------
   static int def_full_pthread_lock_mutex(Mutex *mutex);
   static int def_full_pthread_trylock_mutex(Mutex *mutex);
   static int def_full_pthread_unlock_mutex(Mutex *mutex);

   static int def_tm_conflicting_lock_pthread_lock_mutex(Mutex *mutex);
   static int def_tm_conflicting_lock_pthread_trylock_mutex(Mutex *mutex);
   static int def_tm_conflicting_lock_pthread_unlock_mutex(Mutex *mutex);

   static int def_tx_conflicting_lock_pthread_lock_mutex(Mutex *mutex);
   static int def_tx_conflicting_lock_pthread_trylock_mutex(Mutex *mutex);
   static int def_tx_conflicting_lock_pthread_unlock_mutex(Mutex *mutex);

   //--------------------------------------------------------------------------
   // direct updating locking methods
   //--------------------------------------------------------------------------
   static int dir_full_pthread_lock_mutex(Mutex *mutex);
   static int dir_full_pthread_trylock_mutex(Mutex *mutex);
   static int dir_full_pthread_unlock_mutex(Mutex *mutex);

   static int dir_tm_conflicting_lock_pthread_lock_mutex(Mutex *mutex);
   static int dir_tm_conflicting_lock_pthread_trylock_mutex(Mutex *mutex);
   static int dir_tm_conflicting_lock_pthread_unlock_mutex(Mutex *mutex);

   static int dir_tx_conflicting_lock_pthread_lock_mutex(Mutex *mutex);
   static int dir_tx_conflicting_lock_pthread_trylock_mutex(Mutex *mutex);
   static int dir_tx_conflicting_lock_pthread_unlock_mutex(Mutex *mutex);

   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   static DeletionBuffer deletionBuffer_;
   static std::ofstream logFile_;

   static MutexSet tmConflictingLocks_;
   static MutexSet latmLockedLocks_;
   static MutexThreadSetMap latmLockedLocksAndThreadIdsMap_;
   static MutexThreadMap latmLockedLocksOfThreadMap_;
   static LatmType eLatmType_;
   static InflightTxes transactionsInFlight_;

   static Mutex deletionBufferMutex_;
   static Mutex transactionMutex_;
   static Mutex transactionsInFlightMutex_;
   static Mutex latmMutex_;
   static pthread_mutexattr_t transactionMutexAttribute_;

   static bool initialized_;
   static bool directLateWriteReadConflict_;
   static bool dynamicPriorityAssignment_;
   static bool usingMoveSemantics_;
   static transaction_bookkeeping bookkeeping_;
   static base_contention_manager *cm_;

#if PERFORMING_LATM
   static ThreadMutexSetContainer threadConflictingMutexes_;
   static ThreadMutexSetContainer threadObtainedLocks_;
   static ThreadMutexSetContainer threadCurrentlyLockedLocks_;
#endif

   static ThreadMutexContainer threadMutexes_;
   static ThreadBoolContainer threadBlockedLists_;

#ifndef USE_SINGLE_THREAD_CONTEXT_MAP
   static ThreadWriteContainer threadWriteLists_;
   static ThreadReadContainer threadReadLists_;
   static ThreadBloomFilterList threadBloomFilterLists_;
   static ThreadBloomFilterList threadWBloomFilterLists_;
   static ThreadMemoryContainerList threadNewMemoryLists_;
   static ThreadMemoryContainerList threadDeletedMemoryLists_;
   static ThreadTxTypeContainer threadTxTypeLists_;
   static ThreadBoolContainer threadForcedToAbortLists_;
#else
   static ThreadMemContainer threadMemContainer_;
#endif

   //--------------------------------------------------------------------------
   static bool directUpdating_;
   static size_t globalClock_;
   static size_t stalls_;

   //--------------------------------------------------------------------------
   // must be mutable because in cases where reads collide with other txs
   // modifying the same memory location in-flight, we add that memory
   // location ourselves to the modifiedList for us since performing a read
   // when another in-flight transaction is modifying the same location can
   // cause us to end up reading stale data if the other tx commits with
   // its newly modified data that we were reading.
   //
   // thus, we promote our read to a write and therefore need this map to
   // be mutable - see the read() interface for more documentation
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   // ******** WARNING ******** MOVING threadId_ WILL BREAK TRANSACTION.
   // threadId_ MUST ALWAYS THE FIRST MEMBER OF THIS CLASS. THE MEMBER 
   // INITIALIZATION IS ORDER DEPENDENT UPON threadId_!!
   // ******** WARNING ******** MOVING threadId_ WILL BREAK TRANSACTION
   //--------------------------------------------------------------------------
   size_t threadId_;


#ifdef USE_SINGLE_THREAD_CONTEXT_MAP
   tx_context &context_;
#endif
   //--------------------------------------------------------------------------
   // ******** WARNING ******** MOVING threadId_ WILL BREAK TRANSACTION.
   // threadId_ MUST ALWAYS THE FIRST MEMBER OF THIS CLASS. THE MEMBER 
   // INITIALIZATION IS ORDER DEPENDENT UPON threadId_!!
   // ******** WARNING ******** MOVING threadId_ WILL BREAK TRANSACTION
   //--------------------------------------------------------------------------
   mutable WriteContainer *writeListRef_;
#ifndef DISABLE_READ_SETS
   mutable ReadContainer &readListRef_;
#endif
   mutable bloom_filter *bloomRef_;
#if PERFORMING_WRITE_BLOOM
   mutable bloom_filter *wbloomRef_;
   //mutable bit_vector &sm_wbv_;
#endif

   MemoryContainerList *newMemoryListRef_;
   MemoryContainerList *deletedMemoryListRef_;
   TxType *txTypeRef_;

#if USING_SHARED_FORCED_TO_ABORT
   int *forcedToAbortRef_;
#else
   int forcedToAbortRef_;
#endif

   Mutex *mutexRef_;

#if PERFORMING_LATM
   int &blockedRef_;
#endif

#if PERFORMING_LATM
#if USING_TRANSACTION_SPECIFIC_LATM
   MutexSet &conflictingMutexRef_;
#endif
   MutexSet &obtainedLocksRef_;
   MutexSet &currentlyLockedLocksRef_;
#endif
   int hasMutex_;
   mutable size_t priority_;
   transaction_state state_;
   size_t reads_;

   mutable size_t startTime_;
};

#if 0

template <>
inline int transaction::lock<Mutex> (Mutex &lock) { return transaction::pthread_lock(&lock); }

template <>
inline int transaction::lock<Mutex*> (Mutex *lock) { return transaction::pthread_lock(lock); }

template <>
inline int transaction::trylock<Mutex> (Mutex &lock) { return transaction::pthread_trylock(&lock); }

template <>
inline int transaction::trylock<Mutex*> (Mutex *lock) { return transaction::pthread_trylock(lock); }

template <>
inline int transaction::unlock<Mutex> (Mutex &lock) { return transaction::pthread_unlock(&lock); }

template <>
inline int transaction::unlock<Mutex*> (Mutex *lock) { return transaction::pthread_unlock(lock); }

#endif

//---------------------------------------------------------------------------
// do not remove if (). It is necessary a necessary fix for compilers
// that do not destroy index variables of for loops. In addition, the
// rand()+1 check is necessarily complex so smart compilers can't
// optimize the if away
//---------------------------------------------------------------------------
#define use_atomic(T) if (0 != rand()+1) for (transaction T; !T.committed() && T.restart(); T.end())
#define try_atomic(T) if (0 != rand()+1) for (transaction T; !T.committed() && T.restart(); T.no_throw_end()) try 
#define atomic(T)     if (0 != rand()+1) for (transaction T; !T.committed() && T.check_throw_before_restart() && T.restart_if_not_inflight(); T.no_throw_end()) try



#define catch_before_retry(E) catch (aborted_tx &E)
#define before_retry catch (aborted_tx &)
#define end_atom catch (aborted_tx &) {}

} // core namespace
}
#include <boost/stm/detail/transaction_impl.hpp>
#include <boost/stm/detail/latm_general_impl.hpp>
#include <boost/stm/detail/auto_lock.hpp>

///////////////////////////////////////////////////////////////////////////////
#endif // TRANSACTION_H


