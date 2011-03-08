//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <list>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <boost/assign/v2/put/modifier/push_front.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/pipe/modulo/forward.hpp>

#include <libs/assign/v2/test/put/pipe/modulo/forward.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modulo{
namespace xxx_forward{

    void test()
    {
        namespace as2 = boost::assign::v2;
        typedef int T; typedef std::list<T> C; C cont;
        
        typedef as2::functor_aux::identity f_;
        typedef as2::put_aux::modulo_fun<f_> par1_;
        typedef as2::put_aux::modulo_modifier<
        	as2::put_aux::keyword_modifier, 
            as2::modifier_tag::push_front
        > par2_;
        typedef ::boost::mpl::vector2<par1_, par2_> pars_;
        
        as2::put_pipe_aux::forward_pars<pars_>(
            as2::put( cont ),
            ( as2::_put %  ( as2::_fun = as2::_identity ) % ( as2::_push_front ) ).pars()
        )( 1 )( 2 )( 3 );
        BOOST_ASSIGN_V2_CHECK( cont.size() == 3 );
        BOOST_ASSIGN_V2_CHECK( cont.front() == 3 ); cont.pop_front();
        BOOST_ASSIGN_V2_CHECK( cont.front() == 2 ); cont.pop_front();
        BOOST_ASSIGN_V2_CHECK( cont.front() == 1 ); cont.pop_front();
    }

}// xxx_forward
}// xxx_modulo
}// xxx_pipe
}// xxx_put
}// xxx_test_assign
