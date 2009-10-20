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

#include "main.h"

/*
   cout << "DracoSTM usage:" << endl << endl;
   cout << "  -bench <name> - 'rbtree', 'linkedlist', 'hashmap' (or 'hashtable')" << endl;
   cout << "                  'irrevocable_int'" << endl;
   cout << "                  'isolated_int'" << endl;
   cout << "                  'isolated_int_lock'" << endl;
   cout << "                  'isolated_composed_int_lock'" << endl;
   cout << "                  'isolated_composed_int_lock2'" << endl;
   cout << "                  'linkedlist_w_locks' (uses transactions and locks)" << endl;
   cout << "                  'hashmap_w_locks' (uses transactions and locks)" << endl;
   cout << "                  'list_hash_w_locks' (uses transactions and locks)" << endl;
   cout << "                  'tx_linear_lock' (uses transactions and multiple linearly ordered locks)" << endl;
   cout << "                  'lot_example' (for paper)" << endl;
   cout << "  -def          - do deferred updating transactions" << endl;
   cout << "  -dir          - do direct updating transactions" << endl;
   cout << "  -latm <name>  - 'full', 'tm', 'tx'" << endl;
   cout << "  -h            - shows this help (usage) output" << endl;
   cout << "  -inserts <#>  - sets the # of inserts per container per thread" << endl;
   cout << "  -threads <#>  - sets the # of threads" << endl;
   cout << "  -lookup       - performs individual lookup after inserts" << endl;
   cout << "  -remove       - performs individual remove after inserts/lookup" << endl;
*/
////////////////////////////////////////////////////////////////////////////

void set_default_environment() {

    kMaxArrSize = 10;
    kMaxArrIter = 10;
    kMaxIterations = 1;
    kMaxInserts = 100;
    kDoRemoval = false;
    kDoLookup = false;
    kMoveSemantics = false;

    bench = "isolated_int_lock";
    updateMethod = "direct";
    kMaxThreads = 2;
    latmType="full";
}

