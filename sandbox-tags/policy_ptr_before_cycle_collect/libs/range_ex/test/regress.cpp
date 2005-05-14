//  (C) Copyright Eric Niebler 2004.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
 Revision history:
   13 December 2004 : Initial version.
*/

#include <cstdlib>
#include <set>
#include <list>
#include <vector>
#include <iterator>
#include <functional>
#include <boost/test/minimal.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size_type.hpp>
#include "../../../boost/range_ex/algorithm.hpp"
#include "../../../boost/range_ex/transform.hpp"
#include "../../../boost/range_ex/indirect.hpp"
#include "../../../boost/range_ex/filter.hpp"
#include "../../../boost/range_ex/reverse.hpp"

///////////////////////////////////////////////////////////////////////////////
// dummy function object, used with algorithms
//
struct null_fun
{
    template<typename T>
        void operator()(T const &t) const
    {
    }
};

///////////////////////////////////////////////////////////////////////////////
// dummy predicate, used with algorithms
//
struct null_pred
{
    template<typename T>
    bool operator()(T const &t) const
    {
        return t == T();
    }
};

///////////////////////////////////////////////////////////////////////////////
// dummy unary op, used with algorithms
//
struct null_op1
{
    template<typename T>
    T const & operator()(T const & t) const
    {
        return t;
    }
};

///////////////////////////////////////////////////////////////////////////////
// dummy binary op, used with algorithms
//
struct null_op2
{
    template<typename T,typename U>
    T const & operator()(T const & t, U const & u) const
    {
        return t;
    }
};

template<typename Rng>
void test_random_algorithms(Rng & rng, std::random_access_iterator_tag)
{
    typedef BOOST_DEDUCED_TYPENAME boost::range_result_iterator<Rng>::type iterator;
    typedef BOOST_DEDUCED_TYPENAME boost::range_value<Rng>::type value_type;
    typedef BOOST_DEDUCED_TYPENAME boost::range_size<Rng>::type size_type;
    typedef BOOST_DEDUCED_TYPENAME boost::iterator_category<iterator>::type iterator_category;

    // just make sure these compile (for now)
    if(0)
    {
        boost::random_shuffle(rng);
        boost::random_shuffle(rng, null_op1());

        boost::sort(rng);
        boost::sort(rng, std::less<value_type>());

        boost::stable_sort(rng);
        boost::stable_sort(rng, std::less<value_type>());

        boost::partial_sort(rng, boost::begin(rng));
        boost::partial_sort(rng, boost::begin(rng), std::less<value_type>());

        boost::nth_element(rng, boost::begin(rng));
        boost::nth_element(rng, boost::begin(rng), std::less<value_type>());

        boost::push_heap(rng);
        boost::push_heap(rng, std::less<value_type>());

        boost::pop_heap(rng);
        boost::pop_heap(rng, std::less<value_type>());

        boost::make_heap(rng);
        boost::make_heap(rng, std::less<value_type>());

        boost::sort_heap(rng);
        boost::sort_heap(rng, std::less<value_type>());
    }
}

template<typename Rng>
void test_random_algorithms(Rng & rng, std::input_iterator_tag)
{
    // no-op
}

template<typename Rng>
void test_algorithms(Rng & rng)
{
    typedef BOOST_DEDUCED_TYPENAME boost::range_result_iterator<Rng>::type iterator;
    typedef BOOST_DEDUCED_TYPENAME boost::range_value<Rng>::type value_type;
    typedef BOOST_DEDUCED_TYPENAME boost::range_size<Rng>::type size_type;
    typedef BOOST_DEDUCED_TYPENAME boost::iterator_category<iterator>::type iterator_category;

    // just make sure these compile (for now)
    if(0)
    {
        value_type val = value_type();

        value_type rng2[] = {value_type(),value_type(),value_type()};
        typedef value_type* iterator2;

        value_type out[100] = {};
        typedef value_type* out_iterator;

        null_fun f = null_fun();
        iterator i = iterator();
        bool b = bool();
        out_iterator o = out_iterator();
        size_type s = size_type();

        f = boost::for_each(rng, null_fun());

        i = boost::find(rng, val);
        i = boost::find_if(rng, null_pred());

        i = boost::find_end(rng, rng2);
        i = boost::find_end(rng, rng2, std::equal_to<value_type>());

        i = boost::find_first_of(rng, rng2);
        i = boost::find_first_of(rng, rng2, std::equal_to<value_type>());

        i = boost::adjacent_find(rng);
        i = boost::adjacent_find(rng, std::equal_to<value_type>());

        s = boost::count(rng, val);
        s = boost::count_if(rng, null_pred());

        std::pair<iterator,iterator2> p1;
        p1 = boost::mismatch(rng, boost::begin(rng2));
        p1 = boost::mismatch(rng, boost::begin(rng2),std::equal_to<value_type>());

        b = boost::equal(rng, boost::begin(rng2));
        b = boost::equal(rng, boost::begin(rng2),std::equal_to<value_type>());

        i = boost::search(rng, rng2);
        i = boost::search(rng, rng2, std::equal_to<value_type>());

        o = boost::copy(rng, boost::begin(out));
        o = boost::copy_backward(rng, boost::end(out));

        o = boost::transform(rng, boost::begin(out), null_op1());
        o = boost::transform(rng, boost::begin(rng2), boost::begin(out), null_op2());

        boost::replace(rng, val, val);
        boost::replace_if(rng, null_pred(), val);

        o = boost::replace_copy(rng, boost::begin(out), val, val);
        o = boost::replace_copy_if(rng, boost::begin(out), null_pred(), val);

        boost::fill(rng, val);
        boost::fill_n(rng, boost::size(rng), val);

        boost::generate(rng, &std::rand);
        boost::generate_n(rng, boost::size(rng), &std::rand);

        i = boost::remove(rng, val);
        i = boost::remove_if(rng, null_pred());

        o = boost::remove_copy(rng, boost::begin(out), val);
        o = boost::remove_copy_if(rng, boost::begin(out), null_pred());

        i = boost::unique(rng);
        i = boost::unique(rng, std::equal_to<value_type>());

        o = boost::unique_copy(rng, boost::begin(out));
        o = boost::unique_copy(rng, boost::begin(out), std::equal_to<value_type>());

        boost::reverse(rng);

        o = boost::reverse_copy(rng, boost::begin(out));

        boost::rotate(rng, boost::begin(rng));

        o = boost::rotate_copy(rng, boost::begin(rng), boost::begin(out));

        i = boost::partition(rng, null_pred());
        i = boost::stable_partition(rng, null_pred());

        o = boost::partial_sort_copy(rng, out);
        o = boost::partial_sort_copy(rng, out, std::less<value_type>());

        i = boost::lower_bound(rng, val);
        i = boost::lower_bound(rng, val, std::less<value_type>());

        i = boost::upper_bound(rng, val);
        i = boost::upper_bound(rng, val, std::less<value_type>());

        std::pair<iterator,iterator> p2;
        p2 = boost::equal_range(rng, val);
        p2 = boost::equal_range(rng, val, std::less<value_type>());

        b = boost::binary_search(rng, val);
        b = boost::binary_search(rng, val, std::less<value_type>());

        boost::inplace_merge(rng, boost::begin(rng));
        boost::inplace_merge(rng, boost::begin(rng), std::less<value_type>());

        b = boost::includes(rng, rng2);
        b = boost::includes(rng, rng2, std::equal_to<value_type>());

        o = boost::set_union(rng, rng2, boost::begin(out));
        o = boost::set_union(rng, rng2, boost::begin(out), std::equal_to<value_type>());

        o = boost::set_intersection(rng, rng2, boost::begin(out));
        o = boost::set_intersection(rng, rng2, boost::begin(out), std::equal_to<value_type>());

        o = boost::set_difference(rng, rng2, boost::begin(out));
        o = boost::set_difference(rng, rng2, boost::begin(out), std::equal_to<value_type>());

        o = boost::set_symmetric_difference(rng, rng2, boost::begin(out));
        o = boost::set_symmetric_difference(rng, rng2, boost::begin(out), std::equal_to<value_type>());

        i = boost::min_element(rng);
        i = boost::min_element(rng, std::less<value_type>());

        i = boost::max_element(rng);
        i = boost::max_element(rng, std::less<value_type>());

        b = boost::lexicographical_compare(rng, rng);
        b = boost::lexicographical_compare(rng, rng, std::equal_to<value_type>());

        b = boost::next_permutation(rng);
        b = boost::next_permutation(rng, std::less<value_type>());

        b = boost::prev_permutation(rng);
        b = boost::prev_permutation(rng, std::less<value_type>());
    }

    // test the algorithms that require a random-access range
    test_random_algorithms(rng, iterator_category());
}

int* addr(int &i) { return &i; }
bool true_(int) { return true; }

///////////////////////////////////////////////////////////////////////////////
// test_main
//   
int test_main( int, char*[] )
{
    // int_iterator
    typedef ::boost::counting_iterator<int> int_iterator;

    // define come containers
    char ntcs_buffer[] = "\1\2\3\4\5";
    char *ntcs  = ntcs_buffer;
    int array[] = { 1,2,3,4,5 };
    std::list<int> my_list(int_iterator(1),int_iterator(6));
    std::vector<int> my_vector(int_iterator(1),int_iterator(6));
    std::pair<std::vector<int>::iterator,std::vector<int>::iterator> my_pair(my_vector.begin(),my_vector.end());

    // test the algorithms with list and const list
    test_algorithms(ntcs);
    test_algorithms(array);
    test_algorithms(my_list);
    test_algorithms(my_vector);
    test_algorithms(my_pair);

    boost::reverse_range<std::vector<int> > r = my_vector | boost::adaptor::reverse;
    typedef boost::transform_range<std::vector<int>,int*(*)(int&)> int_ptr_range;
    int_ptr_range t = my_vector | boost::adaptor::transform(&addr);
    boost::indirect_range<int_ptr_range> i = t | boost::adaptor::indirect;
    boost::filter_range<std::vector<int>, bool(*)(int)> f = my_vector | boost::adaptor::filter(&true_);

    return 0;
}
