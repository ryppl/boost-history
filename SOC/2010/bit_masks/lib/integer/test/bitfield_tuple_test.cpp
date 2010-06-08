//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include "test_type_list.hpp"
void ignore(...) {}

struct red;
int main() {
    // lets make some errors : )
    // bitfield_tuple < storage<int>, member<int,red,6> > temp;
    // test suite for the storage
    {
        typedef storage<int, std::allocator<int> > test1;
        test1 s;
        ignore( s );
    }
    
#if 0
    details::bft_impl_< storage<int, storage_policy_stack>,
                        member<int, red, 6u>,
                        mpl::void_,
                        mpl::void_,
                        mpl::void_,
                        mpl::void_,
                        mpl::void_,
                        mpl::void_,
                        mpl::void_,
                        mpl::void_ > temp;
    // >

    ignore(&temp);
#endif
    return 0;
}
