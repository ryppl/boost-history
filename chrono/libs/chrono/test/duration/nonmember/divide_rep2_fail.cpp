//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>

#include "../../rep.h"

void test()
{
    boost::chrono::duration<Rep> d(Rep(15));
    d = d / 5;
}
