//-----------------------------------------------------------------------------
// boost mpl/test/while_true.cpp source file
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

#include "boost/mpl/while_true.hpp"
#include "boost/mpl/same_as.hpp"
#include "boost/mpl/utility/next.hpp"
#include "boost/mpl/int_t.hpp"
#include <iostream>

namespace mpl = boost::mpl;

struct print_number
{
    template<typename N> struct apply
    {
        typedef typename mpl::next<N>::type type;
        static void execute() { std::cout << N::value << '\n'; }
    };
};

int main()
{
    typedef mpl::while_true<
        mpl::not_same_as< mpl::int_t<10> >
      , mpl::int_t<0>
      , print_number
      > loop;

    loop::execute();
    return 0;
}
