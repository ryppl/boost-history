// combinatorial_ex1.cpp -- Test program for partial permutation and partial combination generic functions
//                   in combinatorial.h.

// Copyright © Philip F. Garofalo 2008. All rights reserved.
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
// prev_partial_permutation,the one without the user-supplied compare function.
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
    std::copy(numerals, numerals + r, std::ostream_iterator<char>(std::cout, "  "));
    std::cout << "|  ";
    std::copy(numerals + r, numerals + DIM(numerals),
        std::ostream_iterator<char>(std::cout, "  "));
    std::cout << '\n';
}    // PrintLetters

char theMsg[] = "\nPlease select a function to test:\n\n"
                "1. Next partial permutation\n"
                "2. Previous partial permutation\n"
                "3. Next partial combination\n"
                "4. Previous partial combination\n"
                "5. Next partial permutation with compare functor\n"
                "6. Previous partial permutation with compare functor\n"
                "7. Next partial combination with compare functor\n"
                "8. Previous partial combination with compare functor\n"
                "9. Quit\n"
                "\nEnter the number of your selection: ";

// main -----------------------------------------------------------------//


int main(int argc, char** argv)
{
    unsigned r = argc < 2 ? 2 : lexical_cast<unsigned>(argv[1]);

    std::cout << "\nr is equal to " << r << ". Change on command line, e.g., "
        << argv[0] << " 5\n\n";
    
     unsigned selection;
     while(true)
     {    
        std::cout << theMsg;
	    std::cin >> selection;
	    
	    std::cout << "\nPartial permutations and partial combinations appear to the left of the\n"
	        << "vertical bar.\n";

	    unsigned count = 1;
	    
	    switch(selection)
	    {
	    case 1:
	        std::cout << "\n\tNext partial permutations\n\n";
	        std::partial_sort(numerals, numerals + r, numerals + DIM(numerals));
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_partial_permutation(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 2:
	        std::cout << "\n\tPrevious partial permutations\n\n";
	        std::partial_sort(numerals, numerals + r, numerals + DIM(numerals),
                std::greater<char>());
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_partial_permutation(numerals, numerals + r,
                numerals + DIM(numerals)));
	        break;
	    case 3:
	        std::cout <<  "\n\tNext partial combinations\n\n";
	        std::partial_sort(numerals, numerals + r, numerals + DIM(numerals));
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_partial_combination(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 4:
	        std::cout <<  "\n\tPrevious partial combinations\n\n";
	        std::sort(numerals, numerals + DIM(numerals));
	        std::rotate(numerals, numerals + DIM(numerals) - r, numerals + DIM(numerals));
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_partial_combination(numerals, numerals + r, numerals + DIM(numerals)));
	        break;
	    case 5:
	        std::cout <<  "\n\tNext partial permutations using compare functor\n\n";
	        std::partial_sort(numerals, numerals + r, numerals + DIM(numerals),
                std::greater<char>());
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_partial_permutation(numerals, numerals + r,
                numerals + DIM(numerals), std::greater<char>()));
	        break;
	    case 6:
	        std::cout <<  "\n\tPrevious partial permutations using compare functor\n\n";
	        std::partial_sort(numerals, numerals + r, numerals + DIM(numerals));
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_partial_permutation(numerals, numerals + r,
                numerals + DIM(numerals), std::greater<char>()));
	        break;
	    case 7:
	        std::cout <<  "\n\tNext partial combinations using compare functor\n\n";
	        std::partial_sort(numerals, numerals + r, numerals + DIM(numerals),
                std::greater<char>());
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(next_partial_combination(numerals, numerals + r,
                numerals + DIM(numerals), std::greater<char>()));
	        break;
	    case 8:
	        std::cout <<  "\n\tPrevious partial combinations using compare functor\n\n";
	        std::sort(numerals, numerals + DIM(numerals), std::greater<char>());
	        std::rotate(numerals, numerals + DIM(numerals) - r,
                numerals + DIM(numerals));
	        do {
	            std::cout << std::setw(3) << count++ << ". ";
	            PrintLetters(r);
	        } while(prev_partial_combination(numerals, numerals + r,
                numerals + DIM(numerals), std::greater<char>()));
	        break;
	    case 9:
	    default:
		    std::cout << "\nAll done!" << std::endl;
	    	return EXIT_SUCCESS;
	    }    // switch
	    
	    // Print numerals again to verify that last function call resets string.
	    std::cout << "  1. ";
	    PrintLetters(r);
    }	// while
    
    return 0;
}	// main

