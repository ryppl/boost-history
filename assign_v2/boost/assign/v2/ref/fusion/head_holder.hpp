//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_HEAD_HOLDER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_HEAD_HOLDER_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/mpl/void.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

	template<typename Tag,typename T, bool is_empty = false>
	class head_holder{
    
		typedef ref::wrapper<Tag, T> wrapper_;
        
        public:

		typedef T head_value_type;
        
        head_holder(){}
        head_holder( T& h ) : head( h ){}
        
        mutable wrapper_ head;
        
    	T& back()const
        {
        	return this->head.get();
        }
        
    };

	template<typename Tag, typename T>
	class head_holder<Tag, T, true>{
    
    	//head_holder(){}
    
    	public:
    
    	typedef ::boost::mpl::void_ head_value_type;
    
    	head_value_type back()const
        {
        	return head_value_type();
        }
    
    };

}// fusion_aux
}// ref
}// v2
}// assign
}// boost

#endif
