// test_combinatorial.cpp ------------------------------------------------------//
//   Test program for r-permutation and r-combination generic functions
//   in rcombo.h.

// Copyright © Philip F. Garofalo 2008. All rights reserved.
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

// 1. Combination sequences are sorted.
// 2. Each sequence produced in the series is in lexicographical
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
#include <stdexcept>
#include <cstring>      // strncmp

//#define BOOST_INCLUDE_MAIN
#include <boost/test/minimal.hpp>
#include <boost/lexical_cast.hpp>

#include "combinatorial.hpp"
using namespace boost;

// macros ---------------------------------------->>

#define DIM(a)    (sizeof(a)/sizeof(a)[0])


// Test data ---------------------------------------->>

char theNumerals[] = { '6', '3', '2', '5', '4', '1' };
  
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
   struct detail::iterator_traits<char*> {
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
}   // factorial

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
}   // factorial


// format_sequence -------------------------------------------------//

template<class InputIterator>
void format_sequence(InputIterator first, InputIterator middle,
                 InputIterator last, std::ostream& os = std::cout)
{
    //typedef typename iterator_traits<InputIterator>::value_type T; // not compiling
    std::copy(first, middle, std::ostream_iterator<char>(os, "  "));  // <char> was <T>
    os << "|  ";
    std::copy(middle, last, std::ostream_iterator<char>(os, "  "));
}   // format_sequence


// invalid_combination_msg ----------------------------------------------//

template<class T>
std::string invalid_sequence_msg(const std::string& seq_type, unsigned n,
                                 const T& seq, int r)
{
    std::ostringstream msg;
    msg << "Invalid " << seq_type << " #" << n << " of P(" << seq.size()
    	<< ", " << r << "): ";
    format_sequence(seq.begin(), seq.begin() + r, seq.end(), msg);
    return msg.str();
}	// invalid_sequence_msg

// invalid_combination_msg ----------------------------------------------//

template<class T>
inline std::string invalid_combination_msg(unsigned n, const T& seq, int r)
{
	return invalid_sequence_msg("combination", n, seq, r);
}   // invalid_combination_msg

// invalid_permutation_msg ----------------------------------------------//

template<class T>
inline std::string invalid_permutation_msg(unsigned n, const T& seq, int r)
{
	return invalid_sequence_msg("permutation", n, seq, r);
}


// valid_next_partial_permutation --------------------------------------------------//

template<class ForwardIterator>
bool valid_next_partial_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is greater than previous
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle);
	
	prev = std::vector<T>(first, middle);

    return retval;
}   // valid_next_partial_permutation

// valid_next_partial_permutation ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_next_partial_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is greater than previous
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle, comp);
	
	prev = std::vector<T>(first, middle);

    return retval;
}   // valid_next_partial_permutation

// valid_prev_partial_permutation ---------------------------------------------//

template<class ForwardIterator>
bool valid_prev_partial_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is greater than previous
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end());
	
	prev = std::vector<T>(first, middle);

    return retval;
}   // valid_prev_partial_permutation

// valid_prev_partial_permutation ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_prev_partial_permutation(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is greater than previous
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end(), comp);
	
	prev = std::vector<T>(first, middle);

    return retval;
}   // valid_prev_partial_permutation

// valid_partial_combination --------------------------------------------------//

template<class ForwardIterator>
bool valid_next_partial_combination(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
    if (!is_sorted(first, middle))
    	return false;
    	
   	// and greater than the last one
 	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is greater than last
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle);
	
	prev = std::vector<T>(first, middle);

    return retval;

}   // valid_next_partial_combination

// valid_prev_partial_combination ---------------------------------------------//

template<class ForwardIterator>
bool valid_prev_partial_combination(ForwardIterator first,
                              ForwardIterator middle,
                              bool fresh_start)
{
    if (!is_sorted(first, middle))
    	return false;
    	
   	// and less than the last one
 	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is less than last
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end());
	
	prev = std::vector<T>(first, middle);

    return retval;

}   // valid_prev_partial_combination

// valid_next_partial_combination ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_next_partial_combination(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
    if (!is_sorted(first, middle, comp))
    	return false;
    	
	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is greater than last
		retval = lexicographical_compare(prev.begin(), prev.end(), first, middle, comp);
	
	prev = std::vector<T>(first, middle);

    return retval;
}   // valid_next_partial_combination

// valid_prev_partial_combination ---------------------------------------------//

template<class ForwardIterator, class Compare>
bool valid_prev_partial_combination(ForwardIterator first,
                              ForwardIterator middle,
                              Compare comp,
                              bool fresh_start)
{
    if (!is_sorted(first, middle, comp))
    	return false;
    	
	typedef typename detail::iterator_traits<ForwardIterator>::value_type T;
	static std::vector<T> prev;
	bool retval = true;
	if (!fresh_start && first != middle)	// test to ensure this sequence is less than last
		retval = lexicographical_compare(first, middle, prev.begin(), prev.end(), comp);
	
	prev = std::vector<T>(first, middle);

    return retval;
}   // valid_prev_partial_combination

// test_factorial -------------------------------------------------------//

void test_factorial()
{
    BOOST_REQUIRE(factorial(0) == 1);
    BOOST_REQUIRE(factorial(1) == 1);
    BOOST_REQUIRE(factorial(5) == 120);
    BOOST_REQUIRE(factorial(6) == 720);
    BOOST_REQUIRE(factorial(11) == 39916800UL);

    BOOST_REQUIRE(factorial(0, 0) == 1);
    BOOST_REQUIRE(factorial(1, 1) == 1);
    BOOST_REQUIRE(factorial(2, 1) == 2);
    BOOST_REQUIRE(factorial(2, 2) == 2);
    BOOST_REQUIRE(factorial(5, 3) == 60);
    BOOST_REQUIRE(factorial(6, 4) == 360);
    BOOST_REQUIRE(factorial(7, 0) == 1);
    BOOST_REQUIRE(factorial(7, 2) == 42);
    BOOST_REQUIRE(factorial(7, 7) == 5040);
}   // test_factorial


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

// test_next_partial_permutation ----------------------------------------------//

template<class T>
void test_next_partial_permutation(T& seq, int r)
{
	std::partial_sort(seq.begin(), seq.begin() + r, seq.end());
    T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_next_partial_permutation(seq.begin(), seq.begin() + r, count == 1))
            {
                BOOST_ERROR(invalid_permutation_msg(count, seq, r).c_str());
            }
        } while(next_partial_permutation(seq.begin(), seq.begin() + r, seq.end()));
    }
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long perm_cnt = factorial(seq.size(), r);
	if (count != perm_cnt)
	{
	    std::ostringstream msg;
	    msg << "next_partial_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}
	                       
	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("next_partial_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_next_partial_permutation

// test_next_partial_permutation_comp -----------------------------------------//

template<class T>
void test_next_partial_permutation_comp(T& seq, int r)
{
    typedef typename T::value_type U;
	std::partial_sort(seq.begin(), seq.begin() + r, seq.end(), std::greater<U>());
	T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_next_partial_permutation(seq.begin(), seq.begin() + r,
                std::greater<U>(), count == 1))
            {
                BOOST_ERROR(invalid_permutation_msg(count, seq, r).c_str());
            }
        } while(next_partial_permutation(seq.begin(), seq.begin() + r, seq.end(),
            std::greater<U>()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long perm_cnt = factorial(seq.size(), r);
	if (count != perm_cnt)
	{
	    std::ostringstream msg;
	    msg << "next_partial_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("next_partial_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_next_partial_permutation_comp

// test_prev_partial_permutation ----------------------------------------------//

template<class T>
void test_prev_partial_permutation(T& seq, int r)
{
    typedef typename T::value_type U;
	std::partial_sort(seq.begin(), seq.begin() + r, seq.end(), std::greater<U>());
	T checkseq = seq;
    unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_prev_partial_permutation(seq.begin(), seq.begin() + r, count == 1))
            {
                BOOST_ERROR(invalid_permutation_msg(count, seq, r).c_str());
            }
        } while(prev_partial_permutation(seq.begin(), seq.begin() + r, seq.end()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long perm_cnt = factorial(seq.size(), r);
	if (count != perm_cnt)
	{
	    std::ostringstream msg;
	    msg << "prev_partial_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("prev_partial_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_prev_partial_permutation

// test_prev_partial_permutation_comp -------------------------------------------//

template<class T>
void test_prev_partial_permutation_comp(T& seq, int r)
{
    typedef typename T::value_type U;
	std::partial_sort(seq.begin(), seq.begin() + r, seq.end());
	T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_prev_partial_permutation(seq.begin(), seq.begin() + r,
                std::greater<U>(), count == 1))
            {
                BOOST_ERROR(invalid_permutation_msg(count, seq, r).c_str());
            }
        } while(prev_partial_permutation(seq.begin(), seq.begin() + r, seq.end(),
            std::greater<U>()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long perm_cnt = factorial(seq.size(), r);
	if (count != perm_cnt)
	{
	    std::ostringstream msg;
	    msg << "prev_partial_permutation generated "<< count
	        << " permutations when there should have been "
	        << perm_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("prev_partial_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_prev_partial_permutation_comp

// test_next_partial_combination ----------------------------------------------//

template<class T>
void test_next_partial_combination(T& seq, int r)
{
	std::partial_sort(seq.begin(), seq.begin() + r, seq.end());
	T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_next_partial_combination(seq.begin(), seq.begin() + r, count == 1))
                BOOST_ERROR(invalid_combination_msg(count, seq, r).c_str());
        } while(next_partial_combination(seq.begin(), seq.begin() + r, seq.end()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch
    catch(const combinatorial_sequence_disorder& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch



    const unsigned long comb_cnt = factorial(seq.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    std::ostringstream msg;
	    msg << "next_partial_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("next_partial_permutation didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_next_partial_combination

// test_next_partial_combination_comp -----------------------------------------//

template<class T>
void test_next_partial_combination_comp(T& seq, int r)
{
    typedef typename T::value_type U;
	std::partial_sort(seq.begin(), seq.begin() + r, seq.end(), std::greater<U>());
	T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_next_partial_combination(seq.begin(), seq.begin() + r,
                std::greater<U>(), count == 1))
                BOOST_ERROR(invalid_combination_msg(count, seq, r).c_str());
        } while(next_partial_combination(seq.begin(), seq.begin() + r, seq.end(),
            std::greater<U>()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch
    catch(const combinatorial_sequence_disorder& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long comb_cnt = factorial(seq.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    std::ostringstream msg;
	    msg << "next_partial_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("next_partial_combutation didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_next_partial_combination_comp

// test_prev_partial_combination ----------------------------------------------//

template<class T>
void test_prev_partial_combination(T& seq, int r)
{
	std::sort(seq.begin(), seq.end());
	std::rotate(seq.begin(), seq.end() - r, seq.end());
	T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_prev_partial_combination(seq.begin(), seq.begin() + r, count == 1))
                BOOST_ERROR(invalid_combination_msg(count, seq, r).c_str());
        } while(prev_partial_combination(seq.begin(), seq.begin() + r, seq.end()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch
    catch(const combinatorial_sequence_disorder& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long comb_cnt = factorial(seq.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    std::ostringstream msg;
	    msg << "prev_partial_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("prev_partial_combination didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_prev_partial_combination

// test_prev_partial_combination_comp -----------------------------------------//

template<class T>
void test_prev_partial_combination_comp(T& seq, int r)
{
    typedef typename T::value_type U;
	std::sort(seq.begin(), seq.end(), std::greater<U>());
	std::rotate(seq.begin(), seq.end() - r, seq.end());
	T checkseq = seq;
	unsigned count = 0;
    try {
        do {
            ++count;
            if (!valid_prev_partial_combination(seq.begin(), seq.begin() + r,
                std::greater<U>(), count == 1))
                BOOST_ERROR(invalid_combination_msg(count, seq, r).c_str());
        } while(prev_partial_combination(seq.begin(), seq.begin() + r, seq.end(),
            std::greater<U>()));
    }   // try
    catch(const combinatorial_range_error& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch
    catch(const combinatorial_sequence_disorder& oops)
    {
        BOOST_ERROR(oops.what());
        return;
    }   // catch

    const unsigned long comb_cnt = factorial(seq.size(), r) / factorial(r);
	if (count != comb_cnt)
	{
	    std::ostringstream msg;
	    msg << "prev_partial_combination generated "<< count
	        << " combinations when there should have been "
	        << comb_cnt << '\n';
	    BOOST_ERROR(msg.str().c_str());
	}

	if (!equal(seq.begin(), seq.begin() + r, checkseq.begin()))
	{
	    BOOST_ERROR("prev_partial_combination didn't restore sequence to initial value at end of series.");
	    format_sequence(seq.begin(), seq.begin() + r, seq.end());
	    std::cout << '\n';
	    format_sequence(checkseq.begin(), checkseq.begin() + r, checkseq.end());
	    std::cout << '\n';
	}
}   // test_prev_partial_combination_comp


// test_main ------------------------------------------------------------//

int test_main(int, char*[])
{
    // test factorial function ---------------------------------------->>
    test_factorial();

    std::vector<char> nums(theNumerals, theNumerals + DIM(theNumerals));

    // test permutation and combination functions --------------------->>
    // Repeat for various container sizes
    for (int s = 0; s < DIM(theNumerals); s++)
    {
        std::vector<char> nums2(theNumerals, theNumerals + s);
        // repeat tests for r equal to 1 through the size of the container.
        for(int r = 1; r <= nums2.size(); r++)
        {
            test_next_partial_permutation(nums2, r);
            test_next_partial_permutation_comp(nums2, r);

            test_prev_partial_permutation(nums2, r);
            test_prev_partial_permutation_comp(nums2, r);

            test_next_partial_combination(nums2, r);
            test_next_partial_combination_comp(nums2, r);

            test_prev_partial_combination(nums2, r);
            test_prev_partial_combination_comp(nums2, r);
	    }	// for
    }   // for

    std::cout << "\nTest of combinatorial.hpp is complete." << std::endl;

    return boost::minimal_test::errors_counter() > 0 ? EXIT_FAILURE : EXIT_SUCCESS;

}   // test_main

