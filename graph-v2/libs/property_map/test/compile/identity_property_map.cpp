// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/property_map/archetypes.hpp>
#include <boost/property_map/identity_property_map.hpp>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::property_map;

    typedef null_archetype<> Key;
    typedef identity_property_map<Key> IdentityMap;

    Key& k = static_object<Key>::get();
    IdentityMap imap;
    get(imap, k);
    imap[k];

    return 0;
}
