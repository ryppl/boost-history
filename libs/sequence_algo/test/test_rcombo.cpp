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

// This program exercises all of the functions in rcombo.h.
// First all of the lowest level support functions are tested.
// Then the combination and permutation functions are tested for
// correctness in four aspects:

// 1. Internal sort order is correct for combination sequences.
// 2. Each sequence in the series is in lexicographical
//    order with respect to the previously generated sequence.
// 3. The total number of combinations or permutations
//    is correct.
// 4. The last sequence is equal to the first.

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

// macros ---------------------------------------->>

#define DIM(a)    (sizeof(a)/sizeof(a)[0])

// externals

namespace boost {
	namespace test {
		extern int test_tools_errors;	// defined in test_main.cpp
	}
}

// Test data ---------------------------------------->>

char theNumerals[] = { '6', '3', '2', '5', '4', '1' };
  
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


// format_sequence -------------------------------------------------//

template<class InputIterator>
void format_sequence(InputIterator first, InputIterator middle,
                 InputIterator last, ostream& os = cout)
{
    copy(first, middle, ostream_iterator<char>(os, "  "));
    os << "|  ";
    copy(middle, last, ostream_iterator<char>(os, "  "));
    //os << '\n';
}


// invalid_combination_msg ----------------------------------------------//

template<class T>
string invalid_sequence_msg(const string& seq_type, unsigned n, const T& con, int r)
{
    ostringstream msg;
    msg << "Invalid " << seq_type << " #" << n << " of P(" << con.size()
    	<< ", " << r << "): ";
    format_sequence(con.begin(), con.begin() + r, con.end(), msg);
    return msg.str();
}	// invalid_sequence_msg

// invalid_combination_msg ----------------------------------------------//

template<class T>
inline string invalid_combination_msg(unsigned n, const T& con, int r)
{
	return invalid_sequence_msg("combination", n, con, r);
}

// invalid_permutation_msg ----------------------------------------------//

template<class T>
inline string invalid_permutation_msg(unsigned n, const T& con, int r)
{
	return invalid_sequence_msg("permutation", n, con, r);
}

// valid_r_permutation --------------------------------------------------//

template<class ForwardIterator>
bool valid_next_r_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is greater than previous
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle);
	
	prev = vector<T>(first, middle);

    return retval;
}   // valid_next_r_permutation

// valid_next_r_permutation ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_next_r_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is greater than previous
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle, comp);
	
	prev = vector<T>(first, middle);

    return retval;
}   // valid_next_r_permutation

// valid_prev_r_permutation ---------------------------------------------//

template<class ForwardIterator>
bool valid_prev_r_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is greater than previous
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end());
	
	prev = vector<T>(first, middle);

    return retval;
}   // valid_prev_r_permutation

// valid_prev_r_permutation ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_prev_r_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is greater than previous
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end(), comp);
	
	prev = vector<T>(first, middle);

    return retval;
}   // valid_prev_r_permutation

// valid_r_combination --------------------------------------------------//

template<class ForwardIterator>
bool valid_next_r_combination(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
    if (!is_sorted(first, middle))
    	return false;
    	
   	// and greater than the last one
 	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is greater than last
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle);
	
	prev = vector<T>(first, middle);

    return retval;

}   // valid_next_r_combination

// valid_prev_r_combination ---------------------------------------------//

template<class ForwardIterator>
bool valid_prev_r_combination(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
    if (!is_sorted(first, middle))
    	return false;
    	
   	// and less than the last one
 	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is less than last
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end());
	
	prev = vector<T>(first, middle);

    return retval;

}   // valid_prev_r_combination

// valid_next_r_combination ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_next_r_combination(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
    if (!is_sorted(first, middle, comp))
    	return false;
    	
	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is greater than last
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle, comp);
	
	prev = vector<T>(first, middle);

    return retval;
}   // valid_next_r_combination

// valid_prev_r_combination ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_prev_r_combination(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
    if (!is_sorted(first, middle, comp))
    	return false;
    	
	typedef typename iterator_traits<ForwardIterator>::value_type T;
	static vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure curr sequence is less than last
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end(), comp);
	
	prev = vector<T>(first, middle);

    return retval;
}   // valid_prev_r_combination

// test_factorial -------------------------------------------------------//

void test_factorial()
{
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
}   // test_factorial

// test_is_sorted -------------------------------------------------------//

template<class T>
void test_is_sorted(T& seq)
{
    sort(seq.begin(), seq.end());
    BOOST_CRITICAL_TEST(is_sorted(seq.begin(), seq.end()));
    BOOST_CRITICAL_TEST(!is_sorted(seq.rbegin(), seq.rend()));
    random_shuffle(seq.begin(), seq.end());
    // may very well end up sorted, but unlikely.
    BOOST_TEST(!is_sorted(seq.begin(), seq.end()));

    sort(seq.begin(), seq.end(), greater<char>());
    BOOST_CRITICAL_TEST(is_sorted(seq.begin(), seq.end(), greater<char>()));
    BOOST_CRITICAL_TEST(!is_sorted(seq.rbegin(), seq.rend(), greater<char>()));
    random_shuffle(seq.begin(), seq.end());
    // may very well end up sorted, but unlikely.
    BOOST_TEST(!is_sorted(seq.begin(), seq.end(), greater<char>()));
}   // test_is_sorted

// test_smallest_greater ------------------------------------------------//

void test_smallest_greater(vector<char>& nums)
{
    BOOST_CRITICAL_TEST(smallest_greater(nums.begin(), nums.end(), '3')
    	== find(nums.begin(), nums.end(), '4'));
    BOOST_CRITICAL_TEST(smallest_greater(nums.begin(), nums.end(), '9') == nums.end());
    BOOST_CRITICAL_TEST(smallest_greater(nums.begin(), nums.end(), '6') == nums.end());

    BOOST_CRITICAL_TEST(smallest_greater(nums.begin(), nums.end(), '3', greater<char>())
    	== find(nums.begin(), nums.end(), '2'));
    BOOST_CRITICAL_TEST(smallest_greater(nums.begin(), nums.end(), '9', greater<char>())
    	== find(nums.begin(), nums.end(), '6'));
    BOOST_CRITICAL_TEST(smallest_greater(nums.begin(), nums.end(), '6', greater<char>())
    	==  find(nums.begin(), nums.end(), '5'));
}   // test_smallest_greater

// test_largest_less ----------------------------------------------------//

void test_largest_less(vector<char>& nums)
{
    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '4')
    	== find(nums.begin(), nums.end(), '3'));
    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '1') == nums.end());
    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '0') == nums.end());

    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '4', greater<char>())
    	== find(nums.begin(), nums.end(), '5'));
    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '1', greater<char>())
    	== find(nums.begin(), nums.end(), '2'));
    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '0', greater<char>())
    	== find(nums.begin(), nums.end(), '1'));
    BOOST_CRITICAL_TEST(largest_less(nums.begin(), nums.end(), '7', greater<char>())
    	== nums.end());
}   // test_largest_less

// ----------------------------------------------------------------------//
// In the following functions, each combinatorial function is tested
// for three properties:
//   1. Each generated sequence is valid. I.e., objects within
//      the sequence are lexicographically sorted (combinations only),
//      and that each sequence follows the previous sequence 
//      in the proper lexicographical series order.
//   2. The total number of generated sequences is equal to the
//      expected number.
//   3. The last sequence is equal to the first.

// test_next_r_permutation ----------------------------------------------//

void test_next_r_permutation(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end());
    vector<char> checknums = nums;
	unsigned count = 0;
	do {
	    ++count;
	    if (!valid_next_r_permutation(nums.begin(), nums.begin() + r, count == 1))
	    {
	        BOOST_ERROR(invalid_permutation_msg(count, nums, r).c_str());
	    }
	} while(next_r_permutation(nums.begin(), nums.begin() + r, nums.end()));

    const unsigned long perm_cnt = factorial(nums.size(), r);
	if (count != perm_cnt)
	{
	    ostringstream msg;
	    msg << "next_r_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}
	                       
	if (nums != checknums)
	{
	    BOOST_ERROR("next_r_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    cout << '\n';
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	    cout << '\n';
	}
}   // test_next_r_permutation

// test_next_r_permutation_comp -----------------------------------------//

void test_next_r_permutation_comp(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end(), greater<char>());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
	    ++count;
	    if (!valid_next_r_permutation(nums.begin(), nums.begin() + r,
	        greater<char>(), count == 1))
	    {
	        BOOST_ERROR(invalid_permutation_msg(count, nums, r).c_str());
	    }
	} while(next_r_permutation(nums.begin(), nums.begin() + r, nums.end(),
	    greater<char>()));

    const unsigned long perm_cnt = factorial(nums.size(), r);
	if (count != perm_cnt)
	{
	    ostringstream msg;
	    msg << "next_r_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("next_r_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_next_r_permutation_comp

// test_prev_r_permutation ----------------------------------------------//

void test_prev_r_permutation(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end(), greater<char>());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
	    ++count;
	    if (!valid_prev_r_permutation(nums.begin(), nums.begin() + r, count == 1))
	    {
	        BOOST_ERROR(invalid_permutation_msg(count, nums, r).c_str());
	    }
	} while(prev_r_permutation(nums.begin(), nums.begin() + r, nums.end()));

    const unsigned long perm_cnt = factorial(nums.size(), r);
	if (count != perm_cnt)
	{
	    ostringstream msg;
	    msg << "prev_r_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("prev_r_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_prev_r_permutation

// test_prev_r_permutation_comp -------------------------------------------//

void test_prev_r_permutation_comp(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
	    ++count;
	    if (!valid_prev_r_permutation(nums.begin(), nums.begin() + r,
	        greater<char>(), count == 1))
	    {
	        BOOST_ERROR(invalid_permutation_msg(count, nums, r).c_str());
	    }
	} while(prev_r_permutation(nums.begin(), nums.begin() + r, nums.end(),
	    greater<char>()));

    const unsigned long perm_cnt = factorial(nums.size(), r);
	if (count != perm_cnt)
	{
	    ostringstream msg;
	    msg << "prev_r_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("prev_r_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_prev_r_permutation_comp

// test_next_r_combination ----------------------------------------------//

void test_next_r_combination(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
        ++count;
	    if (!valid_next_r_combination(nums.begin(), nums.begin() + r, count == 1))
        	BOOST_ERROR(invalid_combination_msg(count, nums, r).c_str());
	} while(next_r_combination(nums.begin(), nums.begin() + r, nums.end()));

    const unsigned long comb_cnt = factorial(nums.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    ostringstream msg;
	    msg << "next_r_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("next_r_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_next_r_combination

// test_next_r_combination_comp -----------------------------------------//

void test_next_r_combination_comp(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end(), greater<char>());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
        ++count;
	    if (!valid_next_r_combination(nums.begin(), nums.begin() + r,
	        greater<char>(), count == 1))
        	BOOST_ERROR(invalid_combination_msg(count, nums, r).c_str());
	} while(next_r_combination(nums.begin(), nums.begin() + r, nums.end(),
	    greater<char>()));

    const unsigned long comb_cnt = factorial(nums.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    ostringstream msg;
	    msg << "next_r_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("next_r_combutation didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_next_r_combination_comp

// test_prev_r_combination ----------------------------------------------//

void test_prev_r_combination(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end());
	rotate(nums.begin(), nums.end() - r, nums.end());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
        ++count;
	    if (!valid_prev_r_combination(nums.begin(), nums.begin() + r, count == 1))
        	BOOST_ERROR(invalid_combination_msg(count, nums, r).c_str());
	} while(prev_r_combination(nums.begin(), nums.begin() + r, nums.end()));

    const unsigned long comb_cnt = factorial(nums.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    ostringstream msg;
	    msg << "prev_r_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("prev_r_combination didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_prev_r_combination

// test_prev_r_combination_comp -----------------------------------------//

void test_prev_r_combination_comp(vector<char>& nums, int r)
{
	sort(nums.begin(), nums.end(), greater<char>());
	rotate(nums.begin(), nums.end() - r, nums.end());
	vector<char> checknums = nums;
	unsigned count = 0;
	do {
        ++count;
	    if (!valid_prev_r_combination(nums.begin(), nums.begin() + r,
	        greater<char>(), count == 1))
        	BOOST_ERROR(invalid_combination_msg(count, nums, r).c_str());
	} while(prev_r_combination(nums.begin(), nums.begin() + r, nums.end(),
	    greater<char>()));

    const unsigned long comb_cnt = factorial(nums.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    ostringstream msg;
	    msg << "prev_r_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (nums != checknums)
	{
	    BOOST_ERROR("prev_r_combination didn't restore sequence to initial value at end of series.");
	    format_sequence(nums.begin(), nums.begin() + r, nums.end());
	    format_sequence(checknums.begin(), checknums.begin() + r, checknums.end());
	}
}   // test_prev_r_combination_comp


// test_main ------------------------------------------------------------//

int test_main(int argc, char* argv[])
{
#ifdef __MWERKS__
    // argc = ccommand(&argv);
#endif

    // test factorial function ---------------------------------------->>
    test_factorial();

    vector<char> nums(theNumerals, theNumerals + DIM(theNumerals));

    // test is_sorted function ---------------------------------------->>
    test_is_sorted(nums);
    
    // test smallest_greater and largest_less functions --------------->>
    test_smallest_greater(nums);
    test_largest_less(nums);

    // test permutation and combination functions --------------------->>
    // Repeat tests for r equal to 0 through the size of the container.
    for(int r = 0; r <= nums.size(); r++)
    {
        test_next_r_permutation(nums, r);
        test_next_r_permutation_comp(nums, r);
        test_prev_r_permutation(nums, r);
        test_prev_r_permutation_comp(nums, r);
        test_next_r_combination(nums, r);
        test_next_r_combination_comp(nums, r);
        test_prev_r_combination(nums, r);
        test_prev_r_combination_comp(nums, r);
	}	// for

    cout << "\nTest of rcombo.hpp is complete." << endl;

    return test::test_tools_errors ? EXIT_FAILURE : EXIT_SUCCESS;

}   // test_main

