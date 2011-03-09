//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <stack>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/check/equal_container.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/conversion.hpp>
#include <boost/assign/v2/utility/conversion/check.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <libs/assign/v2/test/utility/conversion.h>


namespace test_assign_v2{
namespace xxx_utility{
namespace xxx_conversion{

    // suggested by JB:
    //[convert_f
    template<typename C, typename R>
    void f(C cont, R const& r){
        namespace as2 = boost::assign::v2;
        as2::check_aux::equal_container( cont, r );
    }
	//]
    
    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace ns = as2::check_aux;

        // Non-Boost.Assign.2.0 containers - fully qualified as2::converter()
        {
            //[convert_inpl
            std::vector<int> v( 3 ); v[0] = 1; v[1] = 2; v[2] = 0;
            typedef boost::array<int, 3> ar_;
            BOOST_ASSIGN_V2_CHECK(
                ( as2::converter( v ).type<ar_>() )[1] == v[1]
            );
            //]
            BOOST_ASSIGN_V2_CHECK(
                ( as2::converter( v ).type<ar_>() )[0] == v[0]
            );
            BOOST_ASSIGN_V2_CHECK(
                ( as2::converter( v ).type<ar_>() )[2] == v[2]
            );
        }
        {
            //[convert_copy
            std::vector<int> v( 3 ); v[0] = 72; v[1] = 31; v[2] = 48;
            std::stack<int> lifo = as2::converter( v );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
        {
            {
            	typedef int T;
            	typedef std::vector<T> R; R r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48; 
                typedef std::vector<T> C; f<C>( as2::converter( r ), r );
            }
            {
            	typedef int T;
            	typedef std::vector<T> R; R r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48; 
                typedef std::deque<T> C; f<C>( as2::converter( r ), r );
            }
            {
            	typedef int T;
            	typedef std::vector<T> R; R r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48; 
                typedef std::list<T> C; f<C>( as2::converter( r ), r );
            }
            {
            	//[convert_f_invoke
            	typedef int T;
            	typedef std::vector<T> R; R r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48; 
                typedef std::stack<T> C; f<C>( as2::converter( r ), r );
                //]
            }
            {
            	typedef int T;
            	typedef std::vector<T> R; R r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48; 
                typedef std::queue<T> C; f<C>( as2::converter( r ), r );
            }
        }
        
        // Non-Boost.Assign.2.0 containers - name lookup
        {
            //[convert_array
            std::stack<int> lifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::ref::array( 72 )( 31 )( 48 ) );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
        {
            //[convert_csv_array
            std::stack<int> lifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::ref::csv_array( 72, 31, 48 ) );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
        {
            //[convert_deque
            std::stack<int> lifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::deque<int>( 72 )( 31 )( 48 ) );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
        {
            //[convert_csv_array
            std::stack<int> lifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::csv_deque( 72, 31, 48 ) );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
    }

}// xxx_conversion
}// xxx_utility
}// xxx_test_assign

