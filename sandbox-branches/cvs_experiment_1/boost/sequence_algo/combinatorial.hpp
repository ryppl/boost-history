// combinatorial.h header file - r-permutation and r-combination algorithms ---//

// Copyright © Philip F. Garofalo 2002. All rights reserved.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.

// See http://www.boost.org for updates, documentation, and revision
// history.

// Jun 20 2002  Removed TABs and put in std:: when needed [Herve Bronnimann]
//              Also replaced all iter_swap(i,j) by swap(*i,*j)
//              
//              Heck, while I'm at it, I corrected is_sorted, which was
//              using --last (not part of ForwardIterator)
//              I hope I gave an even better implementation that Jeremy
//              and the SGI STL :)                        [Herve Bronnimann]

// Jun ?? 2002  Renamed file to combinatorial.hpp from rcombo.hpp. I also
//              renamed the function smallest_greater to min_element_greater than
//              and largest_less to max_element_less_than.

// Jun 27 2002  All functions now throw std::out_of_bounds if r (the iterator
//              that points into the middle of the sequence) is not within
//              (first, last]. The combination functions throw std::invalid_argument
//              if the selection [first, r) is not in sorted order. [Phil Garofalo]

// Jul 01 2002  Replaced about two-thirds of the calls to sort with partial_sort.
//              This should result in a more efficient program. Also introduced
//              two error objects combinatorial_range_error and
//              combinatorial_sequence_disorder. [Phil Garofalo]
 
// Jul 02 2002  Added reverse_args adapter, removed references to greater<T>() because
//              requirements stipulate that only < need be supported by the
//              containter elements. Replaced calls to min_element_greater_than
//              and max_element_less_than with min_element_if and max_element_if
//              respectively. [Phil Garofalo]
 

#ifndef BOOST_COMBINATORIAL_HPP
#define BOOST_COMBINATORIAL_HPP

#include <algorithm>            // std::sort(), std::rotate(), etc.
#include <functional>           // less<>(), binary_function
#include <stdexcept>            // out_of_range, invalid_argument
#include <boost/utility.hpp>    // next()
#include <algorithm.hpp>        // is_sorted()
#include <minmax.hpp>           // min/max_element_if()

namespace boost {

    // Binary predicate argument reverser
    
    #define BINFUNC std::binary_function<Predicate::first_argument_type, Predicate::second_argument_type, bool>

    template <class Predicate>
    class binary_reverse : public BINFUNC
    {
    protected:
        const Predicate pred;
    public:
        binary_reverse(const Predicate& x) : pred(x) {}
        BINFUNC::result_type operator()(const BINFUNC::first_argument_type& x,
            const BINFUNC::second_argument_type& y) const
        {
            return pred(y, x);
        }
    };  // binary_reverse class

    template <class Predicate>
        inline binary_reverse<Predicate>
        reverse_args(const Predicate& pred)
    {
        return binary_reverse<Predicate>(pred);
    }
    
    // Two exception objects

    // combinatorial_range_error ------------------------------------------//

    struct combinatorial_range_error : public std::out_of_range
    {
        combinatorial_range_error(const string& func) :
            std::out_of_range(func +  ": r is not in range (first, last]")
            { }
    };

    // combinatorial_sequence_disorder ------------------------------------//

    struct combinatorial_sequence_disorder : public std::invalid_argument
    {
        combinatorial_sequence_disorder(const string& func) :
            std::invalid_argument(func + ": [first, r) is not in order")
            { }
    };
    
    // next_r_permutation -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the next
    // r-permutation of elements in lexicographical (or dictionary) order.
    // When calling the function for the first time, the elements in
    // [first, last) should be in ascending lexicographical order to start
    // the permutation sequence at its beginning. Typically, when the function
    // is called it arranges the next r-permutation in [first, r) and returns
    // true. When the last permutation in lexicographical order is passed in,
    // the function std::sorts the entire range, [first, last) into ascending
    // order, restarting the sequence, and returns false.
    
    template<class RandomAccessIterator>
        bool
        next_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;

        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("next_r_permutation");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after index i.
            RandomAccessIterator k =
                min_element_if(i + 1, last, bind1st(less<T>(), *i));

            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::partial_sort(first, r, last);
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
            else
            {
                std::swap(*i, *k);
                std::partial_sort(i + 1, r, last);    // O(n lg n), heapsort
                return true;
            }    // else
        }    // while
    }    // next_r_permutation
    
    // next_r_permutation -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied binary
    // comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        next_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;
     
        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("next_r_permutation");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after index i.
            RandomAccessIterator k =
                min_element_if(i + 1, last, comp, bind2nd(reverse_args(comp), *i));

            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::partial_sort(first, r, last, comp);
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
            else
            {
                std::swap(*i,* k);
                std::partial_sort(i + 1, r, last, comp);    // O(n lg n), heapsort
                return true;
            }    // else
        }    // while
    }    // next_r_permutation
    
    // prev_r_permutation -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the previous
    // r-permutation of elements in lexicographical order. When calling
    // the function for the first time, the elements in [first, last) should
    // be in descending lexicographical order to start a new series at the
    // end. Typically, when the function is called it arranges the previous
    // r-permutation in [first, r) and returns true. When the first permutation
    // in lexicographical order is passed in, the function std::sorts the entire
    // range, [first, last) into descending order, restarting the sequence
    // at the end, and returns false.
    
    template<class RandomAccessIterator>
        bool
        prev_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;

        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("prev_r_permutation");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find the largest element less than *i after index i.
            RandomAccessIterator k =
                max_element_if(i + 1, last, bind2nd(less<T>(), *i));
            
            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::partial_sort(first, r, last, reverse_args(less<T>()));
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
            else
            {
                std::swap(*i,* k);
                std::partial_sort(i+1, r, last, reverse_args(less<T>()));    // O(n lg n), heapsort
                return true;
            }    // else
        }    // while
    }    // prev_r_permutation
    
    // prev_r_permutation -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied
    // binary comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        prev_r_permutation(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;

        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("prev_r_permutation");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find the largest element less than *i after index i.
            RandomAccessIterator k =
                max_element_if(i + 1, last, comp, bind2nd(comp, *i));
            
            if (k == last)            // Didn't find it.
                if (i == first)
                {
                    std::partial_sort(first, r, last, reverse_args(comp));
                    return false;    // we're done--end of permutations
                }
                else
                    --i;
            else
            {
                std::swap(*i,* k);
                std::partial_sort(i + 1, r, last, reverse_args(comp));    // O(n lg n), heapsort 
                return true;
            }    // else
        }    // while
    }    // prev_r_permutation
    
    
    // next_r_combination -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the next
    // r-combination of elements in lexicographical order. The elements
    // in [first, r) must be in ascending lexicographical order.
    // When the function is called and a next combination exists,
    // it arranges the next r-combination in [first, r) and returns true. 
    // If the next combination does not exist, the function std::sorts the
    // entire range, [first, last) into ascending order, restarting the
    // sequence, and returns false.
    
    template<class RandomAccessIterator>
        bool
        next_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;

        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("next_r_combination");
        if (!is_sorted(first, r))
            throw combinatorial_sequence_disorder("next_r_combination");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after r - 1.
            RandomAccessIterator j =
                min_element_if(r, last, bind1st(less<T>(), *i));
            if (j == last)
                if (i == first)
                {
                    std::partial_sort(first, r, last);
                    return false;
                }
                else
                    --i;
            else
            {
                std::swap(*i,* j);
                for(++i; i < r; i++)
                {
                    // find smallest element greater than *(i - 1) after r - 1.
                    j = min_element_if(r, last, bind1st(less<T>(), *(i - 1)));
                    if (j != last)
                        std::swap(*i,* j);
                }    // for
                return true;
            }    // else
        }    // while
    }    // next_r_combination
    
    // next_r_combination -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied
    // binary comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        next_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        typedef typename iterator_traits<RandomAccessIterator>::value_type T;

        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("next_r_combination");
        if (!is_sorted(first, r, comp))
            throw combinatorial_sequence_disorder("next_r_combination");
        
        RandomAccessIterator i = r - 1;
        while(true)
        {
            // find smallest element greater than *i after r - 1.
            RandomAccessIterator j =
                min_element_if(r, last, comp, bind2nd(reverse_args(comp), *i));
            if (j == last)
                if (i == first)
                {
                    std::partial_sort(first, r, last, comp);
                    return false;
                }
                else
                    --i;
            else
            {
                std::swap(*i, *j);
                for(++i; i < r; ++i)
                {
                    // find smallest element greater than *(i - 1) after r - 1.
                    j = min_element_if(r, last, comp,
                        bind2nd(reverse_args(comp), *(i - 1)));
                    if (j != last)
                        std::swap(*i, *j);
                }    // for
                return true;
            }    // else
        }    // while
    }    // next_r_combination
    
    
    // prev_r_combination -----------------------------------------------//
    
    // Arranges the elements in [first, r), from the larger range [first,
    // last) where first < r <= last, such that they represent the
    // previous r-combination of elements in lexicographical order.
    // The elements in [first, r) must be in ascending lexicographical
    // order. When the function is called and a prior combination exists,
    // it arranges the previous r-combination in [first, r) and returns 
    // true. If the prior combination does not exist, the function 
    // arranges the sequence [first, last) into the last r-combination,
    // thus restarting the sequence at the end, and returns false.
    
    template<class RandomAccessIterator>
        bool
        prev_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last)
    {
        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("prev_r_combination");
        if (!is_sorted(first, r))
            throw combinatorial_sequence_disorder("prev_r_combination");

        std::sort(r, last);
        for (RandomAccessIterator i = last - 1; i >= r; --i)
            for (RandomAccessIterator j = first; j < r; ++j)
                if (*i < *j)
                {
                    std::swap(*j, *i);
                    std::sort(++j, last);    // O(n lg n)
                    std::rotate(j, j + (last - r), last); // 2*[n/2]+[m/2]+[(n-m)/2] exchanges
                    return true;
                }    // if
        std::rotate(first, first + (last - r), last);
        return false;
    }    // prev_r_combination
    
    // prev_r_combination -----------------------------------------------//
    
    // Identical to the above, except that it uses a user-supplied
    // binary comparison operator.
    
    template<class RandomAccessIterator, class Compare>
        bool
        prev_r_combination(RandomAccessIterator first, RandomAccessIterator r,
            RandomAccessIterator last, Compare comp)
    {
        if (last - first < 2)
            return false;
        if (!(first < r && r <= last))
            throw combinatorial_range_error("prev_r_combination");
        if (!is_sorted(first, r, comp))
            throw combinatorial_sequence_disorder("prev_r_combination");

        std::sort(r, last, comp);
        for (RandomAccessIterator i = last - 1; i >= r; i--)
            for (RandomAccessIterator j = first; j < r; j++)
                if (comp(*i, *j))
                {
                    std::swap(*j, *i);
                    std::sort(++j, last, comp);    // O(n lg n)
                    std::rotate(j, last - (r - j), last); // 2*[n/2]+[m/2]+[(n-m)/2] exchanges
                    return true;
                }    // if
        std::rotate(first, first + (last - r), last);
        return false;
    }    // prev_r_combination
    
}    // namespace boost

#endif    // BOOST_COMBINATORIAL_HPP
