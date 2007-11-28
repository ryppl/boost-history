// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_T_HPP

#include <boost/dataflow/blueprint/port.hpp>
#include <boost/dataflow/blueprint/binary_operation_t.hpp>
#include <boost/dataflow/support/port.hpp>
#include <boost/call_traits.hpp>

#include <typeinfo>


namespace boost { namespace dataflow { namespace blueprint {

template<typename T>
struct initializer_type
{
    typedef
        typename mpl::if_<
            is_reference<T>,
            T,
            typename call_traits<T>::const_reference
        >::type type;
};
    
template<typename Base, typename PortTraits, typename Port>
class port_t_base : public Base
{
public:
    port_t_base(typename initializer_type<Port>::type p)
        : Base(
            runtime_info(typename PortTraits::mechanism()),
            runtime_info(typename PortTraits::category()),
            runtime_info(typename PortTraits::concept()))
        , p(p)
    {}

    void *get()
    {
        return &p;
    }
    virtual const std::type_info &port_type_info()
    {
        return typeid(Port);
    };
private:
    Port p;
};

template<typename PortTraits, typename Port, typename Enable=void>
class port_t : public port_t_base<port, PortTraits, Port>
{
    BOOST_MPL_ASSERT((is_same<Enable, void>));

public:
    port_t(typename call_traits<Port>::const_reference p)
        : port_t_base<port, PortTraits, Port>(p)
    {}
    virtual port *clone() const
    {
        return new port_t(*this);
    };
};

template<typename PortTraits, typename Port>
class port_t<
    PortTraits,
    Port,
    typename enable_if<
        is_complemented_port<
            typename PortTraits::mechanism,
            typename PortTraits::category,
            Port
        >
    >::type >
    : public port_t_base<complemented_port, PortTraits, Port>
{

    typedef
        typename remove_reference<
            typename get_port_result_type<
                typename PortTraits::mechanism,
                typename PortTraits::category,
                Port
            >::type
        >::type get_port_type;
        
    typedef typename remove_reference<Port>::type port_type;

    template<typename Operation>
    struct operation_type
    {
        typedef
            typename mpl::if_<
                is_same<typename PortTraits::category, ports::producer>,
                blueprint::binary_operation_t<
                    Operation,
                    typename PortTraits::mechanism,
                    get_port_type,
                    typename PortTraits::complement_port_type
                >,
                blueprint::binary_operation_t<
                    Operation,
                    typename PortTraits::mechanism,
                    typename PortTraits::complement_port_type,
                    get_port_type
                >
            >::type type;
    };

public:
    port_t(typename initializer_type<Port>::type p)
        : port_t_base<complemented_port, PortTraits, Port>(p)
        , c(new typename operation_type<operations::connect>::type())
        , e(new typename operation_type<operations::extract>::type())
    {}
    
    virtual shared_ptr<binary_operation<operations::connect> > connector()
    {
        return c;
    };
    virtual shared_ptr<binary_operation<operations::extract> > extractor()
    {
        return e;
    };
    virtual const std::type_info &complement_type_info()
    {
        return typeid(typename PortTraits::complement_port_type);
    }
    virtual port *clone() const
    {
        return new port_t(*this);
    };

private:
    virtual bool is_operable_with_complement(int operation_uuid)
    {
        typedef typename mpl::if_<
            is_same<typename PortTraits::category, ports::producer>,
            port_type,
            typename PortTraits::complement_port_type
        >::type producer_type;
        typedef typename mpl::if_<
            is_same<typename PortTraits::category, ports::producer>,
            typename PortTraits::complement_port_type,
            port_type
        >::type consumer_type;

        if(operation_uuid == runtime::property<operations::connect, int>()())
            return dataflow::are_binary_operable<
                operations::connect, typename PortTraits::mechanism, producer_type, consumer_type>::type::value; 
        
        if(operation_uuid == runtime::property<operations::extract, int>()())
            return typename dataflow::are_binary_operable<
                operations::extract, typename PortTraits::mechanism, producer_type, consumer_type>::type();
        
        return false;
    };

    boost::shared_ptr<binary_operation<operations::connect> > c; 
    boost::shared_ptr<binary_operation<operations::extract> > e; 
};

} } } // namespace boost::dataflow::blueprint

    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP