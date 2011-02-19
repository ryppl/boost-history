//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERT_DISPATCH_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERT_DISPATCH_ER_2010_HPP
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/assign/v2/put/pipe/range.hpp>

#include <boost/assign/v2/utility/convert/tag.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace convert_aux{

    template<typename T, typename U>
    T dispatch(U const& u, convert_tag::put) 
    {
        T t; 
        return (t | v2::_put_range( u ) );

    }

    template<typename T, typename U>
    T dispatch(U const& u, convert_tag::copy)
    {
        return T( boost::begin( u ), boost::end( u ) );
    }

}// convert_aux
}// v2
}// assign
}// boost

#endif
