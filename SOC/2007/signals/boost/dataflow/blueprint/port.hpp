// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_HPP

#include <boost/shared_ptr.hpp>

namespace boost { namespace dataflow { namespace blueprint {

class connection;

class port_traits
{
public:
    virtual const char *category() const=0;
    virtual int category_uuid() const=0;
    virtual shared_ptr<connection> connector()=0;
    virtual ~port_traits() {};
};

class port : public port_traits
{
public:
    virtual void *get()=0;
};

} } } // namespace boost::dataflow::blueprint

    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP