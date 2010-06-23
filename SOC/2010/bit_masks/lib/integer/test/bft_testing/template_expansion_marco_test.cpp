//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>

#include <boost/assert.hpp>
#include <typeinfo>
#include <iostream>

using namespace boost;
using namespace std;

template <BOOST_MAKE_BFT_TEMPLATE_PARAMS()>
struct ham_bone {
    virtual void foo() { }
};

int main() {
    ham_bone<int,int,int,int,int,int,int,int,int,int> temp;
    cout << typeid( &temp ).name() << endl;
    return 0;
}


