//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <typeinfo>
#include <iostream>

// This is compile pass test and needs to be moved into
// the correct location as such I think.

using namespace boost;
using namespace boost::bitfields;

template <BOOST_BFT_TEMPLATE_ARGS_NO_DEFAULTS()>
struct ham_bone { };

int main() {
    ham_bone<int,int,int,int,int,int,int,int,int,int> temp;
    std::cout << typeid( &temp ).name() << std::endl;
    return 0;
}


