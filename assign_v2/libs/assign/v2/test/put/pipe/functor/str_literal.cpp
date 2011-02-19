//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <string>

#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <libs/assign/v2/test/put/pipe/functor/str_literal.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_str_literal{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        typedef const char* T;
        std::deque<T> cont; typedef std::string str_;
        cont | as2::_put( "x" )( "y" )( "z" );
        BOOST_ASSIGN_V2_CHECK( str_( cont[0] ) == "x" );
        BOOST_ASSIGN_V2_CHECK( str_( cont[1] ) == "y" );
        BOOST_ASSIGN_V2_CHECK( str_( cont[2] ) == "z" );
    }

}// xxx_str_literal
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2
