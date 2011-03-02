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
#include <boost/assign/v2/utility/convert/deduce_tag.hpp>

namespace boost{
namespace assign{
namespace v2{
//[convert_dispatch
namespace convert_aux{

    template<typename C, typename R>
    C dispatch(R const& r, convert_tag::put)
    {
        C cont;
        return (cont | v2::_put_range( r ) );

    }

    template<typename C, typename R>
    C dispatch(R const& r, convert_tag::copy)
    {
        return C( boost::begin( r ), boost::end( r ) );
    }

	template<typename C, typename R>
    C dispatch(R const& r)
    {
        typedef typename convert_aux::deduce_tag<C, R>::type tag_;
        return convert_aux::dispatch<C>( r, tag_() );
    }

}// convert_aux
//]
}// v2
}// assign
}// boost

#endif
