
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief The exception namespace, and exception-related functions.
*/

#ifndef BOOST_INCLUDED_XINT_EXCEPTIONS_HPP
#define BOOST_INCLUDED_XINT_EXCEPTIONS_HPP

#include <stdexcept>
#include "detail/basic_types_and_includes.hpp"

namespace boost {
namespace xint {

/*! \brief Sets or resets the global exception handler for the library.

\param fn The function to set as the exception handler, of type \ref
on_exception_t. If not specified, removes any previous exception handler.

\note
This function, and the underlying exception handler system, is \e not
thread-safe. It is your responsibility to ensure that no other threads are using
the library while calling this function.

\see \ref exception_handler
*/
inline void on_exception(on_exception_t fn = on_exception_t()) {
    detail::exception_handler<>::fn = fn;
}

//! \brief Every exception the library throws is from this namespace.
namespace exceptions {

/*!
    Derived from \c std::invalid_argument.
*/
class invalid_argument: public std::invalid_argument {
    public:
    invalid_argument(const std::string& what="invalid argument"):
        std::invalid_argument(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class invalid_base: public std::invalid_argument {
    public:
    invalid_base(const std::string& what="invalid base"): invalid_argument(what)
        { }
};

/*!
    Derived from \c std::range_error.
*/
class invalid_digit: public std::range_error {
    public:
    invalid_digit(const std::string& what="invalid digit"): range_error(what)
        { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class invalid_modulus: public std::invalid_argument {
    public:
    invalid_modulus(const std::string& what="invalid modulus"):
        invalid_argument(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class divide_by_zero: public std::invalid_argument {
    public:
    divide_by_zero(const std::string& what="divide by zero error"):
        invalid_argument(what) { }
};

/*!
    Derived from \c std::invalid_argument.
*/
class cannot_represent: public std::invalid_argument {
    public:
    cannot_represent(const std::string& what=
        "cannot represent the requested value"): invalid_argument(what) { }
};

/*!
    Derived from \c std::range_error.
*/
class too_big: public std::range_error {
    public:
    too_big(const std::string& what=
        "value out of range for requested conversion"): range_error(what) { }
};

/*!
    Derived from \c std::runtime_error.
*/
class not_a_number: public std::runtime_error {
    public:
    not_a_number(const std::string& what="attempted to use a Not-a-Number"):
        runtime_error(what) { }
};

/*!
    Derived from \c std::runtime_error.

    \remarks
    Thrown by the xint::strong_random_generator class's constructor, if the
    system does not provide a cryptographically-secure random generator. Also
    thrown if the generator fails when asked to provide a random value, though
    this should be vanishingly rare.
*/
class no_strong_random: public std::runtime_error {
    public:
    no_strong_random(const std::string& what=
        "system does not have a strong random generator"):
        runtime_error(what) { }
};

/*!
    Derived from \c std::overflow_error.

    \remarks
    Thrown when the library can't get enough memory to represent a number.
*/
class overflow_error: public std::overflow_error {
    public:
    overflow_error(const std::string& what=
        "overflow error: can't obtain enough memory to represent the requested number"):
        std::overflow_error(what) { }
};

} // namespace exception
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_EXCEPTIONS_HPP
