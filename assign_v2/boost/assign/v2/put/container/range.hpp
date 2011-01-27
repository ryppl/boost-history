//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_RANGE_ER_2010_HPP
#include <boost/range/algorithm/for_each.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

namespace boost{
namespace assign{
namespace v2{

    template<typename C, typename R>
    C& put_range(C& cont, R const& r)
    {
        return ::boost::for_each( r, put( cont ) ).container();
    }

    template<typename C, typename R>
    C put_range( R const& r )
    {
		C cont;
        return put_range( cont, r );
    }

}// v2
}// assign
}// boost

#endif
