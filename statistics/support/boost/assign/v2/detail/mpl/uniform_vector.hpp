//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_MPL_UNIFORM_VECTOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_MPL_UNIFORM_VECTOR_ER_2010_HPP
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>

// TODO remove file

namespace boost{
namespace assign{
namespace v2{
namespace mpl{
namespace uniform_vector_aux{

	template<std::size_t N,typename Seq, typename T>
    struct cont : cont< 
    	N-1,
    	typename boost::mpl::push_back<
    		Seq,
            T
        >::type,
        T
    >{};

	template<typename Seq,typename T>
	struct cont<0, Seq, T>{
		typedef Seq type;    
    };

}// uniform_vector_aux

template<std::size_t N, typename T>
struct uniform_vector : uniform_vector_aux::cont<
	N,
    boost::mpl::vector0<>,
    T
>{};

}// mpl
}// v2
}// assign
}// boost

#endif
