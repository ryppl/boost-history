//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <stack>
#include <boost/assign/v2/conversion/check.hpp>
#include <boost/assign/v2/include/convert.hpp>
#include <boost/assign/v2/include/converter.hpp>
#include <boost/assign/v2/include/csv_deque_basic.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <boost/assign/v2/support/check/equal_container.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/multi_array.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/iterator_range.hpp>
#include <libs/assign/v2/test/conversion.h>

namespace test_assign_v2{
namespace xxx_conversion{

    // suggested by JB:
    //[test_conversion_f
    template<typename C, typename R>
    void f(C cont, R const& r)/*<-*/{
        namespace as2 = boost::assign::v2;
        as2::check_aux::equal_container( cont, r );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
    //]

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace ns = as2::check_aux;

        // External containers (fully qualified)
        {
            //[test_convert1
            typedef std::stack<int> C;

            C lifo; lifo.push( 3 ); lifo.push( 2 ); lifo.push( 1 );

            BOOST_ASSIGN_V2_CHECK(
                ( as2::csv_deque( 3, 1, 1 ) | as2::convert<C>() ) < lifo
            );
            //]

        }
        {
            //[test_converter1
            std::list<int> source( 3 ); boost::iota( source, 1 );

            std::queue<int> fifo = as2::converter( source );

            BOOST_ASSIGN_V2_CHECK( fifo.front() == 1 );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == 3 );
            //]
        }
        {
            //[test_converter2
            typedef int T; typedef std::vector<T> R;
            R benchmark( 10 ); boost::iota( benchmark, 0 );

            as2::result_of::converter<R>::type source( benchmark );

            {
                typedef boost::array<T, 10> C;
                f<C>( source, benchmark );
            }
            {
                typedef std::deque<T> C;
                f<C>( source, benchmark );
            }
            {
                typedef std::list<T> C;
                f<C>( source, benchmark );
            }
            {
                typedef std::queue<T> C;
                f<C>( source, benchmark );
            }
            {
                typedef std::stack<T> C;
                f<C>( source, benchmark );
            }
            {
                typedef std::vector<T> C;
                f<C>( source, benchmark );
            }
            //]
        }
        // Boost.Assign.2.0 containers - name lookup
        {
            //[test_converter3
            typedef boost::multi_array<int, 2> array2_;
            typedef array2_::size_type size_;
            typedef size_ const dim_;
            dim_ dim1 = 3, dim2 = 3;
            array2_ array2 = converter(
                as2::csv_deque(-1, +1, -1, +1, -1, +1, -1, +1, -1),
                boost::extents[dim1][dim2]
            );

            const int benchmark [] = {
                -1, +1, -1,
                +1, -1, +1,
                -1, +1, -1
            };
            size_ const n = array2.num_elements();
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    boost::make_iterator_range( array2.data(), n + array2.data() ),
                    boost::make_iterator_range( benchmark, n + benchmark )
                )
            );
            //]
        }
    }

}// xxx_conversion
}// xxx_test_assign

