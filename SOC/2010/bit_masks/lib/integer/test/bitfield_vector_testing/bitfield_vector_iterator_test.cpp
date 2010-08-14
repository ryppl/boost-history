//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost::detail;

/** Test suite for all 4 iterator types of a bitfield_vector.
 *  iterator
 *  const_iterator
 *  reverse_iterator
 *  const_reverse_iterator
 */
int main() {
    // bf_vector_iterator
    {
        typedef bf_vector_iterator<unsigned int, 3>             iter_1;
        typedef bf_vector_iterator<unsigned int, 9>             iter_2;
        typedef bf_vector_iterator<unsigned int, 10>            iter_3;


        // default constructor
        iter_1 i1;
        BOOST_TEST(!i1);

    }
    return boost::report_errors();
}
