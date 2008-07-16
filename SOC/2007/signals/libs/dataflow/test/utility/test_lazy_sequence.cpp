// Copyright Stjepan Rajko 2008. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/fusion/container/lazy_sequence.hpp>

#include <boost/fusion/sequence/intrinsic.hpp>
#include <boost/fusion/iterator.hpp>

#include <boost/fusion/support/category_of.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

template<typename T>
struct singleton_functor
{
    typedef const T& result_type;
    
    singleton_functor(const T& t)
        : t(t) {}
    
    template<typename Index>
    result_type operator()(Index i)
    {   return t; }
    
    T t;
};

template<typename T>
struct index_plus_one
{
    typedef T result_type;
    
    template<typename Index>
    result_type operator()(Index i)
    {   return Index::value + 1; }
};

int one(int index)
{
    return 1;
}

int test_main(int, char* [])
{
    namespace fusion=boost::fusion;

    typedef fusion::lazy_sequence<singleton_functor<int>,3> ones_triple_type;
    ones_triple_type ones_triple(singleton_functor<int>(1));
    
    BOOST_CHECK_EQUAL(*fusion::begin(ones_triple),1);
    BOOST_CHECK_EQUAL(*fusion::next(fusion::begin(ones_triple)), 1);
    BOOST_CHECK_EQUAL(*fusion::prior(fusion::end(ones_triple)), 1);
    BOOST_CHECK_EQUAL(fusion::distance(fusion::begin(ones_triple), fusion::end(ones_triple)), 3);
    BOOST_CHECK_EQUAL(fusion::size(ones_triple), 3);
    BOOST_CHECK((boost::is_same<const int &, fusion::result_of::value_at_c<ones_triple_type, 0>::type >::value));
    BOOST_CHECK((boost::is_same<const int &, fusion::result_of::value_at_c<ones_triple_type, 1>::type >::value));
    BOOST_CHECK((boost::is_same<const int &, fusion::result_of::value_at_c<ones_triple_type, 2>::type >::value));
    
    typedef fusion::lazy_sequence<index_plus_one<size_t>,4> index_shift_type;
    index_shift_type index_shift;
    
    BOOST_CHECK_EQUAL(fusion::at_c<0>(index_shift), 1u);
    BOOST_CHECK_EQUAL(fusion::at_c<1>(index_shift), 2u);
    BOOST_CHECK_EQUAL(fusion::at_c<2>(index_shift), 3u);
    BOOST_CHECK_EQUAL(fusion::at_c<3>(index_shift), 4u);

    
    BOOST_CHECK_EQUAL(fusion::at_c<0>(fusion::make_lazy_sequence(one, boost::mpl::int_<1>())), 1);
    
    return 0;
} // int test_main(int, char* [])




