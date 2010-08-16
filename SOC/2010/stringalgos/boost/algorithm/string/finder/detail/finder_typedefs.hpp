//  Boost string_algo library finder_typedefs.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_FINDER_TYPEDEFS_HPP
#define BOOST_ALGORITHM_FINDER_TYPEDEFS_HPP

#include <boost/range/iterator_range.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/category.hpp>

#include <boost/iterator/iterator_traits.hpp>

#define BOOST_ALGORITHM_DETAIL_DEFAULT_ALLOCATOR_TYPE std::allocator<std::size_t>
#define BOOST_ALGORITHM_DETAIL_DEFAULT_COMPARATOR_TYPE boost::algorithm::is_equal

#define BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T) \
    typedef Range1T substring_type; /*!< The type of the substring */ \
    typedef Range2T string_type; /*!< The type of the string */ \
    typedef typename boost::range_const_iterator<substring_type>::type \
    substring_iterator_type; /*!< The type of the substring's iterator */ \
    typedef typename boost::range_iterator<string_type>::type \
    string_iterator_type; /*!< The type of the string's iterator */ \
    typedef typename boost::iterator_value<substring_iterator_type>::type \
    substring_char_type; /*!< The character type of the substring */ \
    typedef typename boost::iterator_value<string_iterator_type>::type \
    string_char_type; /*!< The character type of the string */ \
    typedef typename boost::iterator_range<substring_iterator_type> \
    substring_range_type; /*!< The range type of the substring (pattern) */ \
    typedef typename boost::iterator_range<string_iterator_type> \
    string_range_type; /*!< The range type of the text */ \
    typedef typename boost::iterator_difference<string_iterator_type>::type \
    string_difference_type /*!< A type capable of holding the difference between two iterators of the text */

#define BOOST_ALGORITHM_DETAIL_UNCOMMON_FINDER_TYPEDEFS(Range1T, Range2T) \
    typedef typename boost::range_category<substring_type>::type substring_iterator_category; \
    typedef typename boost::range_category<string_type>::type string_iterator_category; \
    typedef typename boost::reverse_iterator<substring_iterator_type> substring_reverse_iterator_type; \
    typedef typename boost::reverse_iterator<string_iterator_type> string_reverse_iterator_type; \
    typedef typename boost::iterator_range<substring_reverse_iterator_type> substring_reverse_range_type; \
    typedef typename boost::iterator_range<string_reverse_iterator_type> string_reverse_range_type

/*
#define BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS(Range1T, Range2T) \
        BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T); \
    protected: \
        BOOST_ALGORITHM_DETAIL_UNCOMMON_FINDER_TYPEDEFS(Range1T, Range2T);
*/

#define BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS2(ComparatorT, AllocatorT) \
    typedef ComparatorT comparator_type; /*!< The type of the comparator */ \
    typedef AllocatorT allocator_type /*!< The type of the allocator */


/*
namespace boost { namespace algorithm { namespace detail {

    template <class Range1T, class Range2T, class ComparatorT, class AllocatorT>
    struct finder_typedefs
    {
        BOOST_ALGORITHM_DETAIL_FINDER_TYPEDEFS(Range1T, Range2T);
        BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS2(ComparatorT, AllocatorT);
    };

} } }
*/

#endif // BOOST_ALGORITHM_FINDER_TYPEDEFS_HPP
