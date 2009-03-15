/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__PORT_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__PORT_ADAPTER_HPP


#include <boost/dataflow/blueprint/framework_entity_adapter.hpp>
#include <boost/dataflow/blueprint/port.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework, typename PortOrRef, typename Base=port<BlueprintFramework> >
class port_adapter : public framework_entity_adapter<BlueprintFramework, PortOrRef, Base>
{
    typedef framework_entity_adapter<BlueprintFramework, PortOrRef, Base> base_type;
public:
    port_adapter(blueprint::framework_context<BlueprintFramework> &fo)
        : base_type(fo)
    {   initialize(); }
    template<typename T>
    port_adapter(blueprint::framework_context<BlueprintFramework> &fo, const T &t)
        : base_type(fo, t)
    {   initialize(); }
    template<typename T>
    port_adapter(blueprint::framework_context<BlueprintFramework> &fo, T &t)
        : base_type(fo, t)
    {   initialize(); }
private:
    void initialize()
    {   base_type::set_category_type_info(typeid(typename traits_of<typename base_type::entity_type>::type::category)); }
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__PORT_ADAPTER_HPP
