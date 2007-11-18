// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_HPP

#include <boost/shared_ptr.hpp>
#include <string>

namespace boost { namespace dataflow { namespace blueprint {

class connection;

class port_category
{
public:
    port_category(const std::string &name, int uuid)
        : name_(name), uuid_(uuid)
    {}

    const char *name() const
    {   return name_.c_str(); }
    int uuid() const
    {   return uuid_; }
private:    
    std::string name_;
    int uuid_;
};

class port_traits
{
public:
    port_traits(const port_category &category) : category_(category)
    {}
    const port_category &category() const
    {   return category_;
    }
private:
    port_category category_;
};

class port
{
public:
    port(const port_category &category) : traits_(category)
    {}
    const port_traits &traits() const
    {   return traits_; }
    
    virtual void *get()=0;
    virtual shared_ptr<connection> connector()=0;
    virtual ~port() {};
private:
    port_traits traits_;
};

} } } // namespace boost::dataflow::blueprint

    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP