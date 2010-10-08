//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// duration
// Period::num must be positive, diagnostic required.

#include <boost/chrono.hpp>

void test()
{
    typedef boost::chrono::duration<int, boost::ratio<5, -1> > D;
    D d;
}
