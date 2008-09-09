// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/component.hpp>
#include <boost/dataflow/signals/component/filter.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

#include <map>
#include <iostream>

#include <boost/extension/extension.hpp>
#include <boost/extension/factory_map.hpp>


namespace component {

// a regular Dataflow.Signals component
class add : public boost::signals::filter<
    add,
    // output signature
    void (int),
    // input signature (must be listed to be accessible at runtime)
    void (int, int)>
{
public:
    void operator()(int a, int b)
    {
        out(a+b);
    }
};

}

extern "C" void BOOST_EXTENSION_EXPORT_DECL 
extension_export_components(boost::extensions::factory_map & fm)
{
    using namespace boost::extensions;
    namespace blueprint = boost::dataflow::blueprint;
    using boost::dataflow::signals::tag;

    // "add" is the string key for this component
    // blueprint::component is a polymorphic base class for components
    fm.get<blueprint::component, std::string>()["add"]
    // component_t inherits blueprint::component
        .set<blueprint::component_t<component::add, tag> >();
}
