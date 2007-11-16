// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP
#define BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP

#include <boost/dataflow/blueprint/port.hpp>


namespace boost { namespace dataflow { namespace blueprint {

class connection
{
public:
    virtual void connect(port &producer, port &consumer)=0;
    virtual ~connection() {};
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP