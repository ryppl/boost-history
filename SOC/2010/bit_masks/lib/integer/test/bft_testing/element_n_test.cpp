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
typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    padding<3>,
    member<int, salmon, 16>,
    flag<blue>,
    bitfields::align<32>
>                                       test_tuple_1;

int main() {
    {
        typedef element<test_tuple_1,0>::type type;
        BOOST_TEST((
            is_same<
                bitfields::detail::enable_if_proxy_reference_type_by_index<test_tuple_1,0>::type,
                element<test_tuple_1,0>::type
            >::type::value
        ));
    }
    return boost::report_errors();
}
