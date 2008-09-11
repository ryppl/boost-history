/*
    Copyright 2005-2007 Adobe Systems Incorporated

    Use, modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).

*/

/*************************************************************************************************/

#include <boost/test/minimal.hpp>

#include <iterator>
#include <vector>

#include <boost/move/move.hpp>
#include "./vector.hpp"

#include <memory>

/*************************************************************************************************/

#define BOOST_CHECK_EQUAL(x, y) BOOST_CHECK(x == y)

/*************************************************************************************************/

// Taken from adobe::destroy

template <typename T> // T models Regular
inline void destroy(T* p) { p->~T(); }

template <typename F> // F models ForwardIterator
inline void destroy(F f, F l)
{
    while (f != l) {
        destroy(&*f);
        ++f;
    }
}

/*************************************************************************************************/

void move_test()
{
    typedef test_vector<int>          vector_t;
    typedef test_vector<vector_t>     vector_vector_t;
    typedef std::vector<int>          std_vector_t;
    typedef test_vector<std_vector_t> vector_std_vector_t;
    typedef test_vector<std::auto_ptr<int> > vector_auto_ptr_t;
    
    // Test is_movable<> template function

    BOOST_CHECK(boost::is_movable<std_vector_t>::value);
    BOOST_CHECK(boost::is_movable<vector_t>::value);

    { // Test move_sink<> via push_back
    vector_t x(3, 4);

    x.push_back(7);
    int* addr = x.begin();
    vector_vector_t y;
    y.push_back(boost::move(x));
    
    BOOST_CHECK_EQUAL(addr, &y[0][0]);
    }

    { // Test move with a copy only type
    std_vector_t x(3, 4);
    vector_std_vector_t y;
    y.push_back(boost::move(x)); // really copies
    }
    
    { // Test move algorithm and back_mover
        vector_t x(3, 4);
        x.push_back(7);
        
        vector_vector_t y;
        y.push_back(x);
        y.push_back(x);
        y.push_back(x);
        
        vector_vector_t z;
        int* addr = &y.back().back();
        boost::move(y, boost::back_mover(z));
        BOOST_CHECK_EQUAL(addr, &z.back().back());
    }
    
    { // Test move_backward
        vector_t x(3, 4);
        x.push_back(7);
        
        vector_vector_t y;
        y.push_back(x);
        y.push_back(x);
        y.push_back(x);
        
        vector_vector_t z(y);
        int* addr = &y.back().back();
        boost::move_backward(y, z.end());
        BOOST_CHECK_EQUAL(addr, &z.back().back());
    }
    
    { // Test move_construct
        vector_t x(3, 4);
        vector_t y(x);
        x.~vector_t();
        int* addr = y.begin();
        boost::move_construct(&x, y);
        BOOST_CHECK_EQUAL(addr, x.begin());
    }
    
    { // Test uninitialized_move
        vector_t x[3];
        vector_t y[3];
        y[0].push_back(7);
        
        destroy(&x[0], &x[3]);
        
        int* addr = y[0].begin();
        boost::uninitialized_move(&y[0], &y[3], &x[0]);
        BOOST_CHECK_EQUAL(addr, x[0].begin());
    }

    { // Move auto_ptr
        vector_auto_ptr_t x;
        for(int i = 0; i < 100; ++i)
            x.push_back(std::auto_ptr<int>(new int(i)));
        BOOST_CHECK_EQUAL(100, x.size());
        BOOST_CHECK_EQUAL(0, *x[0]);
        BOOST_CHECK_EQUAL(10, *x[10]);
        BOOST_CHECK_EQUAL(99, *x[99]);
    }
}

int test_main(int, char**) {
    move_test();
    return 0;
}
