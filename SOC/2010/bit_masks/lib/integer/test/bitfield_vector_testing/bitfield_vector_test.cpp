//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_vector.hpp>
#include "test_utility.hpp"

using namespace boost;

/** This is done so that I can inherit from bitfield_vector and have access
 *  to the functions and members which are protectedly inherited from
 *  the bitfield_vector_base class.
 *
 *  This is only done so that testing can be done easier.
 *
 */
template <typename T, std::size_t W>
struct test_for_bitfield_vector
    :bitfield_vector<T, W>
{ };    

int main() {
    
    return boost::report_errors();
}
