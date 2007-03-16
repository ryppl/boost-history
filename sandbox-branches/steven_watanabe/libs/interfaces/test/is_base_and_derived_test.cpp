// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <string>
#include <boost/interfaces.hpp>
#include <boost/interfaces/is_base_and_derived.hpp>
#include <boost/static_assert.hpp>
#include "./detail/hierarchies.hpp"
#include "./detail/template_hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;
    BOOST_STATIC_ASSERT((is_base_and_derived<ifirst, ifirst>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived1, iderived1>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived1, ifirst>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived1, isecond>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, iderived4>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, iderived1>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, iderived3>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, ifirst>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, isecond>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, ithird>::value));
    BOOST_STATIC_ASSERT((is_base_and_derived<iderived4, ifourth>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<ifirst, iderived1>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<isecond, iderived1>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<iderived1, iderived4>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<iderived3, iderived4>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<ifirst, iderived4>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<isecond, iderived4>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<ithird, iderived4>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<ifourth, iderived4>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<int(&)[1], ifirst>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<ifirst, int(&)[1]>::value));
    BOOST_STATIC_ASSERT((!is_base_and_derived<int(&)[1], int(&)[1]>::value));
}
