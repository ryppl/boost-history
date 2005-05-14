//  Boost auto_temp_buf_example.cpp example file  ----------------------------//

//  Copyright 2003 Daryle Walker.  Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or a copy at <http://www.boost.org/LICENSE_1_0.txt>.)

//  See <http://www.boost.org/libs/memory/> for the library's home page.

//  Revision History
//   21 Jul 2003  Initial version (Daryle Walker)

#include <boost/memory/auto_temp_buf.hpp>  // for boost::memory::auto_temp_buf

#include <algorithm>  // for std::copy, std::min
#include <cassert>    // for assert
#include <cstddef>    // for std::size_t, std::ptrdiff_t
#include <iostream>   // for std::cout
#include <iterator>   // for std::ostream_iterator
#include <memory>     // for std::uninitialized_fill_n, etc.
#include <utility>    // for std::pair


// Types
typedef int                                         element_type;
typedef boost::memory::auto_temp_buf<element_type>  buffer_type;
typedef std::pair<element_type *, std::ptrdiff_t>   pair_type;


// Sample function that returns temporary buffers
buffer_type
get_scratch
(
    std::size_t  size
)
{
    return buffer_type( static_cast<std::ptrdiff_t>(size) );
}


// Main program
int
main
(
)
{
    using namespace std;

    size_t const  max_scratch_size = 100;
    if ( buffer_type  b = get_scratch(max_scratch_size) )
    {
        uninitialized_fill_n( b.get(), b.size(), 2 );

        buffer_type  c;
        assert( !c );

        pair_type  d = get_temporary_buffer<element_type>( max_scratch_size );
        if ( c = buffer_type(d.first, d.second) )
        {
            assert( c.valid() );
            uninitialized_fill_n( c.get(), c.size(), 3 );
            copy( b.get(), b.get() + std::min(b.size(), c.size()),
             raw_storage_iterator<element_type *, element_type>(c.get()) );

            buffer_type  e( c );
            assert( e && !c );

            buffer_type  f( e.release() );
            assert( f && !e );
            assert( (f.get() == d.first) && (f.size() == d.second) );
            copy( f.get(), f.get() + f.size(),
             ostream_iterator<element_type>(cout, " ") );
        }
    }

    return 0;
}
