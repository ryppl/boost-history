// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

#define BOOST_PROPERTY_TREE_DEBUG           // Enable ptree debugging
#include <boost/property_tree/ptree.hpp>

// Do not deprecate insecure CRT calls
#if (defined(BOOST_MSVC) && (BOOST_MSVC >= 1400))
#   define _CRT_SECURE_NO_DEPRECATE
#endif

#include <cmath>
#include <boost/test/minimal.hpp>

template<class Ptree>
struct SortPred
{
    bool operator()(const typename Ptree::value_type &v1,
                    const typename Ptree::value_type &v2) const
    {
        return v1.first < v2.first;
    }
};

template<class Ptree>
struct SortPredRev
{
    bool operator()(const typename Ptree::value_type &v1,
                    const typename Ptree::value_type &v2) const
    {
        return !(v1.first < v2.first);
    }
};

// Include char tests, case sensitive
#define CHTYPE char
#define T(s) s
#define PTREE boost::property_tree::ptree
#   include "test_property_tree.hpp"
#undef CHTYPE
#undef T
#undef PTREE

// Include wchar_t tests, case sensitive
#ifndef BOOST_NO_CWCHAR
#   define CHTYPE wchar_t
#   define T(s) L ## s
#   define PTREE boost::property_tree::wptree
#       include "test_property_tree.hpp"
#   undef CHTYPE
#   undef T
#   undef PTREE
#endif

// Include char tests, case insensitive
#define CHTYPE char
#define T(s) s
#define PTREE boost::property_tree::iptree
#   include "test_property_tree.hpp"
#undef CHTYPE
#undef T
#undef PTREE

// Include wchar_t tests, case insensitive
#ifndef BOOST_NO_CWCHAR
#   define CHTYPE wchar_t
#   define T(s) L ## s
#   define PTREE boost::property_tree::wiptree
#       include "test_property_tree.hpp"
#   undef CHTYPE
#   undef T
#   undef PTREE
#endif

int test_main(int, char *[])
{
    
    using namespace boost::property_tree;
    
    // char tests, case sensitive
    {
        ptree *pt = 0;
        test_debug(pt);
        test_constructor_destructor_assignment(pt);
        test_insertion(pt);
        test_erasing(pt);
        test_clear(pt);
        test_pushpop(pt);
        test_container_iteration(pt);
        test_swap(pt);
        test_sort_reverse(pt);
        test_case(pt);
        test_comparison(pt);
        test_front_back(pt);
        test_get_put(pt);
        test_get_child_put_child(pt);
        test_path_separator(pt);
        test_precision(pt);
        test_locale(pt);
        test_leaks(pt);                  // must be a final test
    }

    // wchar_t tests, case sensitive
#ifndef BOOST_NO_CWCHAR
    {
        wptree *pt = 0;
        test_debug(pt);
        test_constructor_destructor_assignment(pt);
        test_insertion(pt);
        test_erasing(pt);
        test_clear(pt);
        test_pushpop(pt);
        test_container_iteration(pt);
        test_swap(pt);
        test_sort_reverse(pt);
        test_case(pt);
        test_comparison(pt);
        test_front_back(pt);
        test_get_put(pt);
        test_get_child_put_child(pt);
        test_path_separator(pt);
        test_precision(pt);
        test_locale(pt);
        test_leaks(pt);                  // must be a final test
    }
#endif

    // char tests, case insensitive
    {
        iptree *pt = 0;
        test_debug(pt);
        test_constructor_destructor_assignment(pt);
        test_insertion(pt);
        test_erasing(pt);
        test_clear(pt);
        test_pushpop(pt);
        test_container_iteration(pt);
        test_swap(pt);
        test_sort_reverse(pt);
        test_case(pt);
        test_comparison(pt);
        test_front_back(pt);
        test_get_put(pt);
        test_get_child_put_child(pt);
        test_path_separator(pt);
        test_precision(pt);
        test_locale(pt);
        test_leaks(pt);                  // must be a final test
    }

    // wchar_t tests, case insensitive
#ifndef BOOST_NO_CWCHAR
    {
        wiptree *pt = 0;
        test_debug(pt);
        test_constructor_destructor_assignment(pt);
        test_insertion(pt);
        test_erasing(pt);
        test_clear(pt);
        test_pushpop(pt);
        test_container_iteration(pt);
        test_swap(pt);
        test_sort_reverse(pt);
        test_case(pt);
        test_comparison(pt);
        test_front_back(pt);
        test_get_put(pt);
        test_get_child_put_child(pt);
        test_path_separator(pt);
        test_precision(pt);
        test_locale(pt);
        test_leaks(pt);                  // must be a final test
    }
#endif

    return 0;
}
