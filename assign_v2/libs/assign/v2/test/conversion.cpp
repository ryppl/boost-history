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
#include <boost/assign/v2/conversion.hpp>
#include <boost/assign/v2/conversion/check.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/conversion.h>

namespace test_assign_v2{
namespace xxx_conversion{

    // suggested by JB:
    //[test_conversion_f
    template<typename C, typename R>
    void f(C cont, R const& r){
        namespace as2 = boost::assign::v2;
        as2::check_aux::equal_container( cont, r );
    }
    //]
    
    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        namespace ns = as2::check_aux;

        // External containers (fully qualified)
        {
            //[test_conversion_vec_array
            std::vector<int> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;
            typedef array<int, 3> ar_; 
            ar_ const& ar = ( r | as2::_convert<ar_>() );
            
            BOOST_ASSIGN_V2_CHECK( range::equal( ar, as2::csv_deque( 72, 31, 48 ) ) );
            //]
        }
        {
            //[test_conversion_vec_stack
            std::vector<int> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;
            std::stack<int> lifo = as2::converter( r );
            
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
        {
            //[test_conversion_stl
            typedef int T; typedef std::vector<T> R; R r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48; 
            f< std::vector<T> >( as2::converter( r ), r );
            f< std::deque<T> >( as2::converter( r ), r );
            f< std::list<T> >( as2::converter( r ), r );
            f< std::stack<T> >( as2::converter( r ), r );
            f< std::queue<T> >( as2::converter( r ), r );
            //]
        }
        {
            //[test_conversion_matrix3x3
            const int sz = 3; typedef array<int, sz>  row_;
            array<row_, sz>  matrix3x3 = converter(
                as2::ref::array
                    ( as2::ref::csv_array( 1, 2, 3 ) | as2::_convert<row_>() )
                    ( as2::ref::csv_array( 4, 5, 6 ) | as2::_convert<row_>() )
                    ( as2::ref::csv_array( 7, 8, 9 ) | as2::_convert<row_>() )
            );
            
            for(int i = 0; i < 9; i++)
            {
                BOOST_ASSIGN_V2_CHECK( matrix3x3[ i / 3 ][ i % 3 ] == i + 1 );
            }
            //]
        }
        // Boost.Assign.2.0 containers - name lookup
        {
            //[test_conversion_as2_deque_array
            std::vector<int> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;
            typedef array<int, 3> ar_; 
            ar_ const& ar = ( as2::csv_deque( 72, 31, 48 ) | as2::_convert<ar_>() );
            
            BOOST_ASSIGN_V2_CHECK( range::equal( ar, as2::csv_deque( 72, 31, 48 ) ) );
            //]
        }
        {
            //[test_conversion_ref_array_stack
            std::stack<int> lifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::ref::array( 72 )( 31 )( 48 ) );
            
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
        {
            //[test_conversion_ref_array_queue
            std::queue<int> fifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::ref::csv_array( 72, 31, 48 ) );
            
            BOOST_ASSIGN_V2_CHECK( fifo.front() == 72 );
            //]
        }
        {
            //[test_conversion_as2_deque_stack
            std::stack<int> lifo = /*<<Notice unqualified (name lookup)>>*/converter( as2::csv_deque( 72, 31, 48 ) );
            
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 48 );
            //]
        }
    }

}// xxx_conversion
}// xxx_test_assign

