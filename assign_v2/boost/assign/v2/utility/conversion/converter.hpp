//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/assign/v2/utility/conversion/convert.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits/add_const.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace convert_aux{

    template<typename R>
    class adapter
    {

        typedef typename boost::add_const<R>::type const_;

         typedef typename ref::copy_wrapper<
            const_
        >::type wrapper_;

        public:

        explicit adapter(const_& r) : w( r ){}

        template<typename C>
        operator C () const
        {
            return this->type<C>();
        }

        template<typename C>
        C type()const
        {
            return convert_aux::convert<C>( this->w.get() );
        }

        private:
        wrapper_ w;

    };

}// convert_aux
namespace result_of{

    template<typename R>
    struct converter
    {
        typedef convert_aux::adapter<R> type;
    };

}//result_of

    template<typename R>
    typename result_of::converter<R>::type
    converter(R const& r){
        typedef typename result_of::converter<R>::type result_;
        return result_( r );
    }

}// v2
}// assign
}// boost

#if defined( BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM ) || defined ( BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP )
#error
#endif

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM(s, data, T) typename T
#define BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP(Seq, R)\
    template<BOOST_PP_SEQ_ENUM(\
        BOOST_PP_SEQ_TRANSFORM(\
            BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP_PARAM,\
            ~,\
            Seq\
        )\
    )>\
    typename ::boost::assign::v2::result_of::converter<R>::type \
    converter( R const& range )\
    {\
        return ::boost::assign::v2::converter( range );\
    }\
/**/


#endif // BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_ER_2010_HPP
