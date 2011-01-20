//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_ITERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_ITERATOR_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/range/detail/join_iterator.hpp>
#include <boost/range/detail/demote_iterator_traversal_tag.hpp>
#include <boost/assign/v2/detail/traits/type/meta_convert.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace chain_aux{

	template<
    	typename It1, 
        typename It2,
        typename Tag
    >
	struct iterator
	{
        typedef typename boost::iterator_reference<It1>::type ref1_;
        typedef typename boost::iterator_reference<It2>::type ref2_;

        typedef assign::v2::type_traits::meta_convert<Tag> convert_;
        typedef typename ::boost::mpl::apply2<
            convert_,
            ref1_,
            ref2_
        >::type reference;
        
        typedef typename boost::remove_reference<reference>::type value_type;


    
    	typedef boost::range_detail::join_iterator<
        	It1
        	,It2
        	,value_type
        	,reference
            // , boost::single_pass_traversal_tag  
    	> type;

	};

}// chain_aux
}// v2
}// assign
}// boost

#endif
