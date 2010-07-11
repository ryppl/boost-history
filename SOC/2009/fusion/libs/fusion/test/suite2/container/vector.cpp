/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include "../internal/base.hpp"

BOOST_TEST_CASE_TEMPLATE_FUNCTION(construction,N)
{
    typename
        rebind<
            typename generate_types<N, mpl::false_>::type
          , fusion::vector<>
        >::type
    copyable;
}

boost::unit_test::test_suite*
init_unit_test_suite(int, char*[])
{
    using boost::unit_test::test_suite;
    using boost::unit_test::framework::master_test_suite;

    test_suite* construction_suite=BOOST_TEST_SUITE("construction");
    construction_suite->add(
        BOOST_TEST_CASE_TEMPLATE(construction, test_max_n_range));

    master_test_suite().add(construction_suite);

    return 0;
}
