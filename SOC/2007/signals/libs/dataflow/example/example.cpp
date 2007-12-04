// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/at.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/include/mpl.hpp>

using namespace boost;

int main()
{
    BOOST_MPL_ASSERT(( mpl::at<fusion::vector<mpl::true_>, mpl::int_<0> >::type ));
    BOOST_MPL_ASSERT(( mpl::is_sequence<fusion::vector<int> > ));
}
