/**
	@file
	shifted_ptr_test3.cpp

	@note
	MinGW users must link with: -Wl,--enable-runtime-pseudo-reloc
	
	@author
	Steven Watanabe <watanabesj@gmail.com>
*/



#include <boost/shifted_ptr.hpp>

#include <vector>
#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/array.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

static int count;

using boost::shifted_ptr;
using boost::shifted;

struct node {
    node() {
        ++count;
    }
    ~node() {
        --count;
    }
    node(const node&) {
        ++count;
    }
    shifted_ptr<node> prior;
    shifted_ptr<node> next;
};

struct list {
public:
    list() {}
    void clear() {
        front.reset();
        back.reset();
    }
    void insert() {
        if(front.get() == 0) {
            front = back = new shifted<node>();
        } else {
            back->next = new shifted<node>();
            back->next->prior = back;
            back = back->next;
        }
    }
private:
    shifted_ptr<node> front;
    shifted_ptr<node> back;
};

struct vector {
    vector() { ++count; }
    ~vector() { --count; }
    vector(const vector& other) : elements(other.elements) { ++count; }
    std::vector<shifted_ptr<vector> > elements;
};

struct create_type {
    template<class T>
    void operator()(T) const {
        new shifted<boost::array<char, T::value> >();
    }
};

BOOST_AUTO_TEST_CASE(test_shifted_ptr) {
/*
    count = 0;
    {
        shifted_ptr<vector> v = new shifted<vector>();
        v->elements.push_back(v);
    }
    BOOST_CHECK_EQUAL(count, 0);
*/
    count = 0;
    {
        list l;
        for(int j = 0; j < 2; ++j) {
            for(int i = 0; i < 2; ++i) {
                l.insert();
            }
            l.clear();
        }
    }
    BOOST_CHECK_EQUAL(count, 0);
/*
    count = 0;
    {
        shifted_ptr<int> test = new shifted<int>(5);
        test = test;
        
        BOOST_CHECK_NE(test.get(), static_cast<int*>(0));
        BOOST_CHECK_EQUAL(*test, 5);
    }

    for(int i = 0; i < 500; ++i) {
        boost::mpl::for_each<boost::mpl::range_c<int, 1, 100> >(create_type());
    }
*/
/*
    count = 0;
    {
        shifted_ptr<vector> v = new shifted<vector>();
        v->elements.push_back(v);
    }
    BOOST_CHECK_EQUAL(count, 0);
*/
    {
        vector v;
        v.elements.push_back(new shifted<vector>());
    }
    BOOST_CHECK_EQUAL(count, 0);
}
