// Copyright 2005 Alexander Nasonov.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef FILE_boost_type_traits_floating_point_promotion_hpp_INCLUDED
#define FILE_boost_type_traits_floating_point_promotion_hpp_INCLUDED

#include <boost/config.hpp>

// Should be the last #include
#include <boost/type_traits/detail/type_trait_def.hpp>

namespace boost {

namespace type_traits { namespace detail {

template<class T>
struct floating_point_promotion
{
    typedef T type;
};

template<>
struct floating_point_promotion<float>
{
    typedef double type;
};

template<>
struct floating_point_promotion<float const>
{
    typedef double const type;
};

template<>
struct floating_point_promotion<float volatile>
{
    typedef double volatile type;
};

template<>
struct floating_point_promotion<float const volatile>
{
    typedef double const volatile type;
};

} }

BOOST_TT_AUX_TYPE_TRAIT_DEF1(
      floating_point_promotion
    , T
    , BOOST_DEDUCED_TYPENAME
        boost::type_traits::detail::floating_point_promotion<T>::type
    )
}

#include <boost/type_traits/detail/type_trait_undef.hpp>

#endif // #ifndef FILE_boost_type_traits_floating_point_promotion_hpp_INCLUDED

