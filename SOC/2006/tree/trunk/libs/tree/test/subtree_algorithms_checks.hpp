//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>
#include <boost/tree/binary_tree.hpp>

#include <boost/lambda/bind.hpp>

#include <list>
#include <algorithm>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

using namespace boost::tree;

namespace test {

template <class Order, class Cursor, class Container>
void test_for_each(Order, Cursor c, Container& cont)
{
    boost::tree::for_each(
        Order(),
        c, 
        boost::lambda::bind(&Container::push_back, &cont, boost::lambda::_1)
    );
    test_traversal(Order(), cont.begin(), cont.end());
}

template <class Order, class Cursor, class OutCursor, class Container>
void test_copy(Order, Cursor c, OutCursor& o, Container& cont)
{    
    boost::tree::copy(Order(), c, o);
    test_traversal(Order(), cont.begin(), cont.end());
}

template <class Order, class Cursor, class OutCursor, class Container>
void test_transform(Order, Cursor c, Cursor d, OutCursor& o, Container& cont)
{
    // First copy test_tree to test_tree2, by adding 1 to each element,
    // then copy test_tree2 to test_list, by subtracting 1 - so 
    // test_list should hold test_tree's original elements in ORDER.
    boost::tree::transform(Order(), c, d, std::bind2nd(std::plus<int>(),1));
    boost::tree::transform(Order(), d, o, std::bind2nd(std::minus<int>(),1));
    test_traversal(Order(), cont.begin(), cont.end());
}

template <class Order, class Cursor>
void algorithms(Order, Cursor c, Cursor d)
{
    std::list<int> test_list;
    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
    typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
    
    test_for_each(Order(), c, test_list);
    
    test_list.clear();
    test_copy(Order(), c, oc_test_list, test_list);
    
    test_list.clear();
    test_transform(Order(), c, d, oc_test_list, test_list);
}

template <class Order, class Cursor>
void compare_cursor_to_iterator_traversal(Order, Cursor cur) {    
    std::list<int> test_list;
    typedef std::back_insert_iterator< std::list<int> > back_insert_iter_list_int;
    typedef output_cursor_iterator_wrapper<back_insert_iter_list_int> oc_bi_lst_type;
    back_insert_iter_list_int it_test_list = std::back_inserter(test_list);
    oc_bi_lst_type oc_test_list = oc_bi_lst_type(it_test_list);
    
    boost::tree::copy(Order(), cur, oc_test_list);
    
    // Are the elements accessed in the correct order?
    BOOST_CHECK(std::equal( boost::tree::begin(Order(), cur),
                            boost::tree::end(Order(), cur),
                            test_list.begin()
                            ));

    // Does end() mark the right element? 
    BOOST_CHECK(std::distance(boost::tree::begin(Order(), cur),
                              boost::tree::end(Order(), cur)) == 
                std::distance(test_list.begin(), test_list.end()));

    // Reverse order.
    BOOST_CHECK(std::equal(    boost::tree::rbegin(Order(), cur),
                            boost::tree::rend(Order(), cur),
                            test_list.rbegin()
                            ));

    BOOST_CHECK(std::distance(boost::tree::rbegin(Order(), cur),
                              boost::tree::rend(Order(), cur)) == 
                std::distance(test_list.rbegin(), test_list.rend()));                    

}

} // namespace test
