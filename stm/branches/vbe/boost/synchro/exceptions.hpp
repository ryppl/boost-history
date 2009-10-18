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

#ifndef BOOST_SYNCHO_EXCEPTIONS_HPP
#define BOOST_SYNCHO_EXCEPTIONS_HPP

//-----------------------------------------------------------------------------
#include <stdexcept>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

    class lock_error : public std::logic_error {
    public:
        lock_error() : std::logic_error("synchro::lock_error") {}
    };

    class timeout_exception : public std::logic_error {
    public:
        timeout_exception() : std::logic_error("synchro::timeout_exception") {}
    };

}}
#endif // BOOST_SYNCHO_EXCEPTIONS_HPP


