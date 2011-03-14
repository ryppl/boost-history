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
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_utility_conversion_convert
namespace convert_tag{

    struct put{};
    struct copy{};

}// convert_tag
//<-
namespace conversion_aux{

    template<typename C, typename R>
    C convert(R const& r, convert_tag::put)
    {
        C cont;
        return ::boost::for_each( r, put( cont ) ).container();
    }

    template<typename C, typename R>
    C convert(R const& r, convert_tag::copy)
    {
        return C( boost::begin( r ), boost::end( r ) );
    }


	template<typename C, typename R>
    struct deduce_tag;
//->
    template<typename C, typename R>
    C convert(R const& r)/*<-*/
    {
        typedef typename conversion_aux::deduce_tag<C, R>::type tag_;
        return conversion_aux::convert<C>( r, tag_() );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename C>
    struct convert_adapter/*<-*/{}/*->*/;
    
    template<typename R, typename C>
    C operator|( R const& r, convert_adapter<C> )/*<-*/
    {
        return convert<C>( r );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// conversion_aux
using conversion_aux::convert;
namespace result_of{

    template<typename /*<<Container>>*/ C, typename /*<<Range>>*/R>
    struct convert/*<-*/{ typedef C type; }/*->*/;

}//result_of

template<typename C>
conversion_aux::convert_adapter<C> _convert()/*<-*/
{
    return conversion_aux::convert_adapter<C>();
}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]
}// v2
}// assign
}// boost

#include <boost/assign/v2/utility/conversion/deduce_tag.hpp> // Do not displace

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
