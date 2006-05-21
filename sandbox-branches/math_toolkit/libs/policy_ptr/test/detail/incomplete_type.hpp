// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

#ifndef BOOST_POLICY_PTR_TEST_INCOMPLETE_HPP_INCLUDED
#define BOOST_POLICY_PTR_TEST_INCOMPLETE_HPP_INCLUDED 

namespace boost { namespace test {

struct incomplete;

policy_ptr::to<incomplete> new_incomplete();

} } // End namespaces test, boost.

#endif // #ifndef BOOST_POLICY_PTR_TEST_INCOMPLETE_HPP_INCLUDED
