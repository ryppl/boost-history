//-----------------------------------------------------------------------------
// boost mpl/test/for_each.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include "boost/mpl/for_each.hpp"
#include "boost/mpl/value_list.hpp"
#include "boost/mpl/arithmetic/plus.hpp"
#include "boost/mpl/identity/project2nd.hpp"
#include "boost/mpl/compose.hpp"
#include "boost/mpl/et.hpp"
#include "boost/static_assert.hpp"
#include <iostream>

namespace mpl = boost::mpl;

struct print_number
{
    template<typename N> struct apply
    {
        typedef N type;
        static void execute() { std::cout << N::value << '\n'; }
    };
};

int main()
{
    typedef mpl::value_list<7,5,1,2,4> values;
    typedef mpl::for_each<
          values
        , mpl::int_t<0>
        , mpl::plus<mpl::_1,mpl::_2>
        >::state state;
    
    BOOST_STATIC_ASSERT(state::value == 19);    

    typedef mpl::for_each<
          values
        , void
        , mpl::compose_f_gxy<
                print_number
              , mpl::project2nd<mpl::_1,mpl::_2>
              >
        > loop;

    loop::execute();
    return 0;
}
