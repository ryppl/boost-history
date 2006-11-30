//  libs/rank_list/example/simpletest.cpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
    Rank List performance test

    The code listed here is just a rough test. It shows that
    Rank Lists outperform vector and list UNDER VERY PARTICULAR
    CONDITIONS. Rank Lists are the best choice in cases where
    _BOTH_ kinds of operations (random access and insert/erase at
    any point) are performed frequently in highly populated
    containers.

    Some results can be found at the end of this file.

    See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#include <list>
#include <vector>
#include <deque>
#include <iostream>
#include <ctime>

#include <boost/rank_list.hpp>

using namespace std;
using namespace boost::rank_lists;

// CONFIGURATION --------------------------------------------------

        // Max seconds per fill-flush cycle. For every container,
        // bigger and bigger tests will be performed, until one
        // exceeds max_time. Total time will be much more.

const unsigned max_time = 120U;

        // Number of elements for the first fill-flush cycle.
        // Lower values are interesting too, but current time
        // resolution is 1 second, and frequent calls to time()
        // might jeopardize the results so...

const unsigned initial_size = 10000U;

// HELPER FUNCTIONS -----------------------------------------------

unsigned random (unsigned max) // Return a random number
{                              // in the interval [0,max)
  unsigned n;                  // That is:
                               //  * greatter or equal to 0
  n = (unsigned)               //  * strictly lesser than max
      ( max *
        ( (double)rand() / RAND_MAX ) );

  return n>=max ? max-1 : n;
}

inline list<unsigned>::iterator operator+       // Provide a +
                 (list<unsigned>::iterator it,  // operator for
                  unsigned u)                   // iterators of
{                                               // std::list
  while (u--)
    ++ it;        // Advance u times

  return it;
}

// TESTING ALGORITHM ----------------------------------------------

template<class C,class I>      // The unused parameters enforce C
void test (C unused,           // and I to be the correct classes
           I unused_too,       // An explicit instantiation like
           const char * name)  // test<list...> () failed in some
{                                                // old compilers
  unsigned size, elapsed, start;
  unsigned i, pos, result;

  cout << endl << "Testing: " << name << endl     // Title and
       << "Size\tTime\tFinal element" << endl;    // table header

  size = initial_size;      // Start trying a 'little'
                            // amount of elements, grow it
  for (;;)                  // a 50% and try again... and so on
  {                         // until the time exceeds MAX_TIME
    cout << size << '\t';

    // START COUNTING TIME HERE ----------------vvvvvv-------------
    start = (unsigned) time (NULL);
    elapsed = 0;
                     // Enclose container and iterator in a block
    {                // so that they are destroyed before we
      C container;       // stop counting time
      I it;

      for (i=0; i<size && elapsed<max_time; i++) // Insert 'size'
      {                                          // elements in
        pos = random (i+1);                      // random
        it = container.begin() + pos;            // positions
        container.insert (it, rand());

        elapsed = (unsigned) time (NULL) - start;
      }

      if (i<size)                                // If maximum time
      {                                          // was reached,
        cout << '>' << elapsed                   // abort
             << "\tAborted ("
             << i*100/size << "% inserted)" << endl;

        break;
      }

      for (i=size; i>1 && elapsed<max_time; i--) // Extract
      {                                          // 'size'-1
        pos = random (i);                        // elements from
        it = container.begin() + pos;            // random
        container.erase (it);                    // positions

        elapsed = (unsigned) time (NULL) - start;
      }

      if (i>1)                                   // If maximum time
      {                                          // was reached,
        cout << '>' << elapsed                   // abort
             << "\tAborted (100% inserted, "
             << i*100/size << "% removed)"
             << endl;
        break;
      }

      result = *container.begin();   // Store the result and
    }                                // destroy the container

    // STOP COUNTING TIME HERE ---------------^^^^^^---------------
    elapsed = (unsigned) time (NULL) - start;

    cout << elapsed << '\t'    // Show the result (just for
         << result             // verifying that all containers
         << endl;              // get the same one)

    size += size/2;            // Next time: 150% of current size
  }
}

// PROGRAM BODY ---------------------------------------------------

int main ()
{
  unsigned seed;
                                           // Make a random seed
  seed = (unsigned) time (NULL);           // with the current time
  cout << "Random seed: " << seed << endl;

  srand (seed);
  test (list<unsigned>(),
        list<unsigned>::iterator(),
        "list<unsigned>");
                                           // Test every container
  srand (seed);                            // with the same random
  test (vector<unsigned>(),                // sequence
        vector<unsigned>::iterator(),
        "vector<unsigned>");

  srand (seed);
  test (deque<unsigned>(),
        deque<unsigned>::iterator(),
        "deque<unsigned>");

  srand (seed);
  test (rank_list<unsigned>(),
        rank_list<unsigned>::iterator(),
        "rank_list<unsigned>");

  getchar ();    // Wait until the user presses ENTER
  return 0;
}

/*
-------------------------------------------------------------------

    Results obtained with:

      - Compiler: Dev-C++ 4.9.9.0
        (Project configured for best optimizations and no debug)
      - Processor: Pentium 4 Mobile running at 1.59 GHz
      - RAM: 512 MB
      - MAX_TIME: 120 seconds
      - INITIAL_SIZE: 10000 elements


Random seed: 1161729471

Testing: list<unsigned>
Size    Time    Final element
10000   1       17825
15000   3       23360
22500   8       6148
33750   26      7020
50625   118     14041
75937   >120    Aborted (83% inserted)

Testing: vector<unsigned>
Size    Time    Final element
10000   0       17825
15000   0       23360
22500   0       6148
33750   2       7020
50625   3       14041
75937   6       31946
113905  15      13288
170857  39      32266
256285  >120    Aborted (100% inserted, 54% removed)

Testing: deque<unsigned>
Size    Time    Final element
10000   0       17825
15000   0       23360
22500   1       6148
33750   1       7020
50625   4       14041
75937   9       31946
113905  20      13288
170857  48      32266
256285  >120    Aborted (100% inserted, 55% removed)

Testing: rank_list<unsigned>
Size    Time    Final element
10000   0       17825
15000   0       23360
22500   0       6148
33750   0       7020
50625   0       14041
75937   1       31946
113905  1       13288
170857  1       32266
256285  3       29099
384427  4       2097
576640  7       17736
864960  12      16371
1297440 19      23998
1946160 30      10100
2919240 50      19130
4378860 84      20558
6568290 >120    Aborted (100% inserted, 35% removed)
-------------------------------------------------------------------
*/

