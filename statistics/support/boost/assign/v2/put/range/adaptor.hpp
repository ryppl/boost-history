//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_RANGE_ADAPTOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_RANGE_ADAPTOR_ER_2010_HPP
#include <boost/assign/v2/put/range/modifier.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_range_aux
{
	template<typename From>
	struct adaptor1
    {
    	adaptor1(From const& from):value(from){}
        mutable From const& value;
        private:
        adaptor1();
    };

	struct adaptor2
    {
    	adaptor2(){}
        template<typename From>
        adaptor1<From> operator()(const From& from)const
        {
        	typedef adaptor1<From> result_;
        	return result_( from );
        }
    };

	template<typename To,typename From>
	To& operator|(To& to, put_range_aux::adaptor1<From> const& from)
    {
		typedef typename put_range_aux::deduce_operation<To>::type tag_;
    	put_range_aux::put_range( tag_(), from.value, to );
        return to;
    }

}// put_range_aux
namespace adaptor
{
	put_range_aux::adaptor2 const _put_range
    	= put_range_aux::adaptor2();	

}// adaptor
}// v2
}// assign
}// boost

#endif
