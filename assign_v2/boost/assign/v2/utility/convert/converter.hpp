//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERT_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERT_CONVERTER_ER_2010_HPP
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/assign/v2/utility/convert/dispatch.hpp>
#include <boost/assign/v2/utility/convert/deduce_tag.hpp>

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
            return convert_aux::dispatch<C>( this->w.get() );
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

// Define name lookup for name
// namespace ns{
//     template<typename A, typename B, typename C> cont;
// }
// is done by expanding this macro:
// #define SEQ (A)(B)(C)
// namespace ns{
//     BOOST_ASSIGN_V2_CONVERTER( cont<A,B,C>, SEQ )
// }
//

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#ifndef BOOST_ASSIGN_V2_CONVERTER_OP
#define BOOST_ASSIGN_V2_CONVERTER_OP(s, data, T) typename T
#endif
#ifndef BOOST_ASSIGN_V2_CONVERTER

// Expanding
//  namespace ns{
//   BOOST_ASSIGN_V2_CONVERTER( cont<A,B,C>, (A)(B)(C) )
//  }
// creates a name-lookup version of converter() for name ns::cont<A, B, C>

#define BOOST_ASSIGN_V2_CONVERTER(U, Seq)\
    template<BOOST_PP_SEQ_ENUM(\
        BOOST_PP_SEQ_TRANSFORM(\
            BOOST_ASSIGN_V2_CONVERTER_OP,\
            ~,\
            Seq\
        )\
    )>\
    typename ::boost::assign::v2::result_of::converter< U >::type \
    converter( U const& cont)\
    {\
        typedef typename ::boost::assign::v2::result_of::converter< U >::type result_; \
        return result_( cont );\
    }\
/**/
#endif

#endif
