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
#include <map>
#include <string>
#include <boost/assign/v2/detail/config/check.hpp>
//#include <boost/assign/v2/csv.hpp>
#include <boost/assign/v2/deque.hpp>
// Options come after
#include <boost/assign/v2/option/data_generator.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/assign/v2/put/csv_put.hpp> // Really?
#include <boost/assign/v2/put/put.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/csv.h>

namespace test_assign_v2{
namespace xxx_csv{

    void test(){

        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_csv_put
            std::deque<int> cont;    
            as2::csv_put( cont, 1, 10, 100 );

            BOOST_ASSIGN_V2_CHECK( range::equal( cont, as2::csv_deque( 1, 10, 100 ) ) );
            //]
        }
        {
            //[test_csv_put_map
            typedef std::map<std::string, int> C; typedef C::value_type T;
            C cont;

            as2::csv_put( cont, T("jan", 31), T("feb", 28), T("mar", 31) );

            BOOST_ASSIGN_V2_CHECK( cont["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( cont["mar"] == 31 );
            //]    
        }
        {
            //[test_csv_put_modulo
            std::deque<int> cont;
            as2::csv_put( cont, as2::_push_front, 100, 10, 1 );

            BOOST_ASSIGN_V2_CHECK( 
                range::equal( cont, as2::csv_deque( 1, 10, 100 ) ) 
            );
            //]
        }
        /*{
            //[test_csv_deque_modulo
            BOOST_AUTO(    
                cont,
                csv( 
                    as2::deque<int>( as2::_nil ) % as2::_push_front, 
                    1, 10, 100 
                )
            );

            BOOST_ASSIGN_V2_CHECK( 
                range::equal( cont, as2::csv_deque( 1, 10, 100 ) ) 
            );
            //]
        }*/
    }

}// xxx_csv
}// xxx_test_assign
