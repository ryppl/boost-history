// combinatorial_ex1.cpp -- Test program for r-permutation and r-combination generic functions
//                   in combinatorial.h.

// Copyright © Philip F. Garofalo 2002. All rights reserved.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

// See http://www.boost.org for updates, documentation, and revision
// history.


#include <iostream>
#include <iomanip>        
#include <iterator>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

#ifdef __MWERKS__
#include <console.h>
#endif

#include <boost/lexical_cast.hpp>
 
#include "combinatorial.hpp"
using namespace boost;

#define DIM(a)    (sizeof(a)/sizeof(a)[0])

char numerals[] = { '6', '3', '2', '5', '4', '1' };
  
#if _MSC_VER <= 1200
//
// Versions 6.0 and earlier of Microsoft's C++ compiler do not support
// partial template specialization, so we'll need to explicitly specialize
// iterator_traits for the container type, which is char* in this test app.
// It is required only before the call to the default version of
// prev_r_permutation,the one without the user-supplied compare function.
//
namespace std {
   template<>
   struct iterator_traits<char*> {
       typedef random_access_iterator_tag iterator_category;
       typedef ptrdiff_t difference_type;
       typedef char value_type;
       typedef char* pointer;
       typedef char& reference;
   };
}   // namespace std
#endif


// PrintLetters ---------------------------------------------------------//



//   This function prints a line of letters with a vertical bar

//   separating the the first r letters from the remainder.


inline void PrintLetters(int r)
{
    copy(numerals, numerals + r, ostream_iterator<char>(cout, "  "));
    cout << "|  ";
    copy(numerals + r, numerals + DIM(numerals), ostream_iterator<char>(cout, "  "));
    cout << '\n';
}    // PrintLetters



// main -----------------------------------------------------------------//


int main(int argc, char** argv)
{
#ifdef __MWERKS__
    argc = ccommand(&argv);
#endif
    
    unsigned r = argc < 2 ? 2 : lexical_cast<unsigned>(argv[1]);

    cout << "\nr is equal to " << r << ". Change on command line, e.g., "
        << argv[0] << " 5\n\n";
    
    cout << "Please select a function to test:\n\n"
        << "1. Next r-permutation\n"
        << "2. Previous r-permutation\n"
        << "3. Next r-combination\n"
        << "4. Previous r-combination\n"
        << "5. Next r-permutation with compare functor\n"
        << "6. Previous r-permutation with compare functor\n"
        << "7. Next r-combination with compare functor\n"
        << "8. Previous r-combination with compare functor\n"
        << "9. Quit\n";
    
     unsigned selection;
     while(true)
     {    
	    cout << "\nEnter the number of your selection: ";
	    cin >> selection;
	    
	    cout << "\nR-permutations and r-combinations appear to the left of the\n"
	        << "vertical bar.\n";

	    unsigned count = 1;
	    
	    switch(selection)
	    {
	    case 1:
	        cout << "\n\tNext r-permutations\n\n";
	        partial_sort(numerals, numerals + r, numerals + DIM(numerals));
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_r_permutation(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 2:
	        cout << "\n\tPrevious r-permutations\n\n";
	        partial_sort(numerals, numerals + r, numerals + DIM(numerals), greater<char>());
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_r_permutation(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 3:
	        cout <<  "\n\tNext r-combinations\n\n";
	        partial_sort(numerals, numerals + r, numerals + DIM(numerals));
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_r_combination(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 4:
	        cout <<  "\n\tPrevious r-combinations\n\n";
	        sort(numerals, numerals + DIM(numerals));
	        rotate(numerals, numerals + DIM(numerals) - r, numerals + DIM(numerals));
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_r_combination(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 5:
	        cout <<  "\n\tNext r-permutations using compare functor\n\n";
	        partial_sort(numerals, numerals + r, numerals + DIM(numerals), greater<char>());
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_r_permutation(numerals, numerals + r, numerals + DIM(numerals), greater<char>()));
	        break;
	    case 6:
	        cout <<  "\n\tPrevious r-permutations using compare functor\n\n";
	        partial_sort(numerals, numerals + r, numerals + DIM(numerals));
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_r_permutation(numerals, numerals + r, numerals + DIM(numerals), greater<char>()));
	        break;
	    case 7:
	        cout <<  "\n\tNext r-combinations using compare functor\n\n";
	        partial_sort(numerals, numerals + r, numerals + DIM(numerals), greater<char>());
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_r_combination(numerals, numerals + r, numerals + DIM(numerals), greater<char>()));
	        break;
	    case 8:
	        cout <<  "\n\tPrevious r-combinations using compare functor\n\n";
	        sort(numerals, numerals + DIM(numerals), greater<char>());
	        rotate(numerals, numerals + DIM(numerals) - r, numerals + DIM(numerals));
	        do {
	            cout << setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_r_combination(numerals, numerals + r, numerals + DIM(numerals), greater<char>()));
	        break;
	    case 9:
	    default:
		    cout << "\nAll done!" << endl;
	    	return EXIT_SUCCESS;
	    }    // switch
	    
	    // Print numerals again to verify that last function call resets string.
	    cout << "  1. ";
	    PrintLetters(r);
    }	// while
    
    return 0;
}	// main

