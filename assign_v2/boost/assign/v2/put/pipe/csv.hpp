//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CSV_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_CSV_ER_2010_HPP

/*#include <boost/assign/v2/put/pipe/csv/rhs.hpp>
#include <boost/assign/v2/put/pipe/csv/generator.hpp>
#include <boost/assign/v2/put/pipe/csv/keyword.hpp>
#include <boost/assign/v2/put/pipe/csv/operator.hpp>*/

#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/utility/csv.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace csv_aux{

    template<typename C, typename Pars, typename SeqArgs, bool enable_pars>
    C& operator|(
    	C& cont, 
        csv_aux::adapter<
        	v2::put_pipe_aux::rhs<Pars, SeqArgs, enable_pars> 
        > const& rhs
    )
    {
		cont | rhs.functor();
    }

}// csv_aux
}// v2
}// assign
}// boost

#endif
