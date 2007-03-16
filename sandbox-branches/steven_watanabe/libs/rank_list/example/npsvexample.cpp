//  libs/rank_list/example/npvsexample.cpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
    Rank List "Non-Proportional Sequence View" example

    The feature called "Non-Proportional Sequence View" (NPSV)
    exploits, in a more general form, the same principle that Rank
    List uses for indexing the sequence with natural numbers.

    The difference is that in the normal sequence view every
    element counts as 1, while in the NPSV each element can count
    as a 'width' that can be changed. The position of every element
    is the sum of the widths of the previous elements in the
    sequence order. Negative widths are forbidden, but zero width
    is allowed.

    When an element has zero width, the next one seems to be in
    the same position from the NPSV point of view. In these cases
    the lookup at this shared position always returns the first
    element of the 'same position' group.

    The type of the width is a parameter of the rank_list template,
    providing a lot of possibilities: unsigned, double, or any
    class providing some overloaded operators =, +, -, +=, -=, ==,
    !=, >, <, <=, >= and a conversion from int, which will be used
    for 0 and 1.

    The output of the program is shown in comments along the code.

    See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#include <iostream>

#include <boost/rank_list.hpp>

using namespace std;
using namespace boost::rank_lists;

// PROGRAM BODY ---------------------------------------------------

int main ()
{
  typedef
  rank_list<char,                  // Use chars for the example
            std::allocator<char>,  // Same as default allocator
            unsigned> A;           // NPSV width type: unsigned

  A a;             // Rank List of chars and NPSV unsigned
  A::iterator it;  // An iterator for a
  unsigned u;

  cout << "We populate a rank_list with letters from a to f"
       << endl;

  for (u=0; u<6; u++)       // Populate with letters
    a.push_back ('a'+u);    // from 'a' to 'f'

  cout << "Normal view with iterators:" << endl;

  for (it=a.begin(); it!=a.end(); ++it)  // This is the fast,
    cout << *it << " ";                  // right way to traverse
  cout << endl;                          // a rank_list  (O(n))
  // Output:
  // a b c d e f

  cout << "Normal view with index operator (slower):" << endl;

  for (u=0; u<a.size(); u++)  // This is the slow, stupid way to
    cout << a[u] << " ";      // do it (O(n log(n))), but it shows
  cout << endl;               // the normal view
  // Output:
  // a b c d e f

  // All widths have been set to 1 by default, so if we use the
  // NPSV we will get the same result

  cout << "Initial Non-Proportional Sequence View (NPSV):" << endl;

  for (u=0; u<a.npsv_width(); u++)     // NPSV instead of size
    cout << *a.npsv_at_pos(u) << " ";  // NPSV instead of []
  cout << endl;
  // Output:
  // a b c d e f

  cout << "Now we set the width of b to 3." << endl;

  a.npsv_set_width (a.begin()+1, 3);   // Width of 'b': 3

  cout << "New NPSV:" << endl;

  for (u=0; u<a.npsv_width(); u++)     // NPSV instead of size
    cout << *a.npsv_at_pos(u) << " ";  // NPSV instead of []
  cout << endl;
  // Output:
  // a b b b c d e f        <--- b appears three times!
  //                             That's because we are
  //                             incrementing u by one every
  //                             time, while a is 3 units wide

  cout << "Now we set the width of c to 0." << endl;

  a.npsv_set_width (a.begin()+2, 0);   // Width of 'c': 0

  cout << "New NPSV:" << endl;

  for (u=0; u<a.npsv_width(); u++)
    cout << *a.npsv_at_pos(u) << " ";
  cout << endl;
  // Output:
  // a b b b c e f          <--- d does not appear!
  //                             That's because c and d are now
  //                             at the same NPSV position

  cout << "Now we refer to c with an iterator:" << endl;

  it = a.npsv_at_pos (4);
  cout << *it << endl;        // Use an iterator to get
  // Output:                  // to the 'c'
  // c

  cout << "Now we advance with the iterator:" << endl;

  ++ it;                      // Advance

  cout << *it << endl;        // Show what is there
  // Output:
  // d                      <--- Ok, so d was still there
  //                             We see it because, with the
  //                             iterator, we advanced through
  //                             the normal sequence view.
  //                             In fact...

  cout << "Normal view with iterators:" << endl;

  for (it=a.begin(); it!=a.end(); ++it)  // ...if we try now
    cout << *it << " ";                 // the first loops,
  cout << endl;                        // we see that the normal
  // Output:                          // sequence is still there,
  // a b c d e f                     // unchanged

  cout << "Normal view with index operator (slower):" << endl;

  for (u=0; u<a.size(); u++)     // Remember that the NPSV is
    cout << a[u] << " ";        // just a view of the sequence,
  cout << endl;                // not the sequence itself
  // Output:
  // a b c d e f

  cout << "Now we set the width of f (last element) to 0." << endl;

  a.npsv_set_width (a.end()-1, 0);     // Width of 'f': 0

  cout << "New NPSV:" << endl;

  for (u=0; u<a.npsv_width(); u++)
    cout << *a.npsv_at_pos(u) << " ";
  cout << endl;
  // Output:
  // a b b b c e            <--- f does not appear
  //                             That's because the position of
  //                             f is exactly the width of the
  //                             rank_list (see loop condition)

  cout << "New NPSV using <= in the loop condition:" << endl;

  for (u=0; u<=a.npsv_width(); u++)    // <--- Note the <=
    cout << *a.npsv_at_pos(u) << " ";  //      (dangerous if f
  cout << endl;                        //      had non-zero width)
  // Output:
  // a b b b c e f          <--- now f _does_ appear
  //                             That's because this time we did
  //                             look at that strange position

  cout << "New NPSV using end() in the loop condition:" << endl;

  for (u=0; (it=a.npsv_at_pos(u))!=a.end(); u++) // Correct, safe
    cout << *it << " ";                          // loop
  cout << endl;
  // Output:
  // a b b b c e f

  it = a.npsv_at_pos (100);

  cout << "npsv_at_pos (100) returns an iterator to..." << endl;

  if (it==a.end())
    cout << "\"THE END\"  ;-)" << endl;
  else
    cout << *it << endl;

  getchar ();    // Wait until the user presses ENTER
  return 0;
}

