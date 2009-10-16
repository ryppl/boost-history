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

#include <iostream>
#include "lotExample.h"
#include <boost/stm/transaction.hpp>
#include "main.h"

using namespace boost::stm;
using namespace std;

native_trans<int> *intP = 0;

static void pointer_test_alloc()
{
   atomic(t)
   {
      intP = t.new_shared_memory(intP);
      SLEEP(1000);
      *intP = 1;
      t.force_to_abort();
      t.end();
   } before_retry { intP = 0; }

   SLEEP(10000);
}

static void do_pointer_access()
{
   int val = 0;

   for (;;)
   {
      atomic(t)
      {
         if (0 != t.read_ptr(intP)) val = *t.read_ptr(intP);

         cout << val << endl;
      } end_atom
   }
}

static void* pointer_access_test(void*)
{
   boost::stm::transaction::initialize_thread();
   do_pointer_access();
   return 0;
}

void pointer_test()
{
   boost::stm::transaction::initialize();
   boost::stm::transaction::initialize_thread();

   pthread_t *threads = new pthread_t[kMaxThreads];
   int *threadId = new int[kMaxThreads];

   //--------------------------------------------------------------------------
   // Reset barrier variables before creating any threads. Otherwise, it is
   // possible for the first thread
   //--------------------------------------------------------------------------
   threadsFinished.value() = 0;
   threadsStarted.value() = 0;
   startTimer = nMain::kStartingTime;
   endTimer = 0;

   for (int j = 0; j < kMaxThreads - 1; ++j)
   {
      threadId[j] = j;
      pthread_create(&threads[j], 0, pointer_access_test, (void *)&threadId[j]);
   }

   pointer_test_alloc();


}







