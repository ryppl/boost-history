// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include <boost/interfaces/detail/offset_of.hpp>
#include <boost/static_assert.hpp>
#include "./detail/hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::detail;
    using namespace boost::interfaces::test;
    BOOST_STATIC_ASSERT((offset_of<ifirst, ifirst>::value) == 0);
    BOOST_STATIC_ASSERT((offset_of<iderived1, iderived1>::value) == 0);
    BOOST_STATIC_ASSERT((offset_of<iderived1, ifirst>::value) == 1);
    BOOST_STATIC_ASSERT((offset_of<iderived1, isecond>::value) == 5);
    BOOST_STATIC_ASSERT((offset_of<iderived4, iderived1>::value) == 1);
    BOOST_STATIC_ASSERT((offset_of<iderived4, ifirst>::value) == 2);
    BOOST_STATIC_ASSERT((offset_of<iderived4, isecond>::value) == 6);
    BOOST_STATIC_ASSERT((offset_of<iderived4, ithird>::value) == 11);
    BOOST_STATIC_ASSERT((offset_of<iderived4, ifourth>::value) == 15);
}
