// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_CONNECTION_ITERATOR_RELATIVE_HPP
#define BOOST_DATAFLOW_PHOENIX_CONNECTION_ITERATOR_RELATIVE_HPP

#include <boost/dataflow/phoenix/static_value.hpp>

#include <boost/mpl/int.hpp>
#include <boost/spirit/phoenix/core/argument.hpp>
#include <boost/spirit/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/phoenix/operator/self.hpp>
#include <boost/typeof/typeof.hpp>

namespace boost { namespace phoenix {

namespace iterator_relative
{
//    function<boost::phoenix::impl::copy_advance_c<-1> > prev;
//    function<boost::phoenix::impl::copy_advance_c<-2> > prev2;
    BOOST_TYPEOF(*(boost::phoenix::arg_names::arg1-
        boost::phoenix::actor<boost::phoenix::static_value<boost::mpl::int_<2> > >())) const prev2;
    BOOST_TYPEOF(*(boost::phoenix::arg_names::arg1-
        boost::phoenix::actor<boost::phoenix::static_value<boost::mpl::int_<2> > >())) const prev1;
}

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_CONNECTION_ITERATOR_RELATIVE_HPP