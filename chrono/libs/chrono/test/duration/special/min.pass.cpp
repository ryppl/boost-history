//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

#include "../../rep.h"

template <class D>
void test()
{
    typedef typename D::rep Rep;
    Rep min_rep = (boost::chrono::duration_values<Rep>::min)();
    BOOST_TEST((D::min)().count() == min_rep);
}

int main()
{
    test<boost::chrono::duration<int> >();
    test<boost::chrono::duration<Rep> >();
    return boost::report_errors();
}
