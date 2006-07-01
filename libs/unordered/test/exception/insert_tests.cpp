
// Copyright 2006 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./containers.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/exception_safety.hpp>
#include <string>
#include "../helpers/random_values.hpp"
#include "../helpers/invariants.hpp"
#include "../helpers/strong.hpp"

#include <cmath>

template <class T>
struct insert_test_base : public test::exception_base
{
    test::random_values<T> values;
    insert_test_base(unsigned int count = 5) : values(count) {}

    typedef T data_type;
    typedef test::strong<T> strong_type;

    data_type init() const {
        return T();
    }

    void check(T const& x, strong_type const& strong) const {
        std::string scope(test::scope);

        if(scope.find_first_of("hash::operator()") == std::string::npos)
            strong.test(x);
        test::check_equivalent_keys(x);
    }
};

template <class T>
struct insert_test1 : public insert_test_base<T>
{
    typedef typename insert_test_base<T>::strong_type strong_type;

    void run(T& x, strong_type& strong) const {
        for(typename test::random_values<T>::const_iterator
                it = this->values.begin(), end = this->values.end(); it != end; ++it)
        {
            strong.store(x);
            x.insert(*it);
        }
    }
};

template <class T>
struct insert_test2 : public insert_test_base<T>
{
    typedef typename insert_test_base<T>::strong_type strong_type;

    void run(T& x, strong_type& strong) const {
        for(typename test::random_values<T>::const_iterator
                it = this->values.begin(), end = this->values.end(); it != end; ++it)
        {
            strong.store(x);
            x.insert(x.begin(), *it);
        }
    }
};

template <class T>
struct insert_test3 : public insert_test_base<T>
{
    typedef typename insert_test_base<T>::strong_type strong_type;

    void run(T& x, strong_type& strong) const {
        // I don't think there's any need for this here.
        //strong.store(x);
        x.insert(this->values.begin(), this->values.end());
    }

    void check(T const& x) const {
        test::check_equivalent_keys(x);
    }
};

template <class T>
struct insert_test4 : public insert_test_base<T>
{
    typedef typename insert_test_base<T>::strong_type strong_type;

    void run(T& x, strong_type& strong) const {
        for(typename test::random_values<T>::const_iterator
                it = this->values.begin(), end = this->values.end(); it != end; ++it)
        {
            strong.store(x);
            x.insert(it, boost::next(it));
        }
    }
};

template <class T>
struct insert_test_rehash1 : public insert_test_base<T>
{
    typedef typename insert_test_base<T>::strong_type strong_type;

    insert_test_rehash1() : insert_test_base<T>(1000) {}

    T init() const {
        typedef typename T::size_type size_type;

        T x;
        x.max_load_factor(0.25);
        size_type bucket_count = x.bucket_count();
        size_type initial_elements = static_cast<size_type>(
            std::ceil(bucket_count * x.max_load_factor()) - 1);
        BOOST_REQUIRE(initial_elements < this->values.size());
        x.insert(this->values.begin(),
                boost::next(this->values.begin(), initial_elements));
        BOOST_REQUIRE(bucket_count == x.bucket_count());
        return x;
    }

    void run(T& x, strong_type& strong) const {
        typename T::size_type bucket_count = x.bucket_count();
        int count = 0;
        typename T::const_iterator pos = x.cbegin();

        for(typename test::random_values<T>::const_iterator
            it = boost::next(this->values.begin(), x.size()), end = this->values.end();
            it != end && count < 10; ++it, ++count)
        {
            strong.store(x);
            pos = x.insert(pos, *it);
        }

        // This isn't actually a failure, but it means the test isn't doing its
        // job.
        BOOST_REQUIRE(x.bucket_count() != bucket_count);
    }
};

template <class T>
struct insert_test_rehash2 : public insert_test_rehash1<T>
{
    typedef typename insert_test_base<T>::strong_type strong_type;

    void run(T& x, strong_type& strong) const {
        typename T::size_type bucket_count = x.bucket_count();
        int count = 0;

        for(typename test::random_values<T>::const_iterator
            it = boost::next(this->values.begin(), x.size()), end = this->values.end();
            it != end && count < 10; ++it, ++count)
        {
            strong.store(x);
            x.insert(*it);
        }

        // This isn't actually a failure, but it means the test isn't doing its
        // job.
        BOOST_REQUIRE(x.bucket_count() != bucket_count);
    }
};

RUN_EXCEPTION_TESTS(
    (insert_test1)(insert_test2)(insert_test3)(insert_test4)
    (insert_test_rehash1)(insert_test_rehash2),
    CONTAINER_SEQ)
