//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_OPERATOR_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_OPERATOR_ER_2010_HPP
#include <boost/assign/v2/ref/list/as_modulo_list.hpp>
#include <boost/assign/v2/ref/list_tuple/as_args_list.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/pipe/functor/rhs.hpp>
#include <boost/assign/v2/put/pipe/modulo/forward.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_pipe_aux{

    template<
        typename C,
        typename ParList,
        typename ArgsList,
        bool enable_pars
    >
    C& operator|(
        C& cont,
        put_pipe_aux::rhs<ParList, ArgsList, enable_pars> const& rhs_
    ){
		ref::as_args_list(
            ref::as_modulo_list<ParList>( put( cont ), rhs_.par_list_cont() ),
            rhs_.args_list_cont()
        );
        return cont;
    }

}// put_pipe_aux
namespace{

    put_pipe_aux::rhs<> const _put = put_pipe_aux::rhs<>();

}
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_PIPE_FUNCTOR_OPERATOR_ER_2010_HPP
