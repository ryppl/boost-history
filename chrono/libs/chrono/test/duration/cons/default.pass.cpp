//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// Rep must be default initialized, not initialized with 0

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "../../rep.h"

template <class D>
void
test()
{
    D d;
    BOOST_TEST(d.count() == typename D::rep());
}

int main()
{
    test<boost::chrono::duration<Rep> >();
    return boost::report_errors();
}
