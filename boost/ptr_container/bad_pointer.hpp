/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003
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

#ifndef BOOST_PTR_CONTAINER_BAD_POINTER_HPP
#define BOOST_PTR_CONTAINER_bAD_POINTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <exception>

namespace boost
{
    class bad_pointer : public std::exception
    {
    public:
        virtual const char* what() const throw()
        {
            return "Null pointer not allowed in a pointer container!";
        }
    };
}

#endif
