//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <queue>
#include <map>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/sub.hpp>
#include <libs/assign/v2/test/put/modulo.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modulo{

    void test()
    {
        using namespace boost::assign::v2;
        using namespace checking::constants;
        namespace chk_cont = checking::container;
        {	// Queue + Repeat
            typedef std::queue<int> cont_;
            cont_ cont;
            ( put( cont ) % ( _repeat = 3 ) )( -1 );
            BOOST_ASSIGN_V2_CHECK( cont.front() == -1); cont.pop();
            BOOST_ASSIGN_V2_CHECK( cont.front() == -1); cont.pop();
            BOOST_ASSIGN_V2_CHECK( cont.front() == -1); cont.pop();
            BOOST_ASSIGN_V2_CHECK( cont.empty() == true);
        }
    }

}// xxx_modulo
}// xxx_put
}// test_assign_v2


