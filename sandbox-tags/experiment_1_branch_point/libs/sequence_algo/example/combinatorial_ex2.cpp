// combinatorial_ex2.cpp - Ask Marilyn Puzzle, Parade magazine 5/5/02 --------------//

// Copyright © Philip F. Garofalo 2002. All rights reserved.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

// See http://www.boost.org for updates, documentation, and revision
// history.


/*
  This program solves the following puzzle that appeared in the "Ask Marilyn"
  column in Parade magazine, May 5, 2002. 

    "Dear Marilyn,
    Can you combine these 12 three-letter words to form three 12-letter words
    instead? (Example: car + tog + rap + her = cartographer.)

        ate con fir his ion man
        mat new per sop spa tic

    --Misty Covington, Sparks, MD."
*/

#ifndef NDEBUG
#pragma message("NDEBUG not defined. Assertions are active.")
#endif

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <exception>
#include <stdexcept>

#include <cstdlib>
#include <cassert>
using namespace std;

#include "combinatorial.hpp"        // next_r_permutation()

#define DIM(a)    (sizeof(a)/sizeof(a)[0])

const char filename[] = "lexicon.txt";
vector<string> theDictionary;

const char tri[12][4] = {
    "ate",
    "con",
    "fir",
    "his",
    "ion",
    "man",
    "mat",
    "new",
    "per",
    "sop",
    "spa",
    "tic"
};
#if !defined _MSC_VER
vector<string> theSegs(tri, &tri[12]);
#else
// I couldn't assuage MSC about the pointer parameters in the above
// definition so, I'll define it with the default constructor here
// and initialize it in main().
vector<string> theSegs;
#endif

const int R = 4;    // select 4 three-letter words at a time

#if _MSC_VER <= 1200

// Versions 6.0 and earlier of Microsoft's C++ compiler do not support
// partial template specialization, so we'll need to explicitly specialize
// iterator_traits for the container type, which is string* in this example
// application. It is required only before the call to the default version
// of prev_r_permutation,the one without the user-supplied compare function.

namespace std {
   template<>
   struct iterator_traits<string*> {
       typedef random_access_iterator_tag iterator_category;
       typedef ptrdiff_t difference_type;
       typedef string value_type;
       typedef string* pointer;
       typedef string& reference;
   };
}   // namespace std

#endif  // MSC_VER

// WordSearch -----------------------------------------------------------//

// This function prints out the current set of 3-letter word segments
// and then searchs the dictionary for each 4-permutation of those
// segments.

void WordSearch()
{
    // sort to start the permutation series
    partial_sort(theSegs.begin(), theSegs.begin() + R, theSegs.end());

    // display the initial set of 3-letter word segments
    copy(theSegs.begin(), theSegs.end(), ostream_iterator<string>(cout, "\t"));
    cout << '\n';
    
    do {
        string trial_word = theSegs[0] + theSegs[1] + theSegs[2] + theSegs[3];
        if (binary_search(theDictionary.begin(), theDictionary.end(), trial_word))
        {
            cout << trial_word << '\n';
            break;
        }    // if
    } while(boost::next_r_permutation(theSegs.begin(), theSegs.begin() + R,
        theSegs.end()));

    // Delete the segments for the found word.
    theSegs.erase(theSegs.begin(), theSegs.begin() + R);

}    // WordSearch()
    
// main -----------------------------------------------------------------//

// Solves the "Ask Marilyn" puzzle in two ways. The first is simple
// but somewhat inefficient. It was my initial attempt at solving the
// problem using next_r_permutation. It goes through the series of 4-
// permutations of the 12 segments until it finds the three 12-letter
// words. Assuming that the words are evenly spaced through the permutation
// series then the last word will be 3/4 of the way through
// the series. That will mean about 3/4 * 11,880 (P(12, 4)) or 8910
// dictionary lookups. 

// The second method interrupts the search after each 12-letter word
// is found in the dictionary to subtract the 4 3-letter words 
// comprising the larger word from the collection of segments, and
// then reprises the search with the smaller collection of segments.
// So instead of going through the entirety of P(12, 4), it steps
// through P(12,4), P(8, 4), P(4, 4). Assuming that the desired
// word is approximately half way through each of those permutation
// series, then the number of dictionary lookups will be
// 11,880/2 + 1680/2 + 24/2 = 6792. Approximately 23.77% fewer lookups.

int main()
{
    cout << "From \"Ask Marilyn\", Parade magazine 5/5/02\n\n";

    cout <<    "Dear Marilyn,\n\
  Can you combine these 12 three-letter words to form three 12-letter words\n\
  instead? (Example: car + tog + rap + her = cartographer.)\n\
\
    ate con fir his ion man\n\
    mat new per sop spa tic\n\
\
  --Misty Covington, Sparks, MD.\n\n";

#if defined _MSC_VER
    // See comment above on the definition of theSegs vector.
    copy(&tri[0], &tri[DIM(tri)], back_inserter(theSegs));
#endif
    
    // open the dictionary

    ifstream ifs(filename);
    if (!ifs)
    {
        cerr << "Couldn't open " << filename << endl;
        return EXIT_FAILURE;
    }

    // Copy words from dictionary file to a vector:
    typedef istream_iterator<string> string_input;

    cout << "\nReading the dictionary file " << filename << " ..." << flush;

    // Loading the dictionary will use up a lot of memory here. MacOS
    // developers make sure to set the application's minimum memory
    // requirement to at least 1 MB.
    copy(string_input(ifs), string_input(), back_inserter(theDictionary));

    cout << "\nThe dictionary contains " << theDictionary.size() << " words.\n\n";
    
    // First method; search the dictionary for every permutation. Stop after
    // the third word is found.

    cout << "\n\n--- First lookup method ---\n\n";

    //  No need for an initial sort; theSegs is already sorted.
    int count = 0;
    do {
        string trial_word = theSegs[0] + theSegs[1] + theSegs[2] + theSegs[3];
        if (binary_search(theDictionary.begin(), theDictionary.end(), trial_word))
        {
            cout << ++count << ". " << trial_word << '\n';
        }
    } while(count < 3 &&
        boost::next_r_permutation(theSegs.begin(), theSegs.begin() + R,
            theSegs.end()));


    // Second method; progressively reduce the permutation set by four after
    // each word is found to reduce the number of permutations to search,.

    cout << "\n\n--- More efficient way to solve the same problem ---\n\n";
    WordSearch();   // find first word
    WordSearch();   // find second word
    WordSearch();   // find third and last word
        
    cout << "\n\nAll done!" << endl;
    return EXIT_SUCCESS;

}    // main
