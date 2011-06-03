//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERSION_CONVERTER_ER_2011_HPP
#define BOOST_ASSIGN_V2_CONVERSION_CONVERTER_ER_2011_HPP
#include <boost/assign/v2/support/pp/ignore.hpp>
#include <boost/assign/v2/conversion/convert.hpp>
#include <boost/call_traits.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_conversion_converter
namespace conversion_aux{

    template<typename R, typename Arg = kwd_nil_>
    class converter
    {
//<-
        typedef boost::iterator_range<
            typename boost::range_iterator<
                typename boost::add_const<R>::type
            >::type
        > source_type;
//->

        public:

        explicit converter(typename call_traits<R>::param_type source)/*<-*/
             : source_( boost::make_iterator_range( source ) )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        converter(
            typename call_traits<R>::param_type source,
            Arg const& arg
        )/*<-*/
             : arg_( arg ),
             source_( boost::make_iterator_range( source ) )
        {}BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        // Implicit conversion
        template<typename C>
        operator C() const/*<-*/
        {
            return this->type<C>();
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

        // Explicit converion
        template<typename C> C type()const/*<-*/
        {
            typedef convert<C, Arg> convert_;
            return this->source_ | convert_( this->arg_ );
        }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
        private:
        Arg arg_;
        source_type source_;
//->
    };

}// conversion_aux
namespace result_of{

    template<typename R, typename Arg = kwd_nil_>
    struct converter/*<-*/
    {
        typedef conversion_aux::converter<R, Arg> type;
    }/*->*/;

}//result_of

    template<typename R>
    typename result_of::converter<R>::type
    converter(R const& r)/*<-*/
    {
        typedef typename result_of::converter<R>::type result_;
        return result_( r );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename R, typename Arg>
    typename result_of::converter<R, Arg>::type
    converter(R const& r, Arg const& arg)/*<-*/
    {
        typedef typename result_of::converter<R, Arg>::type result_;
        return result_( r, arg );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//]
}// v2
}// assign
}// boost

#if defined( BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM ) || defined ( BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP )
#error
#endif

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM(s, data, T) typename T
#define BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP(Seq, R)\
    template<BOOST_PP_SEQ_ENUM(\
        BOOST_PP_SEQ_TRANSFORM(\
            BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM,\
            ~,\
            Seq\
        )\
    )>\
    typename ::boost::assign::v2::result_of::converter<R>::type \
    converter( R const& range )\
    {\
        return ::boost::assign::v2::converter( range );\
    }\
    template<BOOST_PP_SEQ_ENUM(\
        BOOST_PP_SEQ_TRANSFORM(\
            BOOST_ASSIGN_V2_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM,\
            ~,\
            Seq\
        )\
    ), typename Arg>\
    typename ::boost::assign::v2::result_of::converter<R, Arg>::type \
    converter(R const& range, Arg const& arg )\
    {\
        return ::boost::assign::v2::converter( range, arg );\
    }\
/**/


#endif // BOOST_ASSIGN_V2_CONVERSION_CONVERTER_ER_2011_HPP
