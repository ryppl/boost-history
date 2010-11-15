//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_SEQ_ARG_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_FORWARD_SEQ_ARG_ER_2010_HPP
#include <algorithm>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/assign/v2/put/compose/csv/held.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_compose_csv_aux{

	template<typename O>
	class arg_forwarder : ref::wrapper<ref::assign_tag::copy, O const>
    {
    	typedef ref::assign_tag::copy assign_tag_;
        typedef ref::wrapper<assign_tag_, O const> super_t;

        public:
        
        arg_forwarder(){}
        arg_forwarder(O const& object)
        	:super_t( object ){}
        
        template<typename T>
        void operator()(T& t)const
        {
        	this->unwrap()( t );
        }
        template<typename T>
        void operator()(T const& t)const
        {
        	this->unwrap()( t );
        }

    };

	template<typename O, typename Pars, std::size_t N, typename U>
	void forward_seq_arg(O const& object, held<Pars, N, U> const& h)
    {
    	typedef arg_forwarder<O> forwarder_;
        forwarder_ f( object );
		std::for_each(
        	boost::begin( h.sequence_arg() ),
            boost::end( h.sequence_arg() ),
            f
        );
		
    }
 
}// put_compose_csv_aux
}// v2
}// assign
}// boost

#endif
