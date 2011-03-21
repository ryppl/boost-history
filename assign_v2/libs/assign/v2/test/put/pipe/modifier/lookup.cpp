//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <map>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/fun.hpp>
#include <boost/assign/v2/modifier/lookup.hpp>
#include <boost/assign/v2/value/pipe/put.hpp>
#include <boost/assign/v2/value/pipe/csv_put.hpp>
#include <libs/assign/v2/test/value/pipe/modifier/lookup.h>

namespace test_assign_v2{
namespace xxx_value{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_lookup{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[pipe_lookup
        	using namespace lambda;
            std::map<std::string, int> cal;
            BOOST_AUTO( _local, ( as2::_fun = _1 ) );
            BOOST_ASSIGN_V2_CHECK(
                (
                    cal 
                        | as2::_put( "feb", 28 ) 
                        | ( as2::_csv_put % _local % ( as2::_lookup = (_1 = 30) ) )( "apr", "jun", "sep", "nov" )
                        | ( as2::_csv_put % _local % ( as2::_lookup = (_1 = 31) ) )( "jan", "mar", "may", "jul", "aug", "oct", "dec" )
 
                )["feb"] == 28
            );
            BOOST_ASSIGN_V2_CHECK( cal["jun"] == 30 );
            BOOST_ASSIGN_V2_CHECK( cal["mar"] == 31 );
            //] 
        }    
    }

}// xxx_lookup
}// xxx_modifier
}// xxx_pipe
}// xxx_value
}// test_assign_v2
