//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#include <boost/lambda/bind.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

#include "fake_binary_tree.hpp"

using namespace boost::tree;

BOOST_FIXTURE_TEST_SUITE(cursor_algorithms_test, fake_binary_tree_fixture<int>)

template <class Iter>
class mock_unary_functor {
private:
    Iter& m_iter, m_end;
    
public:
    mock_unary_functor(Iter& iter, Iter& end)
    : m_iter(iter), m_end(end) {}
    
    mock_unary_functor(mock_unary_functor<Iter> const& other)
    : m_iter(other.m_iter), m_end(other.m_end) {}
    
    void operator()(typename Iter::value_type::second_type const& val)
    {
        BOOST_CHECK(m_iter != m_end);
        if (m_iter == m_end)
            return;
        BOOST_CHECK_EQUAL(val, m_iter->second);
        ++m_iter;
    }
};

BOOST_AUTO_TEST_CASE_TEMPLATE( test_for_each_descending, Order, orders)
{
    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();
    mock_unary_functor<container_type::const_iterator> muc(ci, cie);
    boost::tree::for_each(
        Order(),
        fbt1.descending_root(), 
        muc
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_for_each_ascending, Order, orders)
{
    typedef std::vector< std::pair<std::size_t, int> > container_type;
    container_type po(11);
    generate_mock_cursor_data(Order(), po);
    container_type::const_iterator ci = po.begin();
    container_type::const_iterator cie = po.end();
    mock_unary_functor<container_type::const_iterator> muc(ci, cie);
    boost::tree::for_each(
        Order(),
        fbt1.ascending_root(), 
        muc
    );
}

BOOST_AUTO_TEST_SUITE_END()