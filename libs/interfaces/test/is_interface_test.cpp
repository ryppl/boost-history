// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <string>
#include <boost/interfaces.hpp>
#include <boost/interfaces/const_view.hpp>
#include <boost/interfaces/is_interface.hpp>
#include <boost/static_assert.hpp>
#include "./detail/hierarchies.hpp"
#include "./detail/template_hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;
    BOOST_STATIC_ASSERT(is_interface<ifirst>::value);
    BOOST_STATIC_ASSERT(is_interface<iderived1>::value);
    BOOST_STATIC_ASSERT(is_interface<iderived4>::value);
    BOOST_STATIC_ASSERT((is_interface< itemplate_first<int, const char*> >::value));
    BOOST_STATIC_ASSERT((is_interface< itemplate_derived1<int, const char*> >::value));
    BOOST_STATIC_ASSERT((is_interface< itemplate_derived4<int, const char*> >::value));
    BOOST_STATIC_ASSERT(is_interface< const_view<ifirst> >::value);
    BOOST_STATIC_ASSERT(!is_interface<std::string>::value);
    BOOST_STATIC_ASSERT(!is_interface<const std::string&>::value);
    BOOST_STATIC_ASSERT(!is_interface<int>::value);
    BOOST_STATIC_ASSERT(!is_interface<int[1]>::value);
    BOOST_STATIC_ASSERT(!is_interface<int(&)[1]>::value);
    BOOST_STATIC_ASSERT(!is_interface<void(void)>::value);
    BOOST_STATIC_ASSERT(!is_interface<void(&)(void)>::value);
}
