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
#include <deque>
#include <string>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/traits/container.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
#include <boost/assign/v2/pipe/csv_put.hpp> 
#include <boost/ptr_container/ptr_map.hpp>/
#include <boost/range/algorithm/equal.hpp>

#include <libs/assign/v2/test/pipe/csv_put.h>

namespace test_assign_v2{
namespace xxx_pipe{
namespace xxx_csv_put{ 

    void test(){
    
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_pipe_csv_put_str_literal
            typedef const char* T; typedef std::string str_; 
            std::deque<T> cont;

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal(
                    cont | /*<<`"x"`, `"y"` and `"z"` are kept as `const char(&)[2]`>>*/as2::_csv_put( "x", "y", "z" ),
                    as2::csv_deque<str_>( "x", "y", "z" )
                )
            );
            //]
        }
        {
            //[test_pipe_csv_put_map
            typedef std::map<std::string, int> C; typedef C::value_type T;
            C cal; C benchmark; 
            benchmark["jan"] = 31;
            benchmark["feb"] = 28;
            benchmark["mar"] = 31;
            
            cal  | as2::_csv_put( T("jan", 31), T( "feb", 28 ), T("mar", 31) );
            
            BOOST_ASSIGN_V2_CHECK( 
                range::equal(
                    cal  | as2::_csv_put( T("jan", 31), T("feb", 28 ), T("mar", 31) ),
                    benchmark
                )
            );
            //]
        }
        {
            //[test_pipe_csv_put_map_ptr
            typedef boost::ptr_map<std::string, int> C; 
            typedef as2::value_container_value<C>::type T; 

            C benchmark; 
            benchmark["jan"] = 31;
            benchmark["feb"] = 28;
            benchmark["mar"] = 31;
            
            C cal; 
            BOOST_ASSIGN_V2_CHECK( 
                range::equal(
                    cal  | as2::_csv_put( T("jan", 31), T("feb", 28 ), T("mar", 31) ),
                    benchmark
                )
            );
            //]
        }

    }// test()

}// xxx_csv_put
}// xxx_pipe
}// xxx_test_assign
