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
struct salmon;
struct blue;

typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    padding<3>,
    member<int, salmon, 16>,
    flag<blue>,
    align<32>
>                  test_tuple_1;

int main() {
    {
        typedef bitfields::detail::disable_if_proxy_reference_type_by_name<
            const test_tuple_1,
            salmon
        >::type                 ref_t_1;
        std::size_t  store_house = 0;

        typedef bitfields::detail::disable_if_proxy_reference_type_by_name<
            test_tuple_1,
            salmon
        >::type                 ref_t_2;

        typedef bitfields::detail::enable_if_proxy_reference_type_by_index<
            test_tuple_1,
            0
        >::type                 ref_t_3;
        typedef bitfields::detail::enable_if_proxy_reference_type_by_index<
            const test_tuple_1,
            0
        >::type                 ref_t_4;
        //  test_tuple_1
        ref_t_3 non_const_ref_3(store_house);

        ref_t_2 non_const_ref(store_house);
        non_const_ref = 3;
        ref_t_1 const_ref(store_house);
        BOOST_TEST( const_ref == 3);

        non_const_ref_3 = 4;
        ref_t_3 const_ref_t_4(store_house);
        BOOST_TEST( const_ref_t_4 == 4 );
    }
    


    return boost::report_errors();
}
