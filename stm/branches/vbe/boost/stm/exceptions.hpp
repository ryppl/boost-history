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
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace boost { namespace stm {

class aborted_transaction_exception_no_unlocks {};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class aborted_transaction_exception : public std::exception
{
public:
   aborted_transaction_exception(char const * const what) : what_(what) {}

   //virtual char const * what() const { return what_; }

private:
   char const * const what_;
};

typedef aborted_transaction_exception aborted_tx;

}}

#endif // BOOST_STM_EXCEPTIONS__HPP


