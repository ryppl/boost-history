//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_EXCEPTIONS__HPP
#define BOOST_STM_EXCEPTIONS__HPP

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <exception>
#include <stdexcept>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace boost { namespace stm {

class aborted_transaction_exception_no_unlocks {};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class aborted_transaction_exception 
    : public std::logic_error
{
public:
   aborted_transaction_exception(char const * const what) 
        : std::logic_error(what) {}

};

class aborted_transaction_exception_error 
    : public aborted_transaction_exception
{
public:
   aborted_transaction_exception_error(char const * const what) 
        : aborted_transaction_exception(what) {}

};

typedef aborted_transaction_exception aborted_tx;

struct not_implemented 
    : public std::logic_error {
   not_implemented(char const * const what) 
        : std::logic_error(what) {}

};

struct invalid_operation 
    : public std::logic_error {
   invalid_operation(char const * const what) 
        : std::logic_error(what) {}

};

}}

#endif // BOOST_STM_EXCEPTIONS__HPP


