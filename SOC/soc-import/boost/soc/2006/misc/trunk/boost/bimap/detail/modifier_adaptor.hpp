// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/modifier_adaptor.hpp
/// \brief A binary to unary functor relation modifier adaptor.

#ifndef BOOST_BIMAP_DETAIL_MODIFIER_ADAPTOR_HPP
#define BOOST_BIMAP_DETAIL_MODIFIER_ADAPTOR_HPP

#include <functional>

namespace boost {
namespace bimap {
namespace detail {

/// \brief A binary to unary functor relation modifier adaptor.

template< class Modifier, class NewArgument, class FirstExtractor, class SecondExtractor >
struct relation_modifier_adaptor :
    public std::unary_function<NewArgument,bool>,
    Modifier,
    FirstExtractor,
    SecondExtractor
{
    relation_modifier_adaptor( Modifier m ) : Modifier(m) {}
    relation_modifier_adaptor( Modifier m, FirstExtractor fe, SecondExtractor se ) :
        Modifier(m), FirstExtractor(fe), SecondExtractor(se) {}

    void operator()( NewArgument & x ) const
    {
        Modifier::operator()( FirstExtractor::operator()( x ), SecondExtractor::operator()( x ) );
    }
};

} // namespace detail
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_MODIFIER_ADAPTOR_HPP
