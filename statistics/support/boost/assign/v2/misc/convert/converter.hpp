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
#include <boost/assign/v2/misc/convert/dispatch.hpp>
#include <boost/assign/v2/misc/convert/deduce.hpp>

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
        	return type<T>();
        }

		template<typename T>
        T type()const
        {
        	typedef typename convert_aux::deduce_tag<T, U>::type tag_;
            return convert<T>( this->w.unwrap(), tag_() );
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

#endif
