// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_SUPPORT_HPP
#define BOOST_DATAFLOW_MANAGED_SUPPORT_HPP

#include <boost/dataflow/generic/framework.hpp>

namespace boost { namespace dataflow { namespace managed {

struct framework : public dataflow::framework<operations::connect> {};

} } }

#endif // BOOST_DATAFLOW_MANAGED_SUPPORT_HPP
