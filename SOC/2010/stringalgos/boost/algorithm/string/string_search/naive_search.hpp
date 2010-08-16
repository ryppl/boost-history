//  Boost string_algo library naive_search.hpp header file  ---------------------------//

//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.

#ifndef BOOST_ALGORITHM_NAIVE_SEARCH_HPP
#define BOOST_ALGORITHM_NAIVE_SEARCH_HPP

#include <boost/range/iterator_range.hpp>
#include <boost/call_traits.hpp>
#include <boost/mpl/void.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <string>

#include <boost/algorithm/string/finder/detail/finder_typedefs.hpp>
#include <boost/algorithm/string/finder/detail/string_search_ranges.hpp>

/*!
    \file
    Implements the naive string search algorithm
*/

namespace boost { namespace algorithm {
    // todo Copyable

    //! An implementation of the naive string search algorithm
	struct naive_search
	{

#       ifndef BOOST_ALGORITHM_DOXYGEN

        template <class Range1CharT, class Range2CharT, class ComparatorT, class AllocatorT>
        class algorithm
		{
        private:
            typedef Range1CharT substring_char_type;
            typedef Range2CharT string_char_type;
            typedef ComparatorT comparator_type;
            typedef AllocatorT allocator_type;
        public:
            std::string get_algorithm_name () const { return "Naive search"; }

            algorithm (comparator_type const &comp, allocator_type const &alloc)
                : comp_(comp), alloc_(alloc) { }

            template <class Range1T, class Range2T>
            inline typename boost::iterator_range<typename boost::range_iterator<Range2T>::type>
                find(typename boost::algorithm::detail::string_search_ranges<Range1T, Range2T> const &ranges)
			{
                BOOST_ALGORITHM_DETAIL_COMMON_FINDER_TYPEDEFS(Range1T, Range2T);

                string_range_type const &str = ranges.str;
                substring_range_type const &substr = ranges.substr;
                string_iterator_type start = ranges.offset;

				for (;
					start != boost::end(str); ++start)
				{
					string_iterator_type str_iter(start);
					substring_iterator_type substr_iter;
					for (substr_iter = boost::begin(substr);
						substr_iter != boost::end(substr) && str_iter != boost::end(str);
                        ++substr_iter, ++str_iter)
					{
						if (!comp_(*str_iter, *substr_iter)) break;
					}
					if (substr_iter == boost::end(substr))
						return boost::iterator_range<string_iterator_type>(start, str_iter);
				}
				return boost::iterator_range<string_iterator_type>(
                    boost::end(str),boost::end(str));
			}

            //No precomputation to be done on the substring
            //It is guaranteed that each of these two functions will get called at least once before find()
            //is used.
            template <class T> inline void on_substring_change(T const&)
            {
            }
            
            //No precomputation to be done on the string
            template <class T> inline void on_string_change(T const&)
            {
            }
        private:
            comparator_type comp_;
            allocator_type alloc_;

		};
#       endif /* !defined(BOOST_ALGORITHM_DOXYGEN) */

	};
    //! Instances of this type can be passed to find functions to require them to
    //!     use the Boyer-Moore algorithm.
    struct naive_search_tag { typedef boost::algorithm::naive_search type; };
} }
	
namespace boost
{
    using boost::algorithm::naive_search;
}

#endif
