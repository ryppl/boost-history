//  Boost string_algo library finder.hpp header file  ---------------------------//

//  Copyright Pavol Droba 2002-2006.
//  Copyright Stefan Mihaila 2010.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/ for updates, documentation, and revision history.


//TODO REPLACE TYPENAME EVERYWHERE WITH BOOST_STRING_TYPENAME
//todo maybe we should replace *substring* with *pattern* and *string* with *text*

#ifndef BOOST_STRING_FINDER_HPP
#define BOOST_STRING_FINDER_HPP

#include <boost/algorithm/string/config.hpp>

#include <boost/algorithm/string/finder/default_search_algorithm.hpp>
#include <boost/algorithm/string/finder/finder.hpp>
#include <boost/algorithm/string/finder/simplified_finder.hpp>
#include <boost/algorithm/string/finder/functor_finders.hpp>
#include <boost/algorithm/string/finder/finder_generators.hpp>

/*! \file
    Defines Finder types and Finder generators. A Finder is an object which is able to 
    find a substring matching a specific criteria in the input.
    Finders can be used directly, or as pluggable components for replace, find 
    and split facilities.
*/

#endif  // BOOST_STRING_FINDER_HPP
