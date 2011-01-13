//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_LINK_HOLDER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_LINK_HOLDER_ER_2010_HPP
#include <boost/mpl/int.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

	template<typename L, bool is_empty = false>
	class link_holder{

		typedef ref::assign_tag::copy tag_;
		typedef ref::wrapper<tag_, const L> wrapper_;

        public:

        //link_holder();
        explicit link_holder( const L& l) : value( l ){}

    	wrapper_ pop()const
        {
        	// Warning : probably inefficient
        	return this->value;
        }

        typedef typename wrapper_::unwrap_type link_type;
        link_type link()const
        {
            return this->value.get();
        }

        protected:
        mutable wrapper_ value;

    };

	template<typename L>
	class link_holder<L,true>{

    	//link_holder(){}

    	void pop()const{}

    };

}// fusion_aux
}// ref
}// v2
}// assign
}// boost

#endif
