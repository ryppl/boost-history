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

#include <iostream>
#include "txLinearLock.h"
#include <boost/stm/transaction.hpp>
#include <boost/stm/synch.hpp>
#include "main.h"

static boost::stm::native_trans<int> gInt1;
static boost::stm::native_trans<int> gInt2;

#ifndef BOOST_STM_T_USE_BOOST_MUTEX
typedef boost::stm::exclusive_lock_adapter<pthread_mutex_t> mutex_type;
#else
typedef boost::stm::exclusive_lock_adapter<boost::mutex> mutex_type;
#endif

static mutex_type lock1;
static mutex_type lock2;

////////////////////////////////////////////////////////////////////////////
using namespace std; using namespace boost::stm;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
static void* Test1(void *threadId)
{
   transaction::initialize_thread();
   int start = *(int*)threadId;

   int startingValue = 0;
   int endingValue = (startingValue + kMaxInserts);

   for (int i = startingValue; i < endingValue/2; ++i)
   {
      for (transaction t; ; t.restart())
      {
         t.lock_conflict(&lock1);
         t.lock_conflict(&lock2);

         try
         {
            BOOST_STM_SYNCHRONIZE(lock2) {
            //stm::lock_guard<mutex_type> lk(lock2);
                --gInt2.value();
                cout << "\tgInt2: " << gInt2.value() << endl;
            }

            SLEEP(1000);

            BOOST_STM_SYNCHRONIZE(lock2) {
            //stm::lock_guard<mutex_type> lk(lock2);
            ++gInt1.value();
            cout << "\tgInt1: " << gInt1.value() << endl;
            }

            t.end();
            SLEEP(50);

            break;
         }
         catch (aborted_tx&) {}
      }
   }

   //--------------------------------------------------------------------------
   // last thread out sets the endTimer
   //--------------------------------------------------------------------------
   endTimer = time(0);
   finishThread(start);

   if (*(int*)threadId != kMainThreadId)
   {
      transaction::terminate_thread();
      pthread_exit(threadId);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
static void* Test3(void *threadId)
{
   transaction::initialize_thread();
   int start = *(int*)threadId;

   int startingValue = 0;
   int endingValue = (startingValue + kMaxInserts);

   for (int i = startingValue; i < endingValue/2; ++i)
   {
      SLEEP(1000);

      BOOST_STM_SYNCHRONIZE(lock1) BOOST_STM_SYNCHRONIZE(lock2) {
      //stm::lock_guard<mutex_type> lk(lock1);
      //stm::lock_guard<mutex_type> lk2(lock2);

        --gInt1.value();
        ++gInt2.value();
        cout << "\t\tgInt1: " << gInt1.value() << endl;
        cout << "\t\tgInt2: " << gInt2.value() << endl;
      }

   }

   //--------------------------------------------------------------------------
   // last thread out sets the endTimer
   //--------------------------------------------------------------------------
   endTimer = time(0);
   finishThread(start);

   if (*(int*)threadId != kMainThreadId)
   {
      transaction::terminate_thread();
      pthread_exit(threadId);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void TestTxLinearLock()
{
   transaction::initialize();

   pthread_t *threads = new pthread_t[kMaxThreads];
   int *threadId = new int[kMaxThreads];

   //--------------------------------------------------------------------------
   // Reset barrier variables before creating any threads. Otherwise, it is
   // possible for the first thread
   //--------------------------------------------------------------------------
   threadsFinished.value() = 0;
   threadsStarted.value() = 0;
   startTimer = 0;
   endTimer = 0;

   for (int j = 0; j < kMaxThreads - 1; ++j)
   {
      threadId[j] = j;
      pthread_create(&threads[j], 0, Test1, (void *)&threadId[j]);
   }

   int mainThreadId = kMaxThreads-1;

   Test3((void*)&mainThreadId);

   while (true)
   {
      if (threadsFinished.value() == kMaxThreads) break;
      SLEEP(10);
   }
}

