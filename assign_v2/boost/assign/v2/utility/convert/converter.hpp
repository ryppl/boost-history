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
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/assign/v2/utility/convert/dispatch.hpp>
#include <boost/assign/v2/utility/convert/deduce.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace convert_aux{

	template<typename U>
    class converter
    {
    
     	typedef typename ref::copy_wrapper<
        	U const
        >::type wrapper_;
        
        public:
        
    	explicit converter(U const& u) : w( u ){}
                
        template<typename T>
        operator T () const
        {
        	return this->type<T>();
        }

		template<typename T>
        T type()const
        {
        	typedef typename convert_aux::deduce_tag<T, U>::type tag_;
            return convert_aux::dispatch<T>( this->w.get(), tag_() );
        }

		private:
        wrapper_ w;

	};

}// convert_aux

	template<typename U>
	convert_aux::converter<U> 
    converter(U const& u){ 
    	typedef convert_aux::converter<U> result_; 
    	return result_( u ); 
    }

}// v2
}// assign
}// boost

// Define name lookup for name
// namespace ns{
// 	template<typename A, typename B, typename C> cont;
// }
// is done by expanding this macro:
// #define SEQ (A)(B)(C) 
// namespace ns{
// 	BOOST_ASSIGN_V2_CONVERTER( cont<A,B,C>, SEQ )
// }
// 

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>

#ifndef BOOST_ASSIGN_V2_CONVERTER_OP
#define BOOST_ASSIGN_V2_CONVERTER_OP(s, data, T) typename T
#endif
#ifndef BOOST_ASSIGN_V2_CONVERTER
#define BOOST_ASSIGN_V2_CONVERTER(U, Seq)\
	template<BOOST_PP_SEQ_ENUM(\
    	BOOST_PP_SEQ_TRANSFORM(\
        	BOOST_ASSIGN_V2_CONVERTER_OP,\
            ~,\
            Seq\
        )\
    )>\
    ::boost::assign::v2::convert_aux::converter< U > \
	converter( U const& cont)\
    {\
    	typedef convert_aux::converter< U > result_; \
    	return result_( cont );\
    }\
/**/    
#endif

#endif
