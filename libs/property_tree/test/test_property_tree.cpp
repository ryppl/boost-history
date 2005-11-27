// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#include "test_utils.hpp"
#include <boost/any.hpp>
#include <list>
#include <cmath>

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

template<class Type>
struct MyExtractor
{
    inline bool operator()(const boost::any &data, 
                           Type &extracted,
                           const std::locale &loc) const
    {
        extracted = boost::any_cast<Type>(data);
        return true;    // Success
    }
};

template<class Type>
struct MyInserter
{
    inline bool operator()(boost::any &data, 
                           const Type &to_insert,
                           const std::locale &loc) const
    {
        data = to_insert;
        return true;    // Success
    }
};

template<class Ch>
struct MyTraits
{

    // Data type to be used by ptree
    typedef boost::any data_type;
    
    // Extractor to be used by ptree
    template<class Type>
    struct extractor: public MyExtractor<Type> { }; 

    // Inserter to be used by ptree
    template<class Type>
    struct inserter: public MyInserter<Type> { };

    // Key comparison function
    inline bool operator()(const std::basic_string<Ch> &key1, 
                           const std::basic_string<Ch> &key2) const
    {
        return key1 < key2;
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
        test_custom_traits(pt);
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
        test_custom_traits(pt);
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
        //test_custom_traits(pt);        // test identical to ptree version
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
        //test_custom_traits(pt);        // test identical to wptree version
        test_leaks(pt);                  // must be a final test
    }
#endif

    return 0;
}
