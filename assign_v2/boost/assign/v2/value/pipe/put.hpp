//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_PUT_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/ref/list/as_modulo_list.hpp>
#include <boost/assign/v2/ref/list_tuple/as_args_list.hpp>
#include <boost/assign/v2/value/put.hpp>
#include <boost/assign/v2/value/pipe/args_list.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_put_pipe_put
namespace put_pipe_aux{

    template<
        typename C,
        typename ParList,
        typename ArgsList,
        bool enable_pars
    >
    C& operator|(
        C& cont,
        put_pipe_aux::args_list<
            ParList, ArgsList, enable_pars
        > const& args_list_
    )/*<-*/{
        ref::as_args_list(
            ref::as_modulo_list<ParList>( 
                put( cont ), args_list_.par_list_cont() 
            ),
            args_list_.args_list_cont()
        );
        return cont;
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

}// put_pipe_aux
//<-
namespace{
//->

    put_pipe_aux::args_list<> const _put /*<-*/= put_pipe_aux::args_list<>()/*->*/;

//<-
}
//->
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_OPERATOR_ER_2010_HPP
