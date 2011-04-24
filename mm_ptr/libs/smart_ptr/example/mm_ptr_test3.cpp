/**
	@file
	mm_ptr_test3.cpp

	@note
	MinGW users must link with: -Wl,--enable-runtime-pseudo-reloc
	
	@author
	Steven Watanabe <watanabesj@gmail.com>
*/



#include <boost/mm_ptr.hpp>
#include <boost/mm_allocator.hpp>

#include <vector>
#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/array.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

static int count;

using boost::mm_ptr;
using boost::mm;
using boost::mm_allocator;

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
    mm_ptr<node> prior;
    mm_ptr<node> next;
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
            front = back = new mm<node>();
        } else {
            back->next = new mm<node>();
            back->next->prior = back;
            back = back->next;
        }
    }
private:
    mm_ptr<node> front;
    mm_ptr<node> back;
};

struct vector {
    vector() { ++count; }
    ~vector() { --count; }
    vector(const vector& other) : elements(other.elements) { ++count; }
    std::vector<mm_ptr<vector>, mm_allocator<mm_ptr<vector> > > elements;
};

struct create_type {
    template<class T>
    void operator()(T) const {
        new mm<boost::array<char, T::value> >();
    }
};

BOOST_AUTO_TEST_CASE(test_mm_ptr) {

    count = 0;
    {
        mm_ptr<vector> v = new mm<vector>();
        v->elements.push_back(v);
    }
    BOOST_CHECK_EQUAL(count, 0);

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

    count = 0;
    {
        mm_ptr<int> test = new mm<int>(5);
        test = test;
        
        BOOST_CHECK_NE(test.get(), static_cast<int*>(0));
        BOOST_CHECK_EQUAL(*test, 5);
    }

    for(int i = 0; i < 500; ++i) {
        boost::mpl::for_each<boost::mpl::range_c<int, 1, 100> >(create_type());
    }

    count = 0;
    {
        mm_ptr<vector> v = new mm<vector>();
        v->elements.push_back(v);
    }
    BOOST_CHECK_EQUAL(count, 0);

    {
        vector v;
        v.elements.push_back(new mm<vector>());
    }
    BOOST_CHECK_EQUAL(count, 0);

}
