//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_ASSIGN_V2_TEST_PUT_COMPOSE_CSV_COMPOSER_ER_2010_H
#define LIBS_ASSIGN_V2_TEST_PUT_COMPOSE_CSV_COMPOSER_ER_2010_H
#include <boost/mpl/int.hpp>
#include <boost/assign/v2.hpp>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_compose{
namespace xxx_csv{
namespace xxx_composer{

        void test()
        {	
        	using namespace boost::assign::v2;
            int a1, b1, c1, d1;//, e1, f1, g1, h1;
            using namespace checking::constants;
            {
                a1 = a; b1 = b; c1 = c; d1 = d;
            }

            using namespace adaptor;
            BOOST_AUTO(
            	tmp,
                ( _csv_put( a1, b1, c1 ) )
            );
            typedef boost::mpl::int_<0> zero_;
            typedef boost::mpl::int_<1> one_;
            typedef boost::mpl::int_<2> two_;
            {
            	int& x = tmp.sequence_arg()[ zero_::value ];
            	BOOST_ASSIGN_V2_CHECK_EQUAL( &a1, &x );
            	BOOST_ASSIGN_V2_CHECK_EQUAL( a, x );
            }
            {
            	int& x = tmp.sequence_arg()[ one_::value ];
            	BOOST_ASSIGN_V2_CHECK_EQUAL( &b1, &x );
            	BOOST_ASSIGN_V2_CHECK_EQUAL( b, x );
            }
            {
            	int& x = tmp.sequence_arg()[ two_::value ];
            	BOOST_ASSIGN_V2_CHECK_EQUAL( &c1, &x );
            	BOOST_ASSIGN_V2_CHECK_EQUAL( c, x );
            }

        }
 
}// xxx_composer
}// xxx_csv
}// xxx_compose
}// xxx_put
}// xxx_test_assign

#endif
