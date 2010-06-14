//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



// boost includes.
#include <boost/integer/details/arbitrary_twos_complement.hpp>
#include <boost/assert.hpp>


using namespace boost;
int main() {
    // basic cast storing a negative number.
    {
        std::size_t storage = 0;
        char temp = -2;
        storage = twos_complement_encoding< char, 4, unsigned int >( temp );
        BOOST_ASSERT((storage == 0xe));

        temp = twos_complement_decoding< unsigned int, 4, char >( storage );
        BOOST_ASSERT((temp == -2));
    }
    // storing a non negative number with a signed type.
    {
        std::size_t storage = 0;
        char temp = 2;
        storage = twos_complement_encoding< char, 4, unsigned int >( temp );
        BOOST_ASSERT((storage == bits_mask<std::size_t, 1>::value ));

        temp = twos_complement_decoding< unsigned int, 4, char >( storage );
        BOOST_ASSERT((temp == 2));
    }
    // storing an unsigned number
    {
        std::size_t storage = 0;
        unsigned char temp = 2;
        storage = twos_complement_encoding< unsigned char, 4, unsigned int >( temp );
        BOOST_ASSERT((storage == bits_mask<std::size_t, 1>::value ));

        temp = twos_complement_decoding< unsigned int, 4, unsigned char >( storage );
        BOOST_ASSERT((temp == 2));
    }
    // corner case stroing a boolean
    {
        std::size_t storage = 0;
        bool temp  = true;
        storage = twos_complement_encoding< bool, 4, unsigned int >( temp );
        BOOST_ASSERT((storage == bits_mask<std::size_t, 0>::value ));

        temp = twos_complement_decoding< unsigned int, 4, bool >( storage );
        BOOST_ASSERT((temp == true));
    }

    // corner case storing a type with width one.
    {
        std::size_t storage = 0;
        char temp  = 1;
        storage = twos_complement_encoding< char, 1, unsigned int >( temp );
        BOOST_ASSERT((storage == bits_mask<std::size_t, 0>::value ));

        temp = twos_complement_decoding< unsigned int, 1, char >( storage );
        BOOST_ASSERT((temp == 1));
    }
    return 0;
}

