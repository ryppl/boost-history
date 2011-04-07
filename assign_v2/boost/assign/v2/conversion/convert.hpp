//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CONVERSION_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_V2_CONVERSION_CONVERT_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/detail/keyword.hpp>
#include <boost/assign/v2/put/csv_put.hpp>
#include <boost/range/algorithm/for_each.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_conversion_convert
namespace convert_tag{

    struct put{};
    struct copy{};

}// convert_tag
//<-
namespace conversion_aux{

    template<typename C, typename R>
    C csv_put_copy(C& cont, R const& range)
    {
        csv_put( cont, as_arg_list( range ) );
        return cont;
    }

	template<typename C, typename Arg>
    Arg const& initializer( Arg const& arg )
    {
    	return arg;
    }

    template<typename C, typename Arg, typename R>
    C dispatch(Arg const& arg, R const& range, convert_tag::put)
    {
        C cont( initializer<C>( arg ) ); 
        return csv_put_copy( cont, range );
    }

    template<typename C, typename R>
    C dispatch(nil_, R const& range, convert_tag::put)
    {
        C cont; return csv_put_copy( cont, range );
    }

    template<typename C, typename R>
    C dispatch(nil_, R const& r, convert_tag::copy)
    {
        return C( boost::begin( r ), boost::end( r ) );
    }

    template<typename C, typename R>
    struct deduce_tag;

    template<typename C, typename Arg, typename R>
    C dispatch(Arg const& arg, R const& r)/*<-*/
    {
        typedef typename deduce_tag<C, R>::type tag_;
        return dispatch<C>( arg, r, tag_() );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/


//->

    template<typename C, typename Arg = nil_>
    struct convert/*<-*/{
    
    	convert(){}
        convert(Arg arg):arg_( arg ){}
        
        Arg const& arg()const{ return this->arg_; }
        
        private:
        mutable Arg arg_;
    
    }/*->*/;
    
    template<typename R, typename C, typename Arg>
    C operator|( R const& r, convert<C, Arg> const& adapter)/*<-*/
    {
        return dispatch<C>( adapter.arg(), r );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// conversion_aux

	template<typename C, typename Arg = nil_>
	struct convert/*<-*/
    	: conversion_aux::convert<C, Arg>
    {
    
    	typedef conversion_aux::convert<C> super_t;
        
    	convert()/*<-*/{}/*->*/
        convert(Arg const& arg)
        	: super_t( arg )
        {}
                
        template<typename Arg1>
        convert<C, Arg1>
        operator+=(Arg1 const& arg1)
        {
        	return convert<C, Arg1>( arg1 );
        }
    
    }/*->*/;

//]
}// v2
}// assign
}// boost

#include <boost/assign/v2/conversion/deduce_tag.hpp>

#endif //  BOOST_ASSIGN_V2_CONVERSION_CONVERT_ER_2010_HPP
