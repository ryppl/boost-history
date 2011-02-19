//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <list>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>
#include <libs/assign/v2/test/put/pipe/functor/container.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_csv{
namespace xxx_container{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        //[csv_pipe_list
        typedef int T; T x = 1, y = 2, z = 0;
        std::list<T> seq;
        seq | as2::_csv_put( x, y, z );
        BOOST_ASSIGN_V2_CHECK( seq.front() == x );
        BOOST_ASSIGN_V2_CHECK( seq.back() == z );
        //]
    }

}// xxx_container
}// xxx_csv
}// xxx_pipe
}// xxx_put
}// xxx_test_assign
