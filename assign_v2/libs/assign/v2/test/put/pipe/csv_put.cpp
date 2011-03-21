//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
//#include <map>
#include <deque>
#include <vector>
#include <list>
#include <string>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp> 
#include <boost/range/algorithm/for_each.hpp>

#include <libs/assign/v2/test/put/pipe/csv_put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_csv_put{ 

    void test(){
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_value_pipe_put_str_literal
            typedef const char* T; typedef std::string str_; std::deque<T> cont;

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal(
                    cont | /*<<`"x"`, `"y"` and `"z"` are kept as `const char(&)[2]`>>*/as2::_csv_put( "x", "y", "z" ),
                    as2::csv_deque<str_>( "x", "y", "z" )
                )
            );
            //]
        }
    }// test()

}// xxx_csv_put
}// xxx_pipe
}// xxx_value
}// xxx_test_assign