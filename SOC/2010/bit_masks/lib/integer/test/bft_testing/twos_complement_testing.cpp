//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)



// boost includes.
#include <boost/integer/details/arbitrary_twos_complement.hpp>
#include <boost/assert.hpp>


using namespace boost;
int main() {
    {
        std::size_t storage = 0;
        char temp = -2;
        storage = twos_complement_encoding< char, 4, unsigned int >( temp );
        BOOST_ASSERT((storage == 0xe));

        temp = twos_complement_decoding< unsigned int, 4, char >( storage );
        BOOST_ASSERT((temp == -2));
    }
    return 0;
}

