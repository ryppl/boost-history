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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/pipe/csv_put.hpp>
// Options come next
#include <boost/assign/v2/option/data_generator.hpp>
#include <boost/assign/v2/option/modifier/mapped.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/typeof/typeof.hpp>

#include <libs/assign/v2/test/pipe/option/mapped.h>

namespace test_assign_v2{
namespace xxx_pipe{
namespace xxx_option{
namespace xxx_mapped{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_pipe_option_mapped
            typedef std::map<std::string, int> C; 
            C benchmark;
            benchmark["feb"] = 28;
            benchmark["apr"] = 30;
            benchmark["jun"] = 30;
            benchmark["sep"] = 30;
            benchmark["nov"] = 30;
            benchmark["jan"] = 31;
            benchmark["mar"] = 31;
            benchmark["may"] = 31;
            benchmark["jul"] = 31;
            benchmark["aug"] = 31;
            benchmark["oct"] = 31;
            benchmark["dec"] = 31;
            
            BOOST_AUTO( apply, as2::_csv_put % (as2::_data = as2::_key) );
            C cal; as2::put( cal )("feb", 28);
            using namespace lambda;
            BOOST_ASSIGN_V2_CHECK(
            	range::equal(
					cal 
                    	| ( apply % ( as2::_mapped = (_1 = 30) ) )( "apr", "jun", "sep", "nov" )
                    	| ( apply % ( as2::_mapped = (_1 = 31) ) )( "jan", "mar", "may", "jul", "aug", "oct", "dec" ),
                    benchmark
                )
            );
            //] 
        }    
        {
            //[test_pipe_option_mapped_ptr
            typedef boost::ptr_map<std::string, int> C; 
            C benchmark;
            benchmark["feb"] = 28;
            benchmark["apr"] = 30;
            benchmark["jun"] = 30;
            benchmark["sep"] = 30;
            benchmark["nov"] = 30;
            benchmark["jan"] = 31;
            benchmark["mar"] = 31;
            benchmark["may"] = 31;
            benchmark["jul"] = 31;
            benchmark["aug"] = 31;
            benchmark["oct"] = 31;
            benchmark["dec"] = 31;
            
            C cal; as2::put( cal )("feb", 28);
            BOOST_AUTO( apply, as2::_csv_put % (as2::_data = as2::_key) );
            using namespace lambda;
            BOOST_ASSIGN_V2_CHECK(
            	range::equal(
					cal 
                    	| ( apply % ( as2::_mapped = (_1 = 30) ) )( "apr", "jun", "sep", "nov" )
                    	| ( apply % ( as2::_mapped = (_1 = 31) ) )( "jan", "mar", "may", "jul", "aug", "oct", "dec" ),
                    benchmark
                )
            );
            //] 
        }    
    }

}// xxx_mapped
}// xxx_option
}// xxx_pipe
}// test_assign_v2
