// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/component.hpp>
#include <boost/dataflow/blueprint/binary_operation.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

#include <boost/extension/factory_map.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/convenience.hpp>

#include <boost/fusion/include/make_vector.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>
#include <list>
#include <map>

int main()
{
    namespace blueprint = boost::dataflow::blueprint;
    using namespace boost::extensions;
    std::string library_path = "libcomponent_add.extension";

    factory_map fm;
    std::cout << "load_single_library returned: "
        << load_single_library(fm, library_path, "extension_export_components") << std::endl;

    typedef std::map<std::string, boost::extensions::factory<boost::dataflow::blueprint::component> > map_type;
    map_type map = fm.get<blueprint::component, std::string>();
    std::cout << "Loaded " << map.size() << " components" << std::endl;

    if(map.find("add") != map.end())
    {
        // create the component
        boost::scoped_ptr<blueprint::component> filter(map["add"].create());
        std::cout << "Created component with " << filter->num_ports() << " ports.";

        // storage components for the input and the result
        blueprint::component_t<boost::signals::storage<void(int, int)>,boost::dataflow::signals::tag > one_two(boost::fusion::make_vector(1, 2));
        blueprint::component_t<boost::signals::storage<void(int)>,boost::dataflow::signals::tag > result;

        std::cout << "Connecting component with input storage (1,2) and result storage..." << std::endl;
        blueprint::connect(one_two.get_port(0), filter->get_port(1));
        blueprint::connect(filter->get_port(0), result.get_port(1));

        // invoke the input to send and process its contents
        one_two.invoke();
        // print the result
        std::cout << "Result storage: " << result.get().at<0>() << std::endl;
    }

    return 0;
}
