//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ANON_GENERIC_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ANON_GENERIC_CONTAINER_ER_2010_HPP
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/apply.hpp>

#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/ref/wrapper/deduce_assign_tag.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/container.hpp>
#include <boost/assign/v2/ref/anon/generic/traits.hpp>

namespace boost{
	struct use_default;
namespace assign{ 
namespace v2{
namespace ref{
namespace anon_aux{
	
    // N 		: size of the array
    // L 		: link 
    // T 		: value type
    // Tag1 	: assign tag
    // Tag2 	: policy tag
    template<std::size_t N, typename L,typename Tag1, typename Tag2, typename T>
    class cont
    	: public anon_aux::inherit<N,L,Tag1,Tag2,T>::super1_t,
    	 public anon_aux::inherit<N,L,Tag1,Tag2,T>::super2_t
    {
    	typedef anon_aux::inherit<N,L,Tag1,Tag2,T> inherit_;
        typedef cont<N,L,Tag1,Tag2,T> this_;

		public:
        typedef typename inherit_::super1_t super1_t; // fusion_list
        typedef typename inherit_::super2_t super2_t; // interface

		protected:
		super1_t const& super1()const
        {
        	return static_cast<super1_t const&>( *this );
        }

		public:
        
        typedef typename super1_t::size_type size_type;
        
        explicit cont(){} 
        explicit cont(const L& link,T& head) 
        	: super1_t( link, head ){}

        // Needed by csv
        template<typename NewL,typename NewTag2>
        explicit cont(const cont<N,NewL,Tag1,NewTag2,T>& that)
        	:super1_t( that ),super2_t(that){}

        typedef cont<N+1,this_,Tag1,Tag2,T> result_type;

        result_type
        operator()(T& t)const
        { 
        	this->super1()( t );
            return result_type( *this, t );
        }
    
    	//using super1_t::operator[];
	};

}// anon_aux                
}// ref
}// v2
}// assign
}// boost

#endif
