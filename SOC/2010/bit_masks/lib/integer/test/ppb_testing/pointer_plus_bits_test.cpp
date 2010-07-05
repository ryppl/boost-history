//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/integer/pointer_plus_bits.hpp>

#include <boost/detail/lightweight_test.hpp>
#include <boost/type_traits.hpp>



int main() {
    // checking type defs.
    {
        typedef ::boost::pointer_plus_bits<int> temp;

        // difference type.
        BOOST_TEST((
            ::boost::is_same<
                temp::difference_type,
                std::ptrdiff_t
            >::value
        ));
        
        // value_type
        BOOST_TEST((
            ::boost::is_same<
                temp::value_type,
                int
            >::value
        ));

        // pointer
        BOOST_TEST((
            ::boost::is_same<
                temp::pointer,
                int*
            >::value
        ));

        // const_pointer
        BOOST_TEST((
            ::boost::is_same<
                temp::const_pointer,
                int const*
            >::value
        ));

        // reference
        BOOST_TEST((
            ::boost::is_same<
                temp::reference,
                int&
            >::value
        ));

        // const_reference
        BOOST_TEST((
            ::boost::is_same<
                temp::const_reference,
                int const&
            >::value
        ));
    
        // stuffed_bits
        BOOST_TEST( temp::stuffed_bits == 2);

        // mask_type
        BOOST_TEST((
            (
            boost::bit_width<temp::mask_type>::value
              ==
            boost::bit_width<boost::uint64_t>::value
              )||(
            boost::bit_width<boost::uint32_t>::value
              ==
            boost::bit_width<temp::mask_type>::value
            )
        ));

    }
    {
    }
    return boost::report_errors();
}
