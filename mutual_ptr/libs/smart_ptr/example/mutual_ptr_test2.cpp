/**
	@file
	mutual_ptr_test2.cpp

	@author
	Steven Watanabe <watanabesj@gmail.com>
*/

#include <boost/mutual_ptr.hpp>
#include <boost/mutual_allocator.hpp>

#include <list>
#include <vector>
#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/array.hpp>


static int count;

using boost::mutual_ptr;
using boost::mutual;
using boost::mutual_allocator;
using boost::operator ==;
using boost::operator !=;

struct node {
    node() {
        ++count;
    }
    ~node() {
        --count;
    }
    mutual_ptr<node> prior;
    mutual_ptr<node> next;
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
            back = new mutual<node>();
        } else {
            back->next = new mutual<node>();
            back->next->prior = back;
            back = back->next;
        }
    }
	~list()
	{
	}
private:
    mutual_ptr<node> front;
    mutual_ptr<node> back;
};

struct vector {
    vector() { ++count; }
    ~vector() { --count; }
    vector(const vector& other) : elements(other.elements) { ++count; }
    //std::vector<mutual_ptr<vector> > elements;
    std::list<mutual_ptr<vector>, mutual_allocator< mutual_ptr<vector> > > elements; //! works fine
};

struct create_type {
    template<class T>
    void operator()(T) const {
        new mutual<boost::array<char, T::value> >();
    }
};

int main() {
    count = 0;
	{
	    list l;
	    for(int j = 0; j < 2; ++j) {
	        for(int i = 0; i < 1000; ++i) {
	            l.insert();
	        }
	        l.clear();
	    }
	}
    std::cout << count << std::endl;

    count = 0;
    {
        mutual_ptr<vector> v = new mutual<vector>();
        v->elements.push_back(v);
    }
    std::cout << count << std::endl;

    count = 0;
    {
        mutual_ptr<vector> v = new mutual<vector>();
        v->elements.push_back(v);
    }
    std::cout << count << std::endl;

    {
        vector v;
        v.elements.push_back(new mutual<vector>());
    }
    std::cout << count << std::endl;

    count = 0;
    {
        mutual_ptr<int> test = new mutual<int>(5);
        test = test;
        
        std::cout << "test = " << * test << std::endl;
    }
    std::cout << count << std::endl;

    count = 0;
    for(int i = 0; i < 500; ++i) {
        boost::mpl::for_each<boost::mpl::range_c<int, 1, 100> >(create_type());
    }
    std::cout << count << std::endl;

    //_exit(-1); // bypassing bug in pool destructor
}
