
// Copyright 2006-2007 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/limits.hpp>
#include "../helpers/random_values.hpp"

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

test::seed_t seed(783656);

template <class X>
void load_factor_tests(X* = 0)
{
    X x;

    BOOST_TEST(x.max_load_factor() == 1.0);
    BOOST_TEST(x.load_factor() == 0);

    // A valid implementation could fail these tests, but I think they're
    // reasonable.
    x.max_load_factor(2.0); BOOST_TEST(x.max_load_factor() == 2.0);
    x.max_load_factor(0.5); BOOST_TEST(x.max_load_factor() == 0.5);
}

template <class X>
void insert_test(X*, float mlf)
{
    X x;
    x.max_load_factor(mlf);
    float b = x.max_load_factor();

    test::random_values<X> values(1000);

    for(typename test::random_values<X>::const_iterator
            it = values.begin(), end = values.end(); it != end; ++it)
    {
        typename X::size_type old_size = x.size(),
                 old_bucket_count = x.bucket_count();
        x.insert(*it);
        if(old_size + 1 < b * old_bucket_count)
            BOOST_TEST(x.bucket_count() == old_bucket_count);
    }
}

template <class X>
void load_factor_insert_tests(X* ptr = 0)
{
    insert_test(ptr, 1.0f);
    insert_test(ptr, 0.1f);
    insert_test(ptr, 100.0f);

    insert_test(ptr, (std::numeric_limits<float>::min)());

    if(std::numeric_limits<float>::has_infinity)
        insert_test(ptr, std::numeric_limits<float>::infinity());
}

int main()
{
    load_factor_tests((boost::unordered_set<int>*) 0);
    load_factor_tests((boost::unordered_multiset<int>*) 0);
    load_factor_tests((boost::unordered_map<int, int>*) 0);
    load_factor_tests((boost::unordered_multimap<int, int>*) 0);

    load_factor_insert_tests((boost::unordered_set<int>*) 0);
    load_factor_insert_tests((boost::unordered_multiset<int>*) 0);
    load_factor_insert_tests((boost::unordered_map<int, int>*) 0);
    load_factor_insert_tests((boost::unordered_multimap<int, int>*) 0);

    return boost::report_errors();
}

#if defined(BOOST_MSVC)
#pragma warning(pop)
#pragma warning(disable:4127) // conditional expression is constant
#endif
