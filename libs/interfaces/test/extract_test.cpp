// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/extract.hpp>
#include "./detail/counter.hpp"
#include "./detail/hierarchies.hpp"
#include "./detail/unit_test_framework.hpp"
using boost::unit_test_framework::test_suite; 

namespace boost { namespace interfaces { namespace test { 

template<int N>
struct base : cfirst<N> { };

template<int N>
struct derived : base<N> { };

template<int N>
struct twice_derived : derived<N> { };

} } } // End namespaces test, interfaces, boost.

void extract_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    {
        base<1> base1;
        ifirst b = base1;
        try {
            base<1>& base2 = extract< base<1> >(b);
        } catch (bad_extract&) {
            bool extract_failed = false;
            BOOST_CHECK(extract_failed);
        }
    }

    {
        derived<1> derived1;
        ifirst d = derived1;
        try {
            derived<1>& derived2 = extract< derived<1> >(d);
        } catch (bad_extract&) {
            bool extract_failed = false;
            BOOST_CHECK(extract_failed);
        }
    }

    {
        // Can't extract a more derived type than the one originally
        // passed to the interface ctor.
        derived<1> derived1;
        base<1>& base1 = derived1;
        ifirst d = base1;
        try {
            derived<1>& derived2 = extract< derived<1> >(d);
            bool extract_suceeeded = false;
            BOOST_CHECK(!extract_suceeeded);
        } catch (bad_extract&) { }
    }

    {       
        // Can't extract a more derived type than the actual 
        // dynamic type.
        derived<1> derived1;
        ifirst d = derived1;
        try {
            twice_derived<1>& twice_derived1 = extract< twice_derived<1> >(d);
            bool extract_suceeeded = false;
            BOOST_CHECK(!extract_suceeeded);
        } catch (bad_extract&) { }
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("extract test");
    test->add(BOOST_TEST_CASE(&extract_test));
    return test;
}
