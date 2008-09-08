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

class add : public boost::signals::filter<
    add,
    void (int),
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
    fm.get<blueprint::component, std::string>()["add"].set<blueprint::component_t<component::add, boost::dataflow::signals::tag> >();
    std::cout << "hello!" << std::endl;
}
