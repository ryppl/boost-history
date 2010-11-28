//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/constants.h>
#include <libs/assign/v2/example/anon.h>

namespace example_assign_v2{
namespace xxx_anon{

	void run(std::ostream& os)
    {
		using namespace boost::assign::v2;
        os << "---xxx_anon " << std::endl;
        {
			os << "csv_anon" << ' ';
            //[csv_anon
            boost::for_each( csv_anon<T>( x, y ,z ), printer(os) );
            // outputs (1,0)(NULL,0)(1,5)
            //]
            os << std::endl;
			os << "anon" << ' ';
            //[anon
            boost::for_each( 
                (anon<T>( _nil ) % _push_front)( a )()( a, b ), 
                printer(os) 
            );
            // outputs (1,5)(NULL,0)(1,0)
            //]
        }
        os << std::endl;
    }

}// xxx_anon
}// example_assign_v2
