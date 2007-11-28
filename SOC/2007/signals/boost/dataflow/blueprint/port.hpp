// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_PORT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_PORT_HPP

#include <boost/dataflow/support/binary_operation.hpp>
#include <boost/dataflow/support/complemented_port.hpp>
#include <boost/dataflow/support/runtime.hpp>

#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace boost { namespace dataflow { namespace blueprint {

template<typename T>
class binary_operation;

class port_traits
{
public:
    port_traits(const runtime_info &mechanism, const runtime_info &category,
        const runtime_info &concept_info)
        : mechanism_(mechanism), category_(category), concept_(concept_info)
    {}
    const runtime_info &mechanism() const
    {   return mechanism_; }
    const runtime_info &category() const
    {   return category_; }
    const runtime_info &concept_info() const
    {   return concept_; }
private:
    runtime_info mechanism_;
    runtime_info category_;
    runtime_info concept_;
};

class port
{
public:
    port(const runtime_info &mechanism, const runtime_info &category,
        const runtime_info &concept_info)
        : traits_(mechanism, category, concept_info)
    {}
    const port_traits &traits() const
    {   return traits_; }
    
    virtual void *get()=0;
    bool is_complemented_port()
    {
        return traits_.concept_info().uuid()
            == runtime::property<concepts::complemented_port, int>()();
    }
    virtual const std::type_info &port_type_info()=0;
    virtual ~port() {};
    virtual port *clone() const =0;
private:
    port_traits traits_;
};

class complemented_port : public port
{
public:
    complemented_port(const runtime_info &mechanism, 
        const runtime_info &category, const runtime_info &concept_info)
        : port(mechanism, category, concept_info)
    {}
        virtual const std::type_info &complement_type_info()=0;
    virtual shared_ptr<binary_operation<operations::connect> > connector()=0;
    virtual shared_ptr<binary_operation<operations::extract> > extractor()=0;
    template<typename Operation>
    typename enable_if<
        is_same<Operation, operations::connect>,
        shared_ptr<binary_operation<Operation> >
    >::type operation()
    {   return connector(); }
    template<typename Operation>
    typename enable_if<
        is_same<Operation, operations::extract>,
        shared_ptr<binary_operation<Operation> >
    >::type operation()
    {   return extractor(); }
template<typename Operation>
    bool is_operable_with_complement()
    {
        return is_operable_with_complement(runtime::property<Operation, int>()());
    }
private:
    virtual bool is_operable_with_complement(int operation_uuid)=0;
};

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