//  (C) Copyright Jeremy Siek 2002.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[code_iterator_property_map
#include <iostream>
#include <vector>
#include <string>

#include <boost/cstdlib.hpp>
#include <boost/property_map/identity_property_map.hpp>
#include <boost/property_map/iterator_property_map.hpp>

using namespace std;
using namespace boost;
using namespace boost::property_map;

// This is a generic "algorithm" that uses a property map to compute
// a person's name as "Last, First" for a given key.
template <typename Key, typename FirstNameMap, typename LastNameMap>
string last_name_first(const Key& key, FirstNameMap first, LastNameMap last)
{
    return get(last, key) + ", " + get(first, key);
}

int
main()
{
    // Start by typedef'ing a bunch of stuff to make name shorter.
    typedef vector<string> Vector;
    typedef Vector::size_type Index;
    typedef Vector::iterator Iterator;

    // Define an identity map over the indices of the vector above.
    typedef identity_property_map<Index> IndexMap;

    // Define an iterator map over the Vector's iterator type and the
    // IndexMap that we just defined. Indices from the IndexMap will
    // provide offsets to an iterator from Vector.
    typedef iterator_property_map<Iterator, IndexMap> IteratorMap;

    // Allocaetd the storage of Person objects and configure a couple.
    Vector first_names(2);
    Vector last_names(2);
    first_names[0] = "Bob";
    first_names[1] = "Fred";
    last_names[0] = "Smith";
    last_names[1] = "Thompson";

    // Create property maps so that we can abstractly access the
    // Person objects stored in the vector above.
    IndexMap indices;
    IteratorMap first_map(first_names.begin(), indices);
    IteratorMap last_map(last_names.begin(), indices);

    // Print formatted names.
    cout << last_name_first(0, first_names, last_names) << endl;
    cout << last_name_first(1, first_names, last_names) << endl;

    return boost::exit_success;
}
//]
