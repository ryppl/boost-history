//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <list>
#include <boost/array.hpp>
#include <boost/assign/v2/chain.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/misc/chain.h>

namespace example_assign_v2{
namespace xxx_misc{
namespace xxx_chain{

    void run(std::ostream& os)
    {
        os << "* xxx_chain" << std::endl;
        {
            os << "chain_read ";
            //[chain_r
            boost::array<int, 2> ar; ar[0] = 0; ar[1] = 1;
            std::list<int> list( 1, 2 );
            std::vector<int> vec( 2 ); vec[0] = 3; vec[1] = 4;
            boost::for_each(
                ar | as2::_chain( list ) | as2::_chain( vec ),
                os << bl::_1 << ' '
            ); // outputs 0, 1, 2, 3, 4
            //]
        }
        {
            //[chain_w
            int const x = -1; int y; boost::array<int, 2> ar;
            boost::copy(
                std::vector<int>(3, x),
                boost::begin(
                    ar /* lvalue */ | as2::_chain(
                        as2::ref::csv_array( y /* lvalue */ )
                    ) /* rvalue */
                )
            );
            assert( ar[0] == x );
            assert( ar[1] == x );
            assert( y == x );
            //]
        }
        os << std::endl;
    }

}// xxx_chain
}// xxx_misc
}// example_assign_v2
