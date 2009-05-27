//  Copyright (c) 2006-2009, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/algorithm.hpp>

#include <boost/lambda/bind.hpp>

#include <boost/tree/forest.hpp>

#define BOOST_TEST_MODULE cursor_algorithm test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>


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
    
    void operator()(typename Iter::value_type::value_type const& val)
    {
        BOOST_CHECK(m_iter != m_end);
        if (m_iter == m_end)
            return;
        BOOST_CHECK_EQUAL(val, m_iter->val);
        ++m_iter;
    }
};

BOOST_AUTO_TEST_CASE_TEMPLATE( test_for_each_descending, Order, orders)
{
    test_data_set mpo;
    mock_cursor_data(mpo);

    typedef typename test_data_set::index<Order>::type container_type;
    const container_type& order_index = mpo.get<Order>();

    typename container_type::const_iterator ci = order_index.begin();
    typename container_type::const_iterator cie = order_index.end();

    mock_unary_functor<typename container_type::const_iterator> muc(ci, cie);
    boost::tree::for_each(
        Order(),
        fbt1.descending_root(), 
        muc
    );
}

BOOST_AUTO_TEST_CASE_TEMPLATE( test_for_each_ascending, Order, orders)
{
    test_data_set mpo;
    mock_cursor_data(mpo);

    typedef typename test_data_set::index<Order>::type container_type;
    const container_type& order_index = mpo.get<Order>();

    typename container_type::const_iterator ci = order_index.begin();
    typename container_type::const_iterator cie = order_index.end();

    fake_binary_tree<int, boost::tree::ascending_vertical_traversal_tag> fabt1(fbt1);

    mock_unary_functor<typename container_type::const_iterator> muc(ci, cie);
    boost::tree::for_each(
        Order(),
        fabt1.root(), 
        muc
    );
}

BOOST_AUTO_TEST_CASE( test_forest_for_each_descending_preorder)
{
    test_data_set mpo;
    mock_cursor_data(mpo);

    typedef test_data_set::index<preorder>::type container_type;
    const container_type& order_index = mpo.get<preorder>();

    container_type::const_iterator ci = order_index.begin();
    container_type::const_iterator cie = order_index.end();
    ++ci;        // 3
    cie = ci;
    ++++++++cie; // 7

    boost::tree::forest<int, fake_binary_tree<int> > ft0(fbt1);

    mock_unary_functor<container_type::const_iterator> muc(ci, cie);
    //FIXME:
//    boost::tree::for_each(
//        preorder(),
//        ft0.begin().begin(), 
//        ft0.begin().end(),
//        muc
//    );
}

BOOST_AUTO_TEST_CASE( test_forest_for_each_ascending_preorder)
{
    test_data_set mpo;
    mock_cursor_data(mpo);

    typedef test_data_set::index<preorder>::type container_type;
    const container_type& order_index = mpo.get<preorder>();

    container_type::const_iterator ci = order_index.begin();
    container_type::const_iterator cie = order_index.end();
    ++ci;        // 3
    cie = ci;
    ++++++++cie; // 7

    fake_root_tracking_binary_tree<int> fabt1(fbt1);
    boost::tree::forest<int, fake_root_tracking_binary_tree<int> > ft0(fabt1);

    mock_unary_functor<container_type::const_iterator> muc(ci, cie);
//    boost::tree::for_each(
//        preorder(),
//        ft0.begin().begin().begin(), //boost::tree::detail::forest_cursor< fake_ascending_binary_cursor<int> >(fabt1.root().begin().begin()), //Shouldn't that just be one begin()? 
//        ft0.begin().end(), //boost::tree::detail::forest_cursor< fake_ascending_binary_cursor<int> >(fabt1.root().begin().end().end().begin()),
//        muc
//    );

    //boost::tree::detail::forest_cursor< fake_root_tracking_binary_cursor<int> > dfc0(ft0.begin().begin().end());
    
    // Also try ft0.begin and ft0.end consistency!
    
    fake_root_tracking_binary_cursor<int> dfc(ft0.begin().begin().begin().base()); //(fabt1.root().begin().begin());
    fake_root_tracking_binary_cursor<int> dfce(ft0.begin().end().base());
    --dfce;
    BOOST_CHECK_EQUAL(*dfce, 7);
    
    BOOST_CHECK_EQUAL(*dfc, ci->val);
    successor(preorder(), dfc);
    BOOST_CHECK_EQUAL(*dfc, (++ci)->val);
    successor(preorder(), dfc);
    BOOST_CHECK_EQUAL(*dfc, 6);
    successor(preorder(), dfc);
    BOOST_CHECK_EQUAL(*dfc, 4);
    successor(preorder(), dfc);
    BOOST_CHECK_EQUAL(*dfc, 7);
    BOOST_CHECK(dfc == dfce);
}

BOOST_AUTO_TEST_SUITE_END()