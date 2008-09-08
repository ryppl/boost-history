// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/component.hpp>

#include <boost/extension/factory_map.hpp>
#include <boost/extension/shared_library.hpp>
#include <boost/extension/convenience.hpp>

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

    std::map<std::string, boost::extensions::factory<boost::dataflow::blueprint::component> > map = fm.get<blueprint::component, std::string>();
    std::cout << "Loaded " << map.size() << " components" << std::endl;

    return 0;
}
