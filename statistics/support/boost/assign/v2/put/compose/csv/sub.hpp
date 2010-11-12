//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_COMPOSE_CSV_SUB_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_COMPOSE_CSV_SUB_ER_2010_HPP
#include <boost/assign/v2/put/compose/common/forward_parameter.hpp>
#include <boost/assign/v2/put/compose/csv/forward_seq_arg.hpp>
#include <boost/assign/v2/put/compose/csv/held.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_compose_csv_aux{

	template<typename V, typename Pars, std::size_t N, typename U>
    V& operator|(
    	V& v, 
        put_compose_csv_aux::held<Pars, N, U> const& c
	)
    {
        put_compose_csv_aux::forward_seq_arg( 
        	put_compose_aux::forward_parameter<Pars>( put( v ), c ), 
            c
        );
        return v;
    }
    
}// put_compose_csv_aux
}// v2
}// assign
}// boost

#endif
