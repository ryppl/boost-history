//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/binary_tree.hpp>
#include <boost/tree/balanced_tree.hpp>
#include <boost/tree/searcher.hpp>

#include <boost/tree/balancers/unbalanced.hpp>

#include <boost/tree/comparators/string.hpp>

#include <boost/multi_index/identity.hpp>

#include <string>

#define BOOST_TEST_MODULE binary_tree test
//#define BOOST_TEST_DYN_LINK
#include <boost/test/included/unit_test.hpp>

//Words from Austern et al., p1290

//TODO: add real tests. (what is where?)
//does boost have timers? what does the austern et al one look like?
// TODO: get timings. that makes that no testcase anymore, right?
void test_normal_string_search_binary_tree()
{
    using namespace boost::tree;
    
    typedef searcher<false, balanced_tree<binary_tree<std::string>, balancers::unbalanced > > searcher_t;
    searcher_t my_tree;
    
    my_tree.insert("anthology");
    my_tree.insert("anagram");
    my_tree.insert("anodyne");
    my_tree.insert("anthrax");
    my_tree.insert("anteater");    
    
    //FIXME: const_iterator doesn't work properly yet
    searcher_t::iterator ci = my_tree.begin(); 
    BOOST_CHECK(*ci++ == "anagram");
    BOOST_CHECK(*ci++ == "anodyne");
    BOOST_CHECK(*ci++ == "anteater");
    BOOST_CHECK(*ci++ == "anthology");
    BOOST_CHECK(*ci++ == "anthrax");
    BOOST_CHECK(ci == my_tree.end());
}

void test_optimized_string_search_binary_tree()
{
    
    using namespace boost::tree;
    using boost::multi_index::identity;

    typedef searcher<false, balanced_tree<binary_tree<std::string>, balancers::unbalanced>, 
                     identity<std::string>,
                     container_lexicographical_compare<std::string, std::string>
                    > searcher_t;
    searcher_t my_tree;
    
    my_tree.insert("anthology");
    my_tree.insert("anagram");
    my_tree.insert("anodyne");
    my_tree.insert("anthrax");
    my_tree.insert("anteater");
    
    //FIXME: const_iterator doesn't work properly yet
    searcher_t::iterator ci = my_tree.begin(); 
    BOOST_CHECK(*ci++ == "anagram");
    BOOST_CHECK(*ci++ == "anodyne");
    BOOST_CHECK(*ci++ == "anteater");
    BOOST_CHECK(*ci++ == "anthology");
    BOOST_CHECK(*ci++ == "anthrax");
    BOOST_CHECK(ci == my_tree.end());
}


BOOST_AUTO_TEST_CASE( string_search_binary_tree_test )
{
    test_normal_string_search_binary_tree();
    test_optimized_string_search_binary_tree();
    return 0;
}
