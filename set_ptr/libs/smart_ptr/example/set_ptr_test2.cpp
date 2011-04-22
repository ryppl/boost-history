/**
	@file
	set_ptr_test2.cpp

	@author
	Steven Watanabe <watanabesj@gmail.com>
*/

#include <boost/set_ptr.hpp>
#include <boost/set_allocator.hpp>

#include <list>
#include <vector>
#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/array.hpp>


static int count;

using boost::set_ptr;
using boost::set;
using boost::set_allocator;
using boost::operator ==;
using boost::operator !=;

struct node {
    node() {
        ++count;
    }
    ~node() {
        --count;
    }
    set_ptr<node> prior;
    set_ptr<node> next;
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
            back = new set<node>();
        } else {
            back->next = new set<node>();
            back->next->prior = back;
            back = back->next;
        }
    }
	~list()
	{
	}
private:
    set_ptr<node> front;
    set_ptr<node> back;
};

struct vector {
    vector() { ++count; }
    ~vector() { --count; }
    vector(const vector& other) : elements(other.elements) { ++count; }
    //std::vector<set_ptr<vector> > elements;
    std::list<set_ptr<vector>, set_allocator< set_ptr<vector> > > elements; //! works fine
};

struct create_type {
    template<class T>
    void operator()(T) const {
        new set<boost::array<char, T::value> >();
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
        set_ptr<vector> v = new set<vector>();
        v->elements.push_back(v);
    }
    std::cout << count << std::endl;

    count = 0;
    {
        set_ptr<vector> v = new set<vector>();
        v->elements.push_back(v);
    }
    std::cout << count << std::endl;

    {
        vector v;
        v.elements.push_back(new set<vector>());
    }
    std::cout << count << std::endl;

    count = 0;
    {
        set_ptr<int> test = new set<int>(5);
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
