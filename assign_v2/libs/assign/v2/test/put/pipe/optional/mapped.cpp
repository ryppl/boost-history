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
#include <boost/assign/v2/put/pipe/csv_put.hpp>
// Options come next
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/option/mapped.hpp>
#include <libs/assign/v2/test/put/pipe/option/mapped.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_option{
namespace xxx_mapped{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_put_pipe_modifier_mapped
            using namespace lambda;
            typedef std::map<std::string, int> C; C cal;
            BOOST_AUTO( _local, ( as2::_data = _1 ) );
            BOOST_ASSIGN_V2_CHECK(
                (
                    cal 
                        | as2::_csv_put( C::value_type( "feb", 28 ) ) 
                        | ( as2::_csv_put % _local % ( as2::_mapped = (_1 = 30) ) )( "apr", "jun", "sep", "nov" )
                        | ( as2::_csv_put % _local % ( as2::_mapped = (_1 = 31) ) )( "jan", "mar", "may", "jul", "aug", "oct", "dec" )
 
                )["feb"] == 28
            );
            BOOST_ASSIGN_V2_CHECK( cal["jun"] == 30 );
            BOOST_ASSIGN_V2_CHECK( cal["mar"] == 31 );
            //] 
        }    
    }

}// xxx_mapped
}// xxx_option
}// xxx_pipe
}// xxx_put
}// test_assign_v2
