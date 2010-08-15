//  Boost string_algo library finder.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.


//TODO REPLACE TYPENAME EVERYWHERE WITH BOOST_STRING_TYPENAME
//TODO maybe finder_t should have a get_internal_string() and get_internal_substring()
//todo maybe we should replace *substring* with *pattern* and *string* with *text*

#ifndef BOOST_STRING_FINDER_HPP
#define BOOST_STRING_FINDER_HPP

#include <boost/algorithm/string/config.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/category.hpp>

#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/detail/finder.hpp>
#include <boost/algorithm/string/compare.hpp>

#include <boost/call_traits.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/placeholders.hpp>

#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/concept_check.hpp>

#include <boost/algorithm/string/finder/default_search_algorithm.hpp>
#include <boost/algorithm/string/finder/finder.hpp>
#include <boost/algorithm/string/finder/simplified_finder.hpp>
#include <boost/algorithm/string/finder/generated_finders.hpp>
#include <boost/algorithm/string/finder/finder_generators.hpp>

//TODO modify this
/*! \file
    Defines Finder types and Finder generators. Finder object is a functor which is able to 
    find a substring matching a specific criteria in the input.
    Finders are used as a pluggable components for replace, find 
    and split facilities. This header contains generator functions 
    for finders provided in this library.
*/

//TODO: test if get_internal_string() and get_internal_substring() are fine


#endif  // BOOST_STRING_FINDER_HPP
