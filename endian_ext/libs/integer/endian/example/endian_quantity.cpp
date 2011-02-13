//  endian_hello_world_2.cpp  ----------------------------------------------------------------//

//  Copyright Beman Dawes, 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

#include <boost/integer/endian/endian_pack.hpp>
#include <boost/integer/endian/endian_binary_stream.hpp>
#include <boost/binary_stream.hpp>
#include <iostream>
#include <boost/units/io.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

using namespace boost;
using namespace boost::integer;
using namespace boost::integer::endianness;
using namespace boost::units;

int main()
{
    quantity<si::length, boost::int_least32_t> q = 825373492 * si::meter;
    endian_pack<big, quantity<si::length, boost::int_least32_t> > b;
    endian_pack<little, quantity<si::length, boost::int_least32_t> > l;
    b=q;
    l=q;

    //~ quantity<si::length, boost::int_least32_t> q2 = b;
    
    std::cout << "Hello, endian quantity!\n\n";

    //~ std::cout << q << '\n';
    //~ std::cout << q2 << '\n';
    std::cout << q << ' ' << b << ' ' << l << '\n';
    std::cout <= b <= ' ' <= l <= '\n';
    //~ std::cout <= q <= ' ' <= b <= ' ' <= l <= '\n';
    
    return 0;
}

