#include <iostream>
#include "boost/constant_time_size/list.hpp"
#include <assert.h>
#include <vector>
#include <list>


void mod_list(std::list<signed int>& l) {
    l.push_back(1);
};

void const_list(const std::list<signed int>& l) {
	static signed int i =0;
	i+=l.front();  
};



template <typename my_list> 
struct tests {
void constructor_default() {
    my_list l;
    assert(l.size()==0);
}
    
void push_back() {
    my_list l;
    l.push_back(1);
    assert(l.size()==1);
}
void push_front() {
    my_list l;
    l.push_front(1); 
    assert(l.size()==1);
}
void pop_back() {
    my_list l;
    l.push_front(1); 
    l.pop_back();
    assert(l.size()==0);
}

void pop_front() {
    my_list l;
    l.push_front(1); 
    l.pop_front();
    assert(l.size()==0);
}


void constructor_allocator() {
	signed int v=8;
    my_list l(std::size_t(3), v, std::allocator<signed int>());
    assert(l.size()==3);
}

void constructor_size_t_value() {
	signed int v=8;
    my_list l(std::size_t(3), v);
    assert(l.size()==3);
}

void constructor_size_t() {
    my_list l(std::size_t(3));
    assert(l.size()==3);
}
void constructor_range() {
    std::vector<signed int> v(3);
    my_list l(v.begin(), v.end());
    assert(l.size()==3);
}
void assign() {
    my_list r(4);
    my_list l(2);
    assert(l.size()==2);
    l = r;
    assert(l.size()==r.size());
}
    
void equal() {
    my_list r(4);
    my_list l(r);
    assert(l==r);
}
void not_equal() {
    my_list r(4);
    my_list l(2);
//    assert(!(l==r));
    assert(l!=r);
}
    
void constructor_list() {
    std::list<signed int> l(2);
    my_list r(l);
    assert(l.size()==r.size());
}
      
void assign_list() {
    std::list<signed int> l1(2);
    my_list l2;
    l2 = my_list(l1);
    assert(l1.size()==l2.size());
}
    

void implicit_conversion_to_const_list() {
    my_list l1(3);
    std::list<signed int> l2(l1);
    assert(l1.size()==l2.size());
}

void conversion_init_list() {
    my_list l1(3);
    std::list<signed int> l2=l1;
    assert(l1.size()==l2.size());
}
void conversion_assign_list() {
    my_list l1(3);
    std::list<signed int> l2;
    l2 = l1;
    assert(l1.size()==l2.size());
}
    
void resize() {
    my_list l(3);
    l.resize(5);
    assert(l.size()==5);
}

void resize_less() {
    my_list l(3);
    l.resize(2);
    assert(l.size()==2);
}

void remove_value() {
	signed int v = 5;
    my_list l(std::size_t(3), v);
    l.push_back(1);
    l.remove(5);
    assert(l.size()==1);
}

void remove_iterator() {
	signed int v = 5;
    my_list l(std::size_t(3), v);
    typename my_list::iterator it =l.begin();
    ++it;
    l.erase(it); 
    assert(l.size()==2);
}
    
     
void clear() {
	signed int v = 5;
    my_list l(std::size_t(3), v);
    l.clear();
    assert(l.size()==0);
}
      
void unique() {
	signed int v = 5;
    my_list l(std::size_t(3), v);
    l.push_back(1);
    l.unique();
    assert(l.size()==2);
}


void swapt() {
	signed int v = 5;
    my_list l1(std::size_t(3), v);
    my_list l2(std::size_t(5), v);
    swap(l1,l2);
    assert(l1.size()==5);
    assert(l2.size()==3);
}

void guard()
{
      my_list l(9);
      mod_list(typename my_list::non_const(l));
      assert(l.size()==10);
      const_list(l);
      assert(l.size()==10);
}

void constructor_from_list()
{
    std::list<int> l(10);
    my_list r(l, 10);
    assert(r.size()==l.size());
}

void test() {
    constructor_from_list();
    constructor_default();
    constructor_allocator();
    constructor_size_t_value();
    constructor_size_t();
    constructor_range();
    constructor_list();
    assign();
    assign_list();
    implicit_conversion_to_const_list();
    conversion_init_list();
    conversion_assign_list();
    push_back();
    push_front();
    pop_back();
    pop_front();
    equal();
    not_equal();
    resize();
    remove_value();
    remove_iterator();
    clear();
    unique();
    swapt();
    guard();
}
};


typedef boost::constant_time_size::list<signed int, std::allocator<signed int>, 
boost::constant_time_size::size_linear_time > my_list1;

typedef boost::constant_time_size::list<signed int, std::allocator<signed int>, 
boost::constant_time_size::size_constant_time > my_list2;

typedef boost::constant_time_size::list<signed int, std::allocator<signed int>, 
boost::constant_time_size::size_quasi_constant_time > my_list3;

typedef boost::constant_time_size::list<signed int, std::allocator<signed int>, 
boost::constant_time_size::size_performance_policy<boost::constant_time_size::if_not_incoherent> > my_list4;

int main() {
//	tests<my_list1>().test();
//	tests<my_list2>().test();
	tests<my_list3>().test();
//	tests<my_list4>().test();

    return 0;
}
