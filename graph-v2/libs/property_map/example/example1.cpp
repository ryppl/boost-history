//  (C) Copyright Jeremy Siek 2002.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[code_example_1
#include <iostream>
#include <map>
#include <string>

#include <boost/cstdlib.hpp>
#include <boost/property_map.hpp>

template <typename AddressMap>
void foo(AddressMap address)
{
    typedef typename boost::property_traits<AddressMap>::value_type value_type;
    typedef typename boost::property_traits<AddressMap>::key_type key_type;

    value_type old_address, new_address;
    key_type fred = "Fred";
    old_address = get(address, fred);
    new_address = "384 Fitzpatrick Street";
    put(address, fred, new_address);

    key_type joe = "Joe";
    value_type& joes_address = address[joe];
    joes_address = "325 Cushing Avenue";
}

int
main()
{
    typedef std::map<std::string, std::string> StringMap;
    StringMap name2address;
    boost::associative_property_map<StringMap> address_map(name2address);

    name2address.insert(make_pair(std::string("Fred"), std::string("710 West 13th Street")));
    name2address.insert(make_pair(std::string("Joe"), std::string("710 West 13th Street")));

    foo(address_map);

    StringMap::iterator i, end = name2address.end();
    for(i = name2address.begin(); i != end; ++i) {
        std::cout << i->first << ": " << i->second << "\n";
    }

    return boost::exit_success;
}
//]
