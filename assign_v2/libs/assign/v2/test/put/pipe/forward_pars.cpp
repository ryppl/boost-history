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
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/put/modifier/std/push_front.hpp>
#include <boost/assign/v2/put/generic.hpp>
#include <boost/assign/v2/put/sub.hpp>
#include <boost/assign/v2/put/generic/new_fun.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/pipe/forward_pars.hpp>

#include <libs/assign/v2/test/put/pipe/forward_pars.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_forward_pars{

    void test()
    {
        using namespace  boost::assign::v2;
        typedef std::list<int> cont_;
        cont_ cont;
        typedef modifier_tag::push_front modifier_tag_;
        typedef functor_aux::identity identity_;
        typedef put_aux::new_fun<identity_> fun_;
        typedef put_aux::set_modifier<modifier_tag_> modifier_;
        typedef boost::mpl::vector2<
        	fun_,
            modifier_
        > pars_;
        put_pipe_aux::forward_pars<pars_>(
            put( cont ),
            (
                _put %  (
                    _fun = _identity
                ) % ( _modifier = modifier_tag_() )
            ).pars()
        )( 1 )( 2 )( 3 );
        BOOST_ASSIGN_V2_CHECK( cont.size() == 3 );
        BOOST_ASSIGN_V2_CHECK( cont.front() == 3 ); cont.pop_front();
        BOOST_ASSIGN_V2_CHECK( cont.front() == 2 ); cont.pop_front();
        BOOST_ASSIGN_V2_CHECK( cont.front() == 1 ); cont.pop_front();
    }

}// xxx_forward_pars
}// xxx_pipe
}// xxx_put
}// xxx_test_assign

