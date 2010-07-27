//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>

using namespace boost;
using namespace boost::bitfields;

struct red;
struct green;
struct pink;
struct blue;
struct salmon;
struct orange;

typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    padding<3>,
    member<int, salmon, 16>,
    flag<blue>
>                                       test_tuple;

typedef bitfields::detail::bitfield_element<char,
    red,
    mpl::size_t<0>,
    mpl::size_t<4>
>                       bftelem;

typedef test_tuple::bitfield_reference<
    find_by_element_name<
        test_tuple,
        red
    >::type
>                           ref_type;
int main() {
    // testing name_exists.
    {
        // name does exist
        BOOST_TEST(( name_exists<test_tuple,red>::type::value ));
        // name does not exist
        BOOST_TEST(( !name_exists<test_tuple,orange>::type::value ));
    }
    // find_by_element_name
    {
        typedef bitfields::detail::bitfield_element<char,
            red,
            mpl::size_t<0>,
            mpl::size_t<4>
        >                       bftelem;
        // name does exist
        BOOST_TEST((
            is_same<
                bftelem,
                find_by_element_name<
                    test_tuple,
                    red
                >::type
            >::type::value
        ));
    }

    // find_by_element_index
    {

        // name does exist
        BOOST_TEST((
            is_same<
                bftelem,
                find_by_element_index<
                    test_tuple,
                    0
                >::type
            >::type::value
        ));
    }

    // get_proxy_reference_type_by_name 
    {
        // get reference type
        BOOST_TEST((
            is_same<
                get_proxy_reference_type_by_name<test_tuple,red>::type,
                ref_type
            >::type::value
        ));
    }

    // get_proxy_reference_type_by_index 
    {
        // get reference type
        BOOST_TEST((
            is_same<
                get_proxy_reference_type_by_index<test_tuple,0>::type,
                ref_type
            >::type::value
        ));
    }
    return boost::report_errors();
}
