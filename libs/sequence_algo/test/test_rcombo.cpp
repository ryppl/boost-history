// test_rcombo.cpp ------------------------------------------------------//
//   Test program for r-permutation and r-combination generic functions
//   in rcombo.h.

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
#include <cstdlib>      // EXIT_SUCCESS, EXIT_FAILURE
#include <cassert>
#include <sstream>      // ostringstream
#include <functional>   // greater<>
using namespace std;

#ifdef __MWERKS__
#include <console.h>
#endif

//#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include <boost/lexical_cast.hpp>
 
#include "rcombo.hpp"
using namespace boost;

#define DIM(a)    (sizeof(a)/sizeof(a)[0])

// Test data

char numerals[] = { '6', '3', '2', '5', '4', '1' };
const unsigned R = 4;
  
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


// factorial ------------------------------------------------------------//

//   n! cannot exceed numeric_limits<unsigned long>::max() in <limits>
//   or ULONG_MAX in <climits>, otherwise the return value is meaningless.

unsigned long factorial(unsigned n)
{
	unsigned long prod = 1;
	while(n)
		prod *= n--;
	return prod;
}

// factorial ------------------------------------------------------------//

//   This version of factorial is optimized for permutation count
//   calculations. It solves for the property that the number of
//   permutations of P(n, r) is n!/(n-r)! = n(n-1)(n-2)...(n-r+1).
//   The return value cannot exceed numeric_limits<unsigned long>::max()
//   in <limits> or ULONG_MAX in <climits>, otherwise it is meaningless.

unsigned long factorial(unsigned n, unsigned r)
{
	assert(n >= r);
	if (n < r)
		throw std::invalid_argument("factorial: Second argument cannot be larger than the first.");
	const unsigned min = n - r;
	unsigned long prod = 1;
	while (n > min)
		prod *= n--;
	return prod;
}


// valid_r_permutation --------------------------------------------------//

template<class ForwardIterator>
inline bool valid_r_permutation(ForwardIterator /*first*/,
                                ForwardIterator /*middle*/,
                                ForwardIterator /*last*/)
{
    return true;
}   // valid_permutation

template<class ForwardIterator, class Compare>
inline bool valid_r_permutation(ForwardIterator /*first*/,
                                ForwardIterator /*middle*/,
                                ForwardIterator /*last*/,
                                Compare comp)
{
    return true;
}   // valid_permutation


// valid_r_combination --------------------------------------------------//

template<class ForwardIterator>
inline bool valid_r_combination(ForwardIterator first,
                                ForwardIterator middle,
                                ForwardIterator /*last*/)
{
    return is_sorted(first, middle);
}   // valid_r_combination

template<class ForwardIterator, class Compare>
inline bool valid_r_combination(ForwardIterator first,
                                ForwardIterator middle,
                                ForwardIterator /*last*/,
                                Compare comp)
{
    return is_sorted(first, middle, comp);
}   // valid_r_combination

// print_combo_sequence -------------------------------------------------//

template<class InputIterator>
display_sequence(InputIterator first, InputIterator middle,
                 InputIterator last, ostream& os = cout)
{
    copy(first, middle, ostream_iterator<char>(os, "  "));
    os << "|  ";
    copy(middle, last, ostream_iterator<char>(os, "  "));
    os << '\n';
}


// main -----------------------------------------------------------------//

int test_main(int argc, char* argv[])
{
#ifdef __MWERKS__
    argc = ccommand(&argv);
#endif
    bool error = false;

    // test factorial function
    BOOST_CRITICAL_TEST(factorial(0) == 1);
    BOOST_CRITICAL_TEST(factorial(1) == 1);
    BOOST_CRITICAL_TEST(factorial(5) == 120);
    BOOST_CRITICAL_TEST(factorial(6) == 720);
    BOOST_CRITICAL_TEST(factorial(11) == 39916800UL);

    BOOST_CRITICAL_TEST(factorial(0, 0) == 1);
    BOOST_CRITICAL_TEST(factorial(1, 1) == 1);
    BOOST_CRITICAL_TEST(factorial(2, 1) == 2);
    BOOST_CRITICAL_TEST(factorial(2, 2) == 2);
    BOOST_CRITICAL_TEST(factorial(5, 3) == 60);
    BOOST_CRITICAL_TEST(factorial(6, 4) == 360);
    BOOST_CRITICAL_TEST(factorial(7, 0) == 1);
    BOOST_CRITICAL_TEST(factorial(7, 2) == 42);
    BOOST_CRITICAL_TEST(factorial(7, 7) == 5040);

    // test is_sorted function
    vector<char> nums(numerals, numerals + DIM(numerals));
    sort(nums.begin(), nums.end());
    vector<char> checknums(nums);

    BOOST_CRITICAL_TEST(is_sorted(nums.begin(), nums.end()));
    BOOST_CRITICAL_TEST(!is_sorted(nums.rbegin(), nums.rend()));
    random_shuffle(nums.begin(), nums.end());
    // may very well end up sorted, but unlikely.
    BOOST_TEST(!is_sorted(nums.begin(), nums.end()));

    sort(nums.begin(), nums.end(), greater<char>());
    BOOST_CRITICAL_TEST(is_sorted(nums.begin(), nums.end(), greater<char>()));
    BOOST_CRITICAL_TEST(!is_sorted(nums.rbegin(), nums.rend(), greater<char>()));
    random_shuffle(nums.begin(), nums.end());
    // may very well end up sorted, but unlikely.
    BOOST_TEST(!is_sorted(nums.begin(), nums.end(), greater<char>()));

    const unsigned long perm_cnt = factorial(nums.size(), R);
    const unsigned long comb_cnt = factorial(nums.size(), R) / factorial(R);

    // testing next_r_permutation

    sort(nums.begin(), nums.end());
    unsigned count = 0;
    do {
        ++count;
        if (!valid_r_permutation(nums.begin(), nums.begin() + R, nums.end()))
        {
            error = true;
            BOOST_ERROR("Invalid permutation");
        }
    } while(next_r_permutation(nums.begin(), nums.begin() + R, nums.end()));

    if (count != perm_cnt)
    {
        ostringstream msg;
        msg << "next_r_permutation generated "<< count
            << " permutations when there should have been "
            << perm_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }
                           
    if (nums != checknums)
    {
        BOOST_ERROR("next_r_permutation didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }

    // testing prev_r_permutation

    sort(nums.begin(), nums.end(), greater<char>());
    count = 0;
    do {
        ++count;
        if (!valid_r_permutation(nums.begin(), nums.begin() + R, nums.end()))
        {
            error = true;
            BOOST_ERROR("Invalid permutation");
        }
    } while(prev_r_permutation(nums.begin(), nums.begin() + R, nums.end()));

    if (count != perm_cnt)
    {
        ostringstream msg;
        msg << "prev_r_permutation generated "<< count
            << " permutations when there should have been "
            << perm_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    sort(checknums.begin(), checknums.end(), greater<char>());
    if (nums != checknums)
    {
        BOOST_ERROR("prev_r_permutation didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }


    // testing next_r_combination

    sort(nums.begin(), nums.end());
    count = 0;
    do {
        ++count;
        if (!valid_r_combination(nums.begin(), nums.begin() + R, nums.end()))
        {
            error = true;
            BOOST_ERROR("Invalid combination");
        }
    } while(next_r_combination(nums.begin(), nums.begin() + R, nums.end()));

    if (count != comb_cnt)
    {
        ostringstream msg;
        msg << "next_r_combination generated "<< count
            << " combinations when there should have been "
            << comb_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    sort(checknums.begin(), checknums.end());
    if (nums != checknums)
    {
        BOOST_ERROR("next_r_permutation didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }


    // testing prev_r_combination

    sort(nums.begin(), nums.end());
    rotate(nums.begin(), nums.end() - R, nums.end());
    count = 0;
    do {
        ++count;
        if (!valid_r_combination(nums.begin(), nums.begin() + R, nums.end()))
        {
            error = true;
            BOOST_ERROR("Invalid combination");
        }
    } while(prev_r_combination(nums.begin(), nums.begin() + R, nums.end()));

    if (count != comb_cnt)
    {
        ostringstream msg;
        msg << "prev_r_combination generated "<< count
            << " combinations when there should have been "
            << comb_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    sort(checknums.begin(), checknums.end());
    rotate(checknums.begin(), checknums.end() - R, checknums.end());
    if (nums != checknums)
    {
        BOOST_ERROR("prev_r_combination didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }

    sort(checknums.begin(), checknums.end(), greater<char>());

    // testing next_r_permutation using compare functor
    
    sort(nums.begin(), nums.end(), greater<char>());
    count = 0;
    do {
        ++count;
        if (!valid_r_permutation(nums.begin(), nums.begin() + R, nums.end(),
            greater<char>()))
        {
            error = true;
            BOOST_ERROR("Invalid permutation");
        }
    } while(next_r_permutation(nums.begin(), nums.begin() + R, nums.end(),
        greater<char>()));

    if (count != perm_cnt)
    {
        ostringstream msg;
        msg << "next_r_permutation generated "<< count
            << " permutations when there should have been "
            << perm_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    if (nums != checknums)
    {
        BOOST_ERROR("next_r_permutation didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }


    // testing prev_r_permutation using compare functor

    sort(nums.begin(), nums.end());
    count = 0;
    do {
        ++count;
        if (!valid_r_permutation(nums.begin(), nums.begin() + R, nums.end(),
            greater<char>()))
        {
            error = true;
            BOOST_ERROR("Invalid permutation");
        }
    } while(prev_r_permutation(nums.begin(), nums.begin() + R, nums.end(),
        greater<char>()));

    if (count != perm_cnt)
    {
        ostringstream msg;
        msg << "prev_r_permutation generated "<< count
            << " permutations when there should have been "
            << perm_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    sort(checknums.begin(), checknums.end());
    if (nums != checknums)
    {
        BOOST_ERROR("prev_r_permutation didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }


    // testing next_r_combination using compare functor

    sort(nums.begin(), nums.end(), greater<char>());
    count = 0;
    do {
        ++count;
        if (!valid_r_combination(nums.begin(), nums.begin() + R, nums.end(),
            greater<char>()))
        {
            error = true;
            BOOST_ERROR("Invalid permutation");
        }
    } while(next_r_combination(nums.begin(), nums.begin() + R, nums.end(),
        greater<char>()));

    if (count != comb_cnt)
    {
        ostringstream msg;
        msg << "next_r_combination generated "<< count
            << " combinations when there should have been "
            << comb_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    sort(checknums.begin(), checknums.end(), greater<char>());
    if (nums != checknums)
    {
        BOOST_ERROR("next_r_combutation didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }

    // testing prev_r_combination using compare functor
    
    sort(nums.begin(), nums.end(), greater<char>());
    rotate(nums.begin(), nums.end() - R, nums.end());
    count = 0;
    do {
        ++count;
        if (!valid_r_combination(nums.begin(), nums.begin() + R, nums.end(),
            greater<char>()))
        {
            error = true;
            BOOST_ERROR("Invalid permutation");
        }
    } while(prev_r_combination(nums.begin(), nums.begin() + R, nums.end(),
        greater<char>()));

    if (count != comb_cnt)
    {
        ostringstream msg;
        msg << "prev_r_combination generated "<< count
            << " combinations when there should have been "
            << comb_cnt << '\n';
        BOOST_ERROR(msg.str().c_str());
        error = true;
    }

    sort(checknums.begin(), checknums.end(), greater<char>());
    rotate(checknums.begin(), checknums.end() - R, checknums.end());
    if (nums != checknums)
    {
        BOOST_ERROR("prev_r_combination didn't restore sequence to initial value at end of series.");
        error = true;
        display_sequence(nums.begin(), nums.begin() + R, nums.end());
        display_sequence(checknums.begin(), checknums.begin() + R, checknums.end());
    }


    cout << "\nTest of rcombo.hpp is complete." << endl;

    return error ? EXIT_FAILURE : EXIT_SUCCESS;

}   // test_main

