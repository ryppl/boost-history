//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_MAP_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_MAP_ER_2010_HPP
#include <boost/range/begin.hpp>
#include <boost/utility.hpp>

#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/traits/container/is_map.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 

    // Assumes 
    // map[ a ] = a; ...; map[ h ] = h;
    template<typename V>
    void do_check(container_tag::map, V & map) // non-const due to operator[]
    {                                      
    	{  // Key                                
            using namespace checking::constants::sorted;
            do_check(
                boost::next( boost::begin( map ), 0 )->first,
                boost::next( boost::begin( map ), 1 )->first,
                boost::next( boost::begin( map ), 2 )->first,
                boost::next( boost::begin( map ), 3 )->first,
                boost::next( boost::begin( map ), 4 )->first,
                boost::next( boost::begin( map ), 5 )->first,
                boost::next( boost::begin( map ), 6 )->first,
                boost::next( boost::begin( map ), 7 )->first
            );
        }
        {  // Data
            using namespace checking::constants;
            do_check(
            	map[ a ],
            	map[ b ],
            	map[ c ],
            	map[ d ],
            	map[ e ],
            	map[ f ],
            	map[ g ],
            	map[ h ]
            );
        }
    }                                           

}// checking
}// v2
}// assign
}// boost    
    
#endif
