// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#include <boost/interfaces/shared_obj.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using namespace boost::interfaces;
using namespace boost::interfaces::test;
using boost::unit_test_framework::test_suite;  

template<typename T>
struct destroyer {
    void operator()(const T* t) const { delete t; }
};
                    
void deleter_test()
{
    {
        shared_obj<ifirst> obj(new cfirst<1>, destroyer< cfirst<1> > ());
    }
    BOOST_CHECK(counter< cfirst<1> >::value() == 0);

    {
        shared_obj<ifirst> obj(new cderived1<1>, destroyer< cderived1<1> >());
    }
    BOOST_CHECK(counter< cderived1<1> >::value() == 0);

    {
        shared_obj<ifirst> obj(new cderived4<1>, destroyer< cderived4<1> >());
    }
    BOOST_CHECK(counter< cderived4<1> >::value() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("shared_obj deleter test");
    test->add(BOOST_TEST_CASE(&deleter_test));
    return test;
}
