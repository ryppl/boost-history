// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// This test should compile for static_move_ptr, but fail to compile for 
// dynamic_move_ptr.

#include <boost/policy_ptr/smart_ptr.hpp>
#include <boost/policy_ptr/policy/auto_copy.hpp>
#include <boost/static_assert.hpp>

int main()
{
    BOOST_STATIC_ASSERT(
        sizeof(int*) 
            == 
        sizeof(boost::smart_ptr<boost::auto_copy>::to<int>)
    );
}
