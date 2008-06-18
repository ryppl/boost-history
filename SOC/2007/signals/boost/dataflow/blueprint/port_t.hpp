// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_T_HPP

#include <boost/dataflow/blueprint/binary_operation_t.hpp>
#include <boost/dataflow/blueprint/port.hpp>
#include <boost/dataflow/utility/copy_cv.hpp>
#include <boost/dataflow/support/port.hpp>

#include <boost/call_traits.hpp>

#include <typeinfo>


namespace boost { namespace dataflow { namespace blueprint {

template<typename Base, typename PortOrRef, typename Tag>
class port_t_base : public Base
{
protected:
    typedef typename remove_reference<PortOrRef>::type port_type;
    typedef typename traits_of<port_type, Tag>::type port_traits_type;
public:
    port_t_base(typename call_traits<PortOrRef>::param_type p)
        : Base(
            runtime_info(typename port_traits_type::category()),
            runtime_info(typename port_traits_type::tag()))
        , p(p)
    {}

    virtual boost::any get()
    {
        return boost::any(&p);
    }
    virtual const std::type_info &port_type_info()
    {
        return typeid(port_type);
    };
    virtual const std::type_info &port_traits_type_info()
    {
        return typeid(port_traits_type);
    };
protected:
    typename remove_const<PortOrRef>::type p;
};

template<typename PortOrRef, typename Tag=default_tag, typename Enable=void>
class port_t : public port_t_base<port, PortOrRef, Tag>
{
    BOOST_MPL_ASSERT((is_same<Enable, void>));

public:
    port_t(typename call_traits<PortOrRef>::param_type p)
        : port_t_base<port, PortOrRef, Tag>(p)
    {}
    virtual port *clone() const
    {
        return new port_t(*this);
    };
};

template<typename PortOrRef, typename Tag>
class port_t<
    PortOrRef,
    Tag,
    typename enable_if<
        is_complemented_port<
            typename remove_reference<PortOrRef>::type,
            Tag>
    >::type >
    : public port_t_base<complemented_port, PortOrRef, Tag>
{
    typedef typename port_t::port_type port_type;
    typedef typename port_t::port_traits_type port_traits_type;

    template<typename Operation>
    struct operation_type
    {
        typedef blueprint::binary_operation_t<
            port_type,
            typename port_traits_type::complement_port_type,
            Operation,
            Tag
            > p_to_c;
        typedef blueprint::binary_operation_t<
            typename port_traits_type::complement_port_type,
            port_type,
            Operation,
            Tag
        > c_to_p;
    };

public:
    port_t(typename call_traits<PortOrRef>::param_type p)
        : port_t_base<complemented_port, PortOrRef, Tag>(p)
        , c_to_p_connector(new typename operation_type<operations::connect>::c_to_p())
        , c_to_p_extractor(new typename operation_type<operations::extract>::c_to_p())
        , p_to_c_connector(new typename operation_type<operations::connect>::p_to_c())
        , p_to_c_extractor(new typename operation_type<operations::extract>::p_to_c())
    {}
    
    virtual shared_ptr<binary_operation<operations::connect> > port_to_complement_connector()
    {   return p_to_c_connector; }
    virtual shared_ptr<binary_operation<operations::extract> > port_to_complement_extractor()
    {   return p_to_c_extractor; }
    virtual shared_ptr<binary_operation<operations::connect> > complement_to_port_connector()
    {   return c_to_p_connector; }
    virtual shared_ptr<binary_operation<operations::extract> > complement_to_port_extractor()
    {   return c_to_p_extractor; }

    virtual const std::type_info &complement_type_info()
    {
        return typeid(typename port_traits_type::complement_port_type);
    }
    virtual port *clone() const
    {
        return new port_t(*this);
    };

private:
    virtual bool is_operable_port_to_complement(int operation_uuid)
    {
        typedef port_type left_type;
        typedef typename port_traits_type::complement_port_type right_type;
        
        if(operation_uuid == runtime::property<operations::connect, int>()())
            return dataflow::are_binary_operable<
                left_type, right_type, operations::connect, Tag>::type::value; 

        if(operation_uuid == runtime::property<operations::extract, int>()())
            return dataflow::are_binary_operable<
                left_type, right_type, operations::extract, Tag>::type::value;

        return false;
    };
    virtual bool is_operable_complement_to_port(int operation_uuid)
    {
        return false;
    }

    boost::shared_ptr<binary_operation<operations::connect> > c_to_p_connector;
    boost::shared_ptr<binary_operation<operations::extract> > c_to_p_extractor; 
    boost::shared_ptr<binary_operation<operations::connect> > p_to_c_connector; 
    boost::shared_ptr<binary_operation<operations::extract> > p_to_c_extractor; 
};

} } } // namespace boost::dataflow::blueprint

#include <boost/dataflow/blueprint/vector_port_t.hpp>
#include <boost/dataflow/blueprint/keyed_port_t.hpp>

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP
