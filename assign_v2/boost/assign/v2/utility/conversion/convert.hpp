//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_ER_2010_HPP
#include <boost/assign/v2/put/container/range.hpp>
#include <boost/assign/v2/utility/conversion/deduce_tag.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace convert_aux{

    template<typename C, typename R>
    C convert(R const& r, convert_tag::put)
    {
        C cont;
        return v2::put_range( cont, r );
    }

    template<typename C, typename R>
    C convert(R const& r, convert_tag::copy)
    {
        return C( boost::begin( r ), boost::end( r ) );
    }

    template<typename C, typename R>
    C convert(R const& r)
    {
		typedef typename convert_aux::deduce_tag<C, R>::type tag_;
        return convert_aux::convert<C>( r, tag_() );
    }

}// convert_aux
using convert_aux::convert;
namespace result_of{

	template<typename /*<<Container>>*/ C, typename /*<<Range>>*/R>
    struct convert{ typedef C type; };

}//result_of
}// v2
}// assign
}// boost

#if defined( BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP_PARAM ) || defined ( BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP )
#error
#endif

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP_PARAM(s, data, T) typename T
#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP(Seq, R)\
    template<typename C, BOOST_PP_SEQ_ENUM(\
        BOOST_PP_SEQ_TRANSFORM(\
            BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP_PARAM,\
            ~,\
            Seq\
        )\
    )>\
    typename ::boost::assign::v2::result_of::convert<C, R>::type \
    convert( R const& range )\
    {\
        return ::boost::assign::v2::convert<C>( range );\
    }\
/**/

#endif //  BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_ER_2010_HPP
