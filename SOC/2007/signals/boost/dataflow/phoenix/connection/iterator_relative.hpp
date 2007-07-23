// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_CONNECTION_ITERATOR_RELATIVE_HPP
#define BOOST_DATAFLOW_PHOENIX_CONNECTION_ITERATOR_RELATIVE_HPP

#include <boost/dataflow/phoenix/static_value.hpp>
#include <boost/dataflow/phoenix/static_function.hpp>

#include <boost/spirit/phoenix/core/argument.hpp>
#include <boost/spirit/phoenix/operator/arithmetic.hpp>
#include <boost/spirit/phoenix/operator/self.hpp>
#include <boost/typeof/typeof.hpp>

namespace boost { namespace phoenix {

namespace iterator_relative
{
    namespace impl
    {
        struct get_value
        {
            template<class T>
            struct result
            {
                typedef const typename T::value_type & type;
            };

            template<class T>
            typename result<T>::type operator()(const T &t) const
            {        
                return t.value;
            }
        };

    }

    static_function<impl::get_value> const get_value = static_function<impl::get_value>();

    BOOST_TYPEOF(get_value(*(boost::phoenix::arg_names::arg1-actor<int_<2> >()))) const prev2;
    BOOST_TYPEOF(get_value(*(boost::phoenix::arg_names::arg1-actor<int_<1> >()))) const prev1;
}

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_CONNECTION_ITERATOR_RELATIVE_HPP
