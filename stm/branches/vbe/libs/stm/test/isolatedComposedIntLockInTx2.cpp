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

#include <iostream>
#include "isolatedComposedIntLockInTx.h"
#include <boost/stm/transaction.hpp>
#include "main.h"

static boost::stm::native_trans<int> gInt;
static boost::stm::native_trans<int> gInt2;
#ifndef BOOST_STM_USE_BOOST_MUTEX
static boost::stm::latm::mutex_type lock1 = PTHREAD_MUTEX_INITIALIZER;
//static boost::stm::latm::mutex_type lock2 = PTHREAD_MUTEX_INITIALIZER;
#else
static boost::stm::latm::mutex_type lock1;
//static boost::stm::latm::mutex_type lock2;
#endif
////////////////////////////////////////////////////////////////////////////
using namespace std; using namespace boost::stm;

///////////////////////////////////////////////////////////////////////////////
static void* Test1(void *threadId)
{
   transaction::initialize_thread();
   int start = *(int*)threadId;

   int startingValue = 0;
   int endingValue = (startingValue + kMaxInserts);

   for (int i = startingValue; i < endingValue/2; ++i)
   {
      std::cout<< __LINE__ << " i= " << i << std::endl;
      int count=0;
      for (transaction t; ; t.restart())
      {
          count++;
        std::cout<< __LINE__ << " i="<<i << " end= " << endingValue/2 << " count= " << count << std::endl;
         t.lock_conflict(&lock1);
         try
         {
             std::cout<< __LINE__ << " lock" << std::endl;
            transaction::lock_(lock1);
            ++gInt.value();
            cout << __LINE__ << "\t" << gInt.value() << endl;
             std::cout<< __LINE__ << " unlock" << std::endl;
            transaction::unlock_(lock1);

            SLEEP(50);
            // do nothing on purpose, allowing other threads time to see
            // intermediate state IF they can get lock1 (they shouldn't)

             std::cout<< __LINE__ << " lock" << std::endl;
            transaction::lock_(lock1);
            --gInt.value();
            cout << __LINE__ << "\t" << gInt.value() << endl;
             std::cout<< __LINE__ << " unlock" << std::endl;
            transaction::unlock_(lock1);

            t.end();
            SLEEP(50);

            break;
         }
         catch (aborted_tx&) {
             std::cout<< __LINE__ << " aborted_tx " << i << std::endl;
        }
      }
   }
    std::cout<< __LINE__ << std::endl;

   //--------------------------------------------------------------------------
   // last thread out sets the endTimer
   //--------------------------------------------------------------------------
   endTimer = time(0);
   finishThread(start);

    std::cout<< __LINE__ << std::endl;
   if (*(int*)threadId != kMainThreadId)
   {
      transaction::terminate_thread();
    std::cout<< __LINE__ << std::endl;
      //pthread_exit(threadId);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
static void* Test3(void *threadId)
{
   if (*(int*)threadId != kMainThreadId) {
   transaction::initialize_thread();
   }
   int start = *(int*)threadId;

   int startingValue = 0;
   int endingValue = (startingValue + kMaxInserts);

   for (int i = startingValue; i < endingValue/2; ++i)
   {
      for (transaction t; ; t.restart())
      {
         try
         {
            ++gInt2.value();
            t.end();
            cout << "\t\t" << gInt2.value() << endl;
            SLEEP(rand() % 50);
            break;
         }
         catch (aborted_tx&) {}
      }
   }

   //--------------------------------------------------------------------------
   // last thread out sets the endTimer
   //--------------------------------------------------------------------------
   endTimer = time(0);

   if (*(int*)threadId != kMainThreadId)
   {
   finishThread(start);
      transaction::terminate_thread();
      //pthread_exit(threadId);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////////
void TestIsolatedComposedIntLockInTx2()
{
   transaction::initialize();
   transaction::initialize_thread();

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

   //Test3((void*)&mainThreadId);

   finishThread(mainThreadId);
    transaction::terminate_thread();
   while (true)
   {
      if (threadsFinished.value() == kMaxThreads) break;
      SLEEP(10);
   }

   cout << "gInt : " << gInt.value() << endl;
}

///////////////////////////////////////////////////////////////////////////////
#if 0
static void* Test2(void *threadId)
{
   transaction::initialize_thread();
   int start = *(int*)threadId;

   int startingValue = start * 100000;
   int endingValue = startingValue + kMaxInserts;

   for (int i = startingValue; i < 100*endingValue; ++i)
   {
      try
      {
         transaction t;
         t.lock_conflict(&lock1);
         int val = t.r(gInt).value();
         //transaction::lock(lock2);
         cout << val << endl;
         //transaction::unlock(lock2);
         t.end();
         SLEEP(10); // do nothing on purpose
      } catch (aborted_tx&) {}
   }

   //--------------------------------------------------------------------------
   // last thread out sets the endTimer
   //--------------------------------------------------------------------------
   endTimer = time(0);

   if (*(int*)threadId != kMainThreadId)
   {
   finishThread(start);
      transaction::terminate_thread();
      pthread_exit(threadId);
   }

   return 0;
}
#endif
