// (C) Copyright Jonathan Turkanis 2004
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// This test should fail to compile for both static_move_ptr and 
// dynamic_move_ptr.

#include <boost/policy_ptr/smart_ptr.hpp>
#include <boost/policy_ptr/policy/move_copy.hpp>

// Subsequent headers use this typedef
typedef boost::smart_ptr<boost::move_copy> policy_ptr;

#include "../detail/hierarchy_virtual_dtor.hpp"
#include "../detail/source_sink.hpp"

void f(policy_ptr::to<base> ptr)
{
    policy_ptr::to<base> ptr2 = ptr;
}

int main()
{
    f(base_source());
}
