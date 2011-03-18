//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_VALUE_PIPE_CSV_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_VALUE_PIPE_CSV_PUT_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/ref/array/as_arg_list.hpp>
#include <boost/assign/v2/ref/list/as_modulo_list.hpp>
#include <boost/assign/v2/value/put.hpp>
#include <boost/assign/v2/value/pipe/arg_list.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_pipe_csv_put
namespace put_pipe_aux{

    template<typename C,
        typename ParList, arg_list_size_type N, typename U>
    C& operator|(C& cont, put_pipe_aux::arg_list<
            ParList, N, U
        > const& arg_list
    )/*<-*/
    {

        v2::ref::as_arg_list(
            v2::ref::as_modulo_list<ParList>( 
                put( cont ), 
                arg_list.par_list_cont() 
            ),
            arg_list.arg_list_cont()
        );
        return cont;

    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// put_pipe_aux

    put_pipe_aux::arg_list_generator<> const _csv_put/*<-*/
        = put_pipe_aux::arg_list_generator<>()/*->*/;

//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_VALUE_PIPE_CSV_PUT_ER_2010_HPP
