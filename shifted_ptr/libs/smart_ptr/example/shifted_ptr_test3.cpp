/**
	@file
	shifted_ptr_test3.cpp

	@author
	Steven Watanabe <watanabesj@gmail.com>
*/



#include <boost/shifted_ptr.hpp>

#include <vector>
#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/array.hpp>

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

static int count;

using boost::shifted_ptr;
using boost::new_sh;

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
            front = back = new_sh<node>();
        } else {
            back->next = new_sh<node>();
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
        new_sh<boost::array<char, T::value> >();
    }
};

BOOST_AUTO_TEST_CASE(test_shifted_ptr) {
    {
        shifted_ptr<vector> v = new_sh<vector>();
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
        std::cout << count << std::endl;
    }
    BOOST_CHECK_EQUAL(count, 0);
    count = 0;

    {
        shifted_ptr<int> test = new_sh<int>(5);
        test = test;
        
        BOOST_CHECK_NE(test.get(), static_cast<int*>(0));
        BOOST_CHECK_EQUAL(*test, 5);
    }

    for(int i = 0; i < 500; ++i) {
        boost::mpl::for_each<boost::mpl::range_c<int, 1, 100> >(create_type());
    }

    {
        shifted_ptr<vector> v = new_sh<vector>();
        v->elements.push_back(v);
    }
    BOOST_CHECK_EQUAL(count, 0);
    count = 0;
    
    {
        vector v;
        v.elements.push_back(new_sh<vector>());
    }
    BOOST_CHECK_EQUAL(count, 0);
    count = 0;
}
