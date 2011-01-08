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

#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <boost/range/algorithm/for_each.hpp>

#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/ref/array.h>

namespace example_assign_v2{
namespace xxx_ref{
namespace xxx_array{

    void run(std::ostream& os)
    {

		namespace as2 = boost::assign::v2;
        namespace bl = boost::lambda;
        
        os << "* xxx_ref_array " << std::endl;

        {
            //[csv_array_r
            boost::for_each(
                as2::ref::csv_array( "x", "y", "z" ),
                os << bl::_1 << ' '
            );
            //outputs x y z
            //]
            //[csv_array_w
            std::string x, y, z;
            as2::ref::csv_array( x, y, z ).assign( "w" );
            assert( y == "w" );
            //]
            //[csv_array_empty
                as2::ref::csv_array<int const>( as2::_nil );
            //]
        }
        {
            //[array_r
            boost::for_each(
                as2::ref::array( -1 )( 0 )( 1 ),
                os << bl::_1 << ' '
            );
            // outputs -1 0 1
            //]
		}

        os << std::endl;
    }

}// xxx_array
}// xxx_ref
}// example_assign_v2

