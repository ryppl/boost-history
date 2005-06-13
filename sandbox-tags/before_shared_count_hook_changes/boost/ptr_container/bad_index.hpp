/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003-2004
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */

#ifndef BOOST_PTR_CONTAINER_BAD_INDEX_HPP
#define BOOST_PTR_CONTAINER_BAD_INDEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/ptr_container/bad_ptr_container_operation.hpp>

namespace boost
{
    class bad_index : public bad_ptr_container_operation
    {
    public:
        bad_index( const char* what ) : bad_ptr_container_operation( what )
        { }
    };
}

#endif
