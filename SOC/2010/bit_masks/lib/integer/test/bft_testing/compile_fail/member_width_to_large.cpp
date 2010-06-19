//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/details/member.hpp>

using namespace boost;

// this fails because the type in side member is to large for the return type.
struct pink;

int main() {
    typedef member<int, pink, 33>::type failure;
}
