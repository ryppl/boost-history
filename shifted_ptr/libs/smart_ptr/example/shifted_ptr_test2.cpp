/**
	@file
	shifted_ptr_test2.cpp

	@author
	Steven Watanabe <watanabesj@gmail.com>
*/

#include <boost/shifted_ptr.hpp>

#include <vector>
#include <iostream>


static int c;

using boost::shifted_ptr;
using boost::new_sh;

struct node {
    node() {
        ++c;
    }
    ~node() {
        --c;
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
            back = new_sh<node>();
        } else {
            back->next = new_sh<node>();
            back->next->prior = back;
            back = back->next;
        }
    }
	~list()
	{
	}
private:
    shifted_ptr<node> front;
    shifted_ptr<node> back;
};

int main() {
	{
	    list l;
	    for(int j = 0; j < 1; ++j) {
	        for(int i = 0; i < 1000; ++i) {
	            l.insert();
	        }
	        l.clear();
	    }
	}
    std::cout << c << std::endl;
}
