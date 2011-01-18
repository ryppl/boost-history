//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_ITERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_ITERATOR_ER_2010_HPP
#include <boost/next_prior.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 
namespace iterator{
    
    template<typename It>
    void do_check(It it)
    {
    	namespace ns = checking::constants;                                           
        ns::do_check(
            *boost::next( it, 0 ),
            *boost::next( it, 1 ),
            *boost::next( it, 2 ),
            *boost::next( it, 3 ),
            *boost::next( it, 4 ),
            *boost::next( it, 5 ),
            *boost::next( it, 6 ),
            *boost::next( it, 7 )
        );
    }                                                                              

namespace sorted{

    template<typename It>
    void do_check(It it)
    {                                                                              
        namespace ns = checking::constants::sorted;                                           
        ns::do_check(
            *boost::next( it, 0 ),
            *boost::next( it, 1 ),
            *boost::next( it, 2 ),
            *boost::next( it, 3 ),
            *boost::next( it, 4 ),
            *boost::next( it, 5 ),
            *boost::next( it, 6 ),
            *boost::next( it, 7 )
        );
    }                                                                              

}// sorted
}// iterator
}// checking
}// v2
}// assign
}// boost    
    
#endif
