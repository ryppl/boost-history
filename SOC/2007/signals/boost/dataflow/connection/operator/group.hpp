// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_CONNECTION_OPERATOR_GROUP_HPP
#define BOOST_DATAFLOW_CONNECTION_OPERATOR_GROUP_HPP

#include <boost/dataflow/connection/group.hpp>
#include <boost/utility/result_of.hpp>

namespace boost { namespace dataflow {
    
namespace operators {
            
    template<typename T1, typename T2>
    typename boost::dataflow::result_of::group<T1,T2>::type
    operator & (T1 &t1, T2 &t2)
    { return boost::dataflow::group(t1, t2); }
    
} // namespace operators
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_CONNECTION_OPERATORS_HPP

