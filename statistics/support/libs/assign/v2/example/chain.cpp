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
#include <list>
#include <boost/array.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/constants.h>
#include <libs/assign/v2/example/chain.h>

namespace example_assign_v2{
namespace xxx_chain{

	void run(std::ostream& os)
    {
    	using namespace boost::assign::v2;
        os << "* xxx_chain" << std::endl;
		{
			os << "chain_read";
        	//[chain_read
            typedef boost::array<T, 1> cont1_; cont1_ cont1; cont1[0] = x;
            typedef std::list<T> cont3_; cont3_ cont3; cont3.push_back( z );
            using namespace adaptor;
            namespace ns = ref::assign_copy;
            boost::for_each(
                cont1 | _chain( ns::csv_anon( y ) ) | _chain( cont3 ),
                printer( os )
            );
            // outputs (1,0)(NULL,0)(1,5)
        	//]
            //[chain_write
            T y1;
            typedef std::vector<T> vec_; vec_ vec( 3, y );
            boost::copy(
                vec,
                boost::begin(
                    cont1 /* lvalue */ | _chain(
                        ns::csv_anon( y1 /* lvalue */ ) /* rvalue */
                    ) | _chain( cont3 /* lvalue */ )
                )
            );
            BOOST_ASSERT( cont1[0] == y );
            BOOST_ASSERT( y1 == y );
            BOOST_ASSERT( cont3.front() == y1 );
        	//]
        }
		{

        }

	}

}// xxx_chain
}// example_assign_v2
