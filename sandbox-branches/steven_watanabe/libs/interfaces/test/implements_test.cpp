// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <string>
#include <boost/interfaces.hpp>
#include <boost/interfaces/implements.hpp>
#include <boost/static_assert.hpp>
#include "./detail/hierarchies.hpp"
#include "./detail/template_hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;
    BOOST_STATIC_ASSERT((implements<cfirst<1>, ifirst>::value));
    BOOST_STATIC_ASSERT((implements<cderived1<1>, ifirst>::value));
    BOOST_STATIC_ASSERT((implements<cderived1<1>, iderived1>::value));
    BOOST_STATIC_ASSERT((implements<cderived4<1>, ifirst>::value));
    BOOST_STATIC_ASSERT((implements<cderived4<1>, iderived1>::value));
    BOOST_STATIC_ASSERT((implements<cderived4<1>, iderived4>::value));
    BOOST_STATIC_ASSERT((!implements<cfirst<1>, isecond>::value));
    BOOST_STATIC_ASSERT((!implements<cderived1<1>, iderived3>::value));
    BOOST_STATIC_ASSERT((!implements<int, ifirst>::value));
    BOOST_STATIC_ASSERT((!implements<int(&)[1], ifirst>::value));
    BOOST_STATIC_ASSERT((!implements<void(void), ifirst>::value));
}
