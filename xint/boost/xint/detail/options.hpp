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
    \brief Declares the arbitrary-length integer type.
*/

#ifndef BOOST_INCLUDED_XINT_INTEGER_OPTIONS_HPP
#define BOOST_INCLUDED_XINT_INTEGER_OPTIONS_HPP

namespace boost {
namespace xint {

BOOST_PARAMETER_TEMPLATE_KEYWORD(fixedlength_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(threadsafe_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(secure_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(nothrow_tag)
BOOST_PARAMETER_TEMPLATE_KEYWORD(allocator_tag)

using boost::is_base_of;
using boost::mpl::_;
using boost::parameter::binding;
using boost::parameter::optional;
using boost::parameter::deduced;

//! \brief Options for the integer_t template are stored in this namespace.
namespace options {

struct fixedlength_base { };
struct threadsafe_base { };
struct secure_base { };
struct nothrow_base { };

template <bitsize_t Bits> struct fixedlength: public fixedlength_base, public
	boost::mpl::integral_c<bitsize_t, Bits> { };
template <bool Value> struct threadsafe: public threadsafe_base, public
    boost::mpl::bool_<Value> { };
template <bool Value> struct secure: public secure_base, public
    boost::mpl::bool_<Value> { };
template <bool Value> struct nothrow: public nothrow_base, public
    boost::mpl::bool_<Value> { };

typedef parameter::parameters<
    optional<deduced<tag::fixedlength_tag>, is_base_of<fixedlength_base, _> >,
    optional<deduced<tag::threadsafe_tag>,  is_base_of<threadsafe_base, _> >,
    optional<deduced<tag::secure_tag>,      is_base_of<secure_base, _> >,
    optional<deduced<tag::nothrow_tag>,     is_base_of<nothrow_base, _> >,
    optional<deduced<tag::allocator_tag>,   is_class<_> >
> integer_signature;

} // namespace options
} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_INTEGER_OPTIONS_HPP
