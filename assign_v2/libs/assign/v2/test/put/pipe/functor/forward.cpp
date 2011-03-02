//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <vector>
#include <string>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/ref/list_tuple/container.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/pipe/functor/forward.hpp>
#include <libs/assign/v2/test/put/pipe/functor/forward.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_forward{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace ppx = as2::put_pipe_aux;
        {
            typedef int T;
            T x = 1, y = 2, z = 0;
            typedef std::vector<T> C; C cont;
            typedef as2::result_of::put<C>::type adapter_;
            adapter_ adapter = as2::put( cont );
            ppx::forward( adapter, as2::ref::list_tuple( x )( y )( z )() );
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[1] == y );
            BOOST_ASSIGN_V2_CHECK( cont[2] == z );
            BOOST_ASSIGN_V2_CHECK( cont.size() == 4 );
        }
        {
            typedef const char* T;
            typedef std::deque<T> C; C cont;
            typedef as2::result_of::put<C>::type adapter_;
            adapter_ adapter = as2::put( cont );
            ppx::forward( adapter, as2::ref::list_tuple( "x" ) );
            typedef std::string str_;
            BOOST_ASSIGN_V2_CHECK( str_( cont[0] ) == "x" );
        }
    }

}// xxx_forward
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2
