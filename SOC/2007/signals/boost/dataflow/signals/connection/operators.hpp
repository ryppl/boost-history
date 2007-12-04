// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_SIGNALS_CONNECTION_OPERATORS_HPP
#define BOOST_DATAFLOW_SIGNALS_CONNECTION_OPERATORS_HPP

#include <boost/dataflow/signals/support.hpp>

namespace boost { namespace signals {

// the include templates expect DATAFLOW_TEMPLATE_TAG to have
// the mechanism type
#define DATAFLOW_TEMPLATE_TAG boost::dataflow::signals::tag

#   define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::signals::connect_mechanism
#   define DATAFLOW_TEMPLATE_BINARY_OPERATION connect
#       define DATAFLOW_TEMPLATE_OPERATOR >>=
#           include <boost/dataflow/templates/operator.hpp>
#       undef DATAFLOW_TEMPLATE_OPERATOR
#       define DATAFLOW_TEMPLATE_OPERATOR |
#           include <boost/dataflow/templates/operator.hpp>
#       undef DATAFLOW_TEMPLATE_OPERATOR
#   undef DATAFLOW_TEMPLATE_BINARY_OPERATION
#   undef DATAFLOW_TEMPLATE_MECHANISM
#undef DATAFLOW_TEMPLATE_TAG
    
    
} } // namespace boost::dataflow

#endif // BOOST_DATAFLOW_SIGNALS_CONNECTION_OPERATORS_HPP

