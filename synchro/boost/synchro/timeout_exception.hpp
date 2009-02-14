//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_TIMEOUT_EXCEPTION__HPP
#define BOOST_SYNCHRO_TIMEOUT_EXCEPTION__HPP

#include <exception>
#include <stdexcept>

namespace boost { namespace synchro {

    class timeout_exception : public std::logic_error {
    public:
        timeout_exception() : std::logic_error("timeout exception") {}
    };

    struct throw_timeout_t {};
    struct nothrow_timeout_t {};
    const throw_timeout_t throw_timeout ={};
    const nothrow_timeout_t no_throw_timeout ={};

}
}
#endif
