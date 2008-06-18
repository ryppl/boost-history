// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_HPP

#include <boost/dataflow/support/binary_operation.hpp>
#include <boost/dataflow/support/complemented_port.hpp>
#include <boost/dataflow/support/runtime.hpp>

#include <boost/any.hpp>
#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace boost { namespace dataflow { namespace blueprint {

template<typename T>
class binary_operation;

class port_traits
{
public:
    port_traits(const runtime_info &category, const runtime_info &tag)
        : category_(category), tag_(tag)
    {}
    const runtime_info &category() const
    {   return category_; }
    const runtime_info &tag() const
    {   return tag_; }
private:
    runtime_info category_;
    runtime_info tag_;
};

class complemented_port;
class vector_port;
class keyed_port;

/// Base class for all blueprint port types.  Run-time analogue of the Port concept. 
class port
{
public:
    port(const runtime_info &category, const runtime_info &tag)
        : traits_(category, tag)
    {}
    const port_traits &traits() const
    {   return traits_; }
    
    virtual bool is_complemented_port() const
    {   return false; }
    virtual bool is_vector_port() const
    {   return false; }
    virtual bool is_keyed_port() const
    {   return false; }
    template<typename T>
    T &as()
    {   return *static_cast<T *>(this); }

    virtual boost::any get()=0;
    virtual const std::type_info &port_type_info()=0;
    virtual const std::type_info &port_traits_type_info()=0;
    virtual ~port() {};
    virtual port *clone() const =0;
    port &resolve_key(port &key);
private:
    port_traits traits_;
};

/// Base class for all blueprint complemented port types.  Run-time analogue of the ComplementedPort concept. 
class complemented_port : public port
{
public:
    complemented_port(const runtime_info &category, const runtime_info &tag)
        : port(category, tag)
    {}
    virtual bool is_complemented_port() const
    {
        return true;
    }
    virtual const std::type_info &complement_type_info()=0;
    
    virtual shared_ptr<binary_operation<operations::connect> > port_to_complement_connector()=0;
    virtual shared_ptr<binary_operation<operations::connect> > complement_to_port_connector()=0;
    virtual shared_ptr<binary_operation<operations::extract> > port_to_complement_extractor()=0;
    virtual shared_ptr<binary_operation<operations::extract> > complement_to_port_extractor()=0;
    
    template<typename Operation>
    typename enable_if<
        is_same<Operation, operations::connect>,
        shared_ptr<binary_operation<Operation> >
    >::type port_to_complement_operation()
    {   return port_to_complement_connector(); }
    
    template<typename Operation>
    typename enable_if<
        is_same<Operation, operations::extract>,
        shared_ptr<binary_operation<Operation> >
    >::type port_to_complement_operation()
    {   return port_to_complement_extractor(); }
    
    template<typename Operation>
    typename enable_if<
        is_same<Operation, operations::connect>,
        shared_ptr<binary_operation<Operation> >
    >::type complement_to_port_operation()
    {   return complement_to_port_connector(); }
    
    template<typename Operation>
    typename enable_if<
        is_same<Operation, operations::extract>,
        shared_ptr<binary_operation<Operation> >
    >::type complement_to_port_operation()
    {   return complement_to_port_extractor(); }

    template<typename Operation>
    bool is_operable_port_to_complement()
    {
        return is_operable_port_to_complement(runtime::property<Operation, int>()());
    }

    template<typename Operation>
    bool is_operable_complement_to_port()
    {
        return is_operable_complement_to_port(runtime::property<Operation, int>()());
    }
private:
    virtual bool is_operable_port_to_complement(int operation_uuid)=0;
    virtual bool is_operable_complement_to_port(int operation_uuid)=0;
};

class vector_port : public port
{
public:
    vector_port(const runtime_info &category, const runtime_info &tag)
        : port(category, tag)
    {}

    virtual bool is_vector_port() const
    {   return true; }

    virtual size_t num_ports() const=0;
    virtual port & get_port(int port_num)=0;
};

class keyed_port : public vector_port
{
public:
    keyed_port(const runtime_info &category, const runtime_info &tag)
        : vector_port(category, tag)
    {}

    virtual bool is_keyed_port() const
    {   return true; }
    virtual bool has_key(const std::type_info &info)=0;
    virtual port &get_keyed_port(const std::type_info &info)=0;
};

inline port &port::resolve_key(port &key)
{
    port *resolved = this;
    while (resolved->is_keyed_port() &&
            resolved->as<keyed_port>().has_key(key.port_traits_type_info()))
        resolved = &resolved->as<keyed_port>().get_keyed_port(key.port_traits_type_info());
    return *resolved;
}

} } } // namespace boost::dataflow::blueprint

namespace boost
{
    template<>
    inline dataflow::blueprint::port* new_clone<dataflow::blueprint::port>
        ( const dataflow::blueprint::port& r )
    {
        return r.clone();
    }
}
    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP
