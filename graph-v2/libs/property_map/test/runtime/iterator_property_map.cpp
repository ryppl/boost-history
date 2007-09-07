// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  Author: Andrew Sutton

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <boost/test/minimal.hpp>
#include <boost/property_map/identity_property_map.hpp>
#include <boost/property_map/iterator_property_map.hpp>

using namespace std;
using namespace boost;
using namespace boost::property_map;

template <typename Sequence>
void build_test(Sequence& seq)
{
    for(size_t i = 0; i < 10; ++i) {
        seq.push_back(2 * i);
    }
}

template <typename Sequence>
void run_test(Sequence& seq)
{
    typedef typename Sequence::value_type Value;
    typedef typename Sequence::size_type Key;
    typedef typename Sequence::iterator Iterator;

    typedef identity_property_map<Key> IdentityMap;
    IdentityMap indices;

    typedef iterator_property_map<Iterator, IdentityMap> IteratorMap;
    IteratorMap values = make_iterator_property_map(seq.begin(), indices);

    // do some stuff
    cout << get(values, 3) << endl;
    put(values, 2, 10);
    cout << get(values, 2) << endl;
}

int test_main(int, char*[])
{
    {
        typedef vector<int> Vector;
        Vector v;
        build_test(v);
        run_test(v);
    }

    {
        typedef list<int> List;
        List l;
        build_test(l);
        run_test(l);
    }

    return 0;
}
