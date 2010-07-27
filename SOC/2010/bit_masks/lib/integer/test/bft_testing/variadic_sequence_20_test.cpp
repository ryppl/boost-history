//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#define BOOST_BFT_PARAM_COUNT 20
#include <boost/integer/bitfield_tuple.hpp>
#include <boost/detail/lightweight_test.hpp>


using namespace boost;
using namespace boost::bitfields;

struct blue;


typedef bitfield_tuple<
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    align<8>,
    flag<blue>
>                                       test_tuple;

int main() {
    test_tuple temp;

    temp.get<blue>() = false;
    BOOST_TEST(( temp.get<blue>() == false ));
    return boost::report_errors();
}
