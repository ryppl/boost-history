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
#include <deque>
#include <list>
#include <map>
#include <string>

#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <boost/assign/v2/put/pipe/csv.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>

#include <libs/assign/v2/example/include.h>

namespace example_assign_v2{
namespace xxx_put{
namespace xxx_pipe{

    void run(std::ostream& os)
    {
        os << "* xxx_pipe " << std::endl;
        namespace as2 = ::boost::assign::v2;
        namespace bl = ::boost::lambda;
        {
            //[vector
            std::vector<int> cont;
            boost::for_each(
                cont | as2::_csv_put( -1, 0, 1 ),
                os << bl::_1 << ' '
            ); //outputs -1 0 1
            //]
            os << std::endl;
        }
        {
            //[map
            typedef std::map<const char*, int> cont_;
            typedef cont_::value_type type;
            cont_ cont;
            assert(
                (
                    cont | as2::_csv_put(
                        type("x", -1),
                        type("y", 0),
                        type("z", 1)
                    )
                )["y"] == 0
            );
            //]
        }
        {
            //[map2
            std::map<const char*, int> cont;
            assert(
                (
                    cont | as2::_put("z", 1)("x", -1)("y", 0)
                )["y"] == 0
            );
            //]
        }
        {
            //[array
            boost::array<int,3> cont;
            boost::for_each(
                cont | as2::_csv_put( -1, 0, 1 ),
                os << bl::_1 << ' '
            ); // outputs -1 0 1
            //]
            os << std::endl;
        }
        {
            //[ptr_vector
            boost::ptr_vector<int> cont;
            boost::for_each(
                cont | as2::_csv_put( -1, 0, 1 ),
                os << bl::_1 << ' '
            ); // output -1 0 1
            //]
            os << std::endl;
        }
        {
            //[list
            std::list<int> cont;
            boost::for_each(
                cont | (
                    as2::_csv_put % as2::_push_front
                )( -1, 0, 1 ),
                os << bl::_1 << ' '
            ); // outputs 1 0 -1
            //]
            os << std::endl;
        }
        {
            //[fun
            std::vector<int> cont;
                cont | (
                    as2::_csv_put % (
                        as2::_fun = (2 * bl::_1)
                    )
                )( -1, 0, 1 );
/*
            boost::for_each(
                cont | (
                    as2::_csv_put % (
                        as2::_fun = (2 * bl::_1)
                    )
                )( -1, 0, 1 ),
                os << bl::_1 << ' '
            ); // outputs -2 0 2
*/
            //]
            os << std::endl;
        }
        {
            //[repeat
            std::vector<int> cont;
            boost::for_each(
                cont | (
                    as2::_csv_put % ( as2::_repeat = 2  )
                )( -1, 0, 1 ),
                os << bl::_1 << ' '
            ); // outputs -1 -1 0 0 1 1
            //]
            os << std::endl;
        }
    	{
            //[lookup
            std::map<const char*,int> cont;
            cont["x"] = -1; cont["y"] = 0; cont["z"] = 1;
            cont | (
                as2::_csv_put % ( as2::_lookup = ( bl::_1 +=2 ) )
            )( "z", "x", "y" );
            assert( cont["y"] == 2 );
            //]
        }
        {
        	//[iterate
            boost::array<int, 3> cont; cont[0] = -1;
            boost::for_each(
                cont | ( as2::_csv_put % ( as2::_iterate = 1  ) )( 0, 1 ),
                os << bl::_1 << ' '
            ); // outputs -1 0 1
            //]
            os << std::endl;
        }
    }

}// xxx_pipe
}// xxx_put
}// example_assign_v2
