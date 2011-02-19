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
#include <map>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <libs/assign/v2/test/put/container/csv.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_container{
namespace xxx_csv{

        void test()
        {
            namespace as2 = boost::assign::v2;
            {
                //[csv_list
                typedef int T; T x = 1, y = 2, z = 0;
                std::list<T> seq1, seq2;
                ( as2::put( seq1 ) )( x )( y )( z );
                ( as2::csv_put( seq2 ) )( x, y, z );
                BOOST_ASSIGN_V2_CHECK( seq1 == seq2 );
                //]
            }
            {
                //[csv_map
                typedef std::map<std::string, int> cont_;
                typedef cont_::value_type type; cont_ cont;
                as2::csv_put( cont )( 
                    type("jan", 31), type("feb", 28), type("mar", 31)
                );                
                BOOST_ASSIGN_V2_CHECK( cont["feb"] == 28 );
                //]
                BOOST_ASSIGN_V2_CHECK( cont["jan"] == 31 );
                BOOST_ASSIGN_V2_CHECK( cont["mar"] == 31 );
            }
            {
                //[ptr_vector
                typedef int T; T x = 1, y = 2, z = 0;
                boost::ptr_vector<T> seq1, seq2;
                as2::csv_put( seq1 )( x, y, z );
                as2::put( seq2 )( x )( y )( z );
                BOOST_ASSIGN_V2_CHECK( seq1 == seq2 );
                //]
            }
            // TODO ptr_map
        }

}// xxx_csv
}// xxx_container
}// xxx_put
}// test_assign_v2
