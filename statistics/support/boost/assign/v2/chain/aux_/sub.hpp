// TODO remove file
/*
//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2009 Neil Groves                                          //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_RANGE_CHAIN_AUX_SUB_ER_2010_HPP
#define BOOST_ASSIGN_V2_RANGE_CHAIN_AUX_SUB_ER_2010_HPP
#include <boost/range/iterator_range.hpp> //iterator_range_detail::
#include <boost/assign/v2/detail/relational_op.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace chain_aux{

    template<typename R>
    struct sub : R , relational_op_aux::crtp< sub<R> > 
    {

        typedef R super_t;

        sub(){}
        sub(super_t const& s):super_t( s ){}

        // Relational op
        template<typename R1>
        bool equal_to(const R1& r)const{
         	return ::boost::iterator_range_detail::equal(
            	(*this), r );
		}
		template<typename R1>
        bool less_than(const R& r)const{
         	return ::boost::iterator_range_detail::less_than(
            	(*this), r );
		}

    };

	template<typename R>
    sub<R> make_sub(const R& r){ return sub<R>( r ); }

}// chain_aux
}// v2
}// assign
}// boost

#endif
*/