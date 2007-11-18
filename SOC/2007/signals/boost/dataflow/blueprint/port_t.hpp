// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_T_HPP

#include <boost/dataflow/blueprint/port.hpp>
#include <boost/dataflow/support/port.hpp>

namespace boost { namespace dataflow { namespace blueprint {

namespace runtime
{
    template<typename T, typename PropertyType>
    struct property;
    
    template < >
    struct property<ports::producer, const char *>
    {
        const char *operator()()
        {
            return "producer";
        }
    };
    
    template < >
    struct property<ports::consumer, const char *>
    {
        const char *operator()()
        {
            return "consumer";
        }
    };
    
    template < >
    struct property<ports::producer, int>
    {
        int operator()()
        {
            return 0;
        }
    };
    
    template < >
    struct property<ports::consumer, int>
    {
        int operator()()
        {
            return 1;
        }
    };
}


template<typename PortTraits, typename Port>
class port_t : public port
{
    typedef typename PortTraits::mechanism::template runtime_connection<PortTraits>::type connection_type;
public:
    port_t(Port p)
        : port(port_category(
            runtime::property<typename PortTraits::category, const char *>()(),
            runtime::property<typename PortTraits::category, int>()()))
        , p(p)
        , c(new connection_type())
    {}
    
    void *get()
    {
        return &p;
    }
    virtual shared_ptr<connection> connector()
    {
        return c;
    };
private:
    Port p;
    boost::shared_ptr<connection> c; 
};

} } } // namespace boost::dataflow::blueprint

    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP