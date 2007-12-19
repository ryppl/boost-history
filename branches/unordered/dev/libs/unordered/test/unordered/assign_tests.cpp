
// Copyright 2006-2007 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "../objects/test.hpp"
#include "../helpers/random_values.hpp"
#include "../helpers/tracker.hpp"
#include "../helpers/equivalent.hpp"

#include <iostream>

test::seed_t seed(96785);

template <class T>
void assign_tests1(T* = 0)
{
    typename T::hasher hf;
    typename T::key_equal eq;

    std::cerr<<"assign_tests1.1\n";
    {
        T x;
        x = x;
        BOOST_TEST(x.empty());
        BOOST_TEST(test::equivalent(x.hash_function(), hf));
        BOOST_TEST(test::equivalent(x.key_eq(), eq));
    }

    std::cerr<<"assign_tests1.2\n";
    {
        test::random_values<T> v(1000);
        T x(v.begin(), v.end());

        test::ordered<T> tracker = test::create_ordered(x);
        tracker.insert_range(v.begin(), v.end());

        x = x;
        tracker.compare(x);

        T y;
        y.max_load_factor(x.max_load_factor() / 20);
        y = x;
        tracker.compare(y);
        BOOST_TEST(x.max_load_factor() == y.max_load_factor());
    }
}

template <class T>
void assign_tests2(T* = 0)
{
    typename T::hasher hf1(1);
    typename T::hasher hf2(2);
    typename T::key_equal eq1(1);
    typename T::key_equal eq2(2);
    typename T::allocator_type al1(1);
    typename T::allocator_type al2(2);

    std::cerr<<"assign_tests2.1\n";
    {
        test::random_values<T> v(1000);
        T x1(v.begin(), v.end(), 0, hf1, eq1);
        T x2(0, hf2, eq2);
        x2 = x1;
        BOOST_TEST(test::equivalent(x2.hash_function(), hf1));
        BOOST_TEST(test::equivalent(x2.key_eq(), eq1));
        test::check_container(x2, v);
    }

    std::cerr<<"assign_tests2.2\n";
    {
        test::random_values<T> v1(100), v2(100);
        T x1(v1.begin(), v1.end(), 0, hf1, eq1, al1);
        T x2(v2.begin(), v2.end(), 0, hf2, eq2, al2);
        x2 = x1;
        BOOST_TEST(test::equivalent(x2.hash_function(), hf1));
        BOOST_TEST(test::equivalent(x2.key_eq(), eq1));
        BOOST_TEST(test::equivalent(x2.get_allocator(), al2));
        test::check_container(x2, v1);
    }
}

int main()
{
    assign_tests1((boost::unordered_set<int>*) 0);
    assign_tests1((boost::unordered_multiset<int>*) 0);
    assign_tests1((boost::unordered_map<int, int>*) 0);
    assign_tests1((boost::unordered_multimap<int, int>*) 0);

    assign_tests1((boost::unordered_set<test::object, test::hash, test::equal_to, test::allocator<test::object> >*) 0);
    assign_tests1((boost::unordered_multiset<test::object, test::hash, test::equal_to, test::allocator<test::object> >*) 0);
    assign_tests1((boost::unordered_map<test::object, test::object, test::hash, test::equal_to, test::allocator<test::object> >*) 0);
    assign_tests1((boost::unordered_multimap<test::object, test::object, test::hash, test::equal_to, test::allocator<test::object> >*) 0);

    assign_tests1((boost::unordered_set<test::equivalent_object, test::hash, test::equal_to, test::allocator<test::equivalent_object> >*) 0);
    assign_tests1((boost::unordered_multiset<test::equivalent_object, test::hash, test::equal_to, test::allocator<test::equivalent_object> >*) 0);
    assign_tests1((boost::unordered_map<test::equivalent_object, test::equivalent_object, test::hash, test::equal_to, test::allocator<test::equivalent_object> >*) 0);
    assign_tests1((boost::unordered_multimap<test::equivalent_object, test::equivalent_object, test::hash, test::equal_to, test::allocator<test::equivalent_object> >*) 0);

    assign_tests2((boost::unordered_set<test::object, test::hash, test::equal_to, test::allocator<test::object> >*) 0);
    assign_tests2((boost::unordered_multiset<test::equivalent_object, test::hash, test::equal_to, test::allocator<test::equivalent_object> >*) 0);
    assign_tests2((boost::unordered_map<test::equivalent_object, test::equivalent_object, test::hash, test::equal_to, test::allocator<test::equivalent_object> >*) 0);
    assign_tests2((boost::unordered_multimap<test::object, test::object, test::hash, test::equal_to, test::allocator<test::object> >*) 0);

    return boost::report_errors();
}
