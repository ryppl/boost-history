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
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/put/modifier/push_front.hpp>
#include <boost/assign/v2/put/generic/generic.hpp>
#include <boost/assign/v2/put/sub/sub.hpp>
#include <boost/assign/v2/put/generic/new_fun.hpp>
#include <boost/assign/v2/put/generic/new_modifier.hpp>
#include <boost/assign/v2/put/compose/functor/composer.hpp>
#include <boost/assign/v2/put/compose/common/forward_parameter.hpp>

#include <libs/assign/v2/test/put/compose/common/forward_parameter.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_compose{
namespace xxx_common{
namespace xxx_forward_parameter{

    void test()
    {
        using namespace boost::assign::v2;
        typedef std::list<int> cont_;
        cont_ cont;
        using namespace adaptor;
        typedef modifier_tag::push_front modifier_tag_;
        typedef functor_aux::identity identity_;
        typedef put_aux::new_fun<identity_> fun_;
        typedef put_aux::set_modifier<modifier_tag_> modifier_;
        typedef boost::mpl::vector2<
        	fun_,
            modifier_
        > pars_;
        put_compose_aux::forward_parameter<pars_>(
            put( cont ),
            _put %  ( _fun = _identity ) % ( _modifier = modifier_tag_() )
        )( 1 )( 2 )( 3 );
        BOOST_ASSIGN_V2_CHECK_EQUAL( cont.size(), 3 );
        BOOST_ASSIGN_V2_CHECK_EQUAL( cont.front(), 3 ); cont.pop_front();
        BOOST_ASSIGN_V2_CHECK_EQUAL( cont.front(), 2 ); cont.pop_front();
        BOOST_ASSIGN_V2_CHECK_EQUAL( cont.front(), 1 ); cont.pop_front();
    }
    
}// xxx_forward_parameter
}// xxx_common
}// xxx_compose
}// xxx_put
}// xxx_test_assign

