//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_RANGE_ER_2010_HPP
#include <boost/range/algorithm/for_each.hpp>
#include <boost/assign/v2/put/sub/functor.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_range_aux
{
	template<typename From>
	class adaptor1 : ref::wrapper<ref::assign_tag::copy, From const>
    {
        typedef ref::wrapper<ref::assign_tag::copy, From const> super_t;

        public:

    	adaptor1(From const& from) : super_t(from){}

        From const& from()const{ return this->unwrap(); }

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
	To& operator|(To& to, put_range_aux::adaptor1<From> const& h)
    {
		// TODO remove
		// typedef typename put_range_aux::deduce_operation<To>::type tag_;
    	// put_range_aux::put_range( tag_(), h.from(), to );

        ::boost::for_each( h.from(), put( to ) );
        return to;
    }

}// put_range_aux
	put_range_aux::adaptor2 const _put_range = put_range_aux::adaptor2();
}// v2
}// assign
}// boost

#endif
