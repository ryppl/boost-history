// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_DIM_HPP
#define BOOST_UNITS_DIM_HPP

#include <boost/static_assert.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/arithmetic.hpp>

#include <boost/units/config.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/detail/dim_impl.hpp>

/// \file 
/// \brief Handling of fundamental dimension/exponent pairs.

namespace boost {

namespace units {

namespace detail {

struct dim_tag { };

}

/// \brief Dimension tag/exponent pair for a single fundamental dimension.
///
/// \detailed 
/// The dim class represents a single dimension tag/dimension exponent pair.
/// That is, @c dim<tag_type,value_type> is a pair where @c tag_type represents the
/// fundamental dimension being represented and @c value_type represents the 
/// exponent of that fundamental dimension as a @c static_rational or other type 
/// providing the required compile-time arithmetic operations. @c tag_type must 
/// provide an ordinal value to allow sorting of lists of dims at compile-time.
/// This can be easily accomplished by inheriting from @c ordinal<N>. Otherwise,
/// @c tag_type may be any type. 
template<typename T,typename V> 
struct dim
{
    typedef dim             type;
    typedef detail::dim_tag tag;
    typedef T               tag_type;
    typedef V               value_type;
};

///// Check that a type is a valid @c dim.
//template<typename T> 
//struct is_dim :
//    public mpl::false_
//{ };
//
//template<typename T,typename V>
//struct is_dim< dim<T,V> > :
//    public mpl::true_
//{ };

} // namespace units

} // namespace boost

#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::units::dim, 2)

#endif

namespace boost {

namespace mpl {

template<>
struct plus_impl<boost::units::detail::dim_tag,boost::units::detail::dim_tag>
{
    template<class T0, class T1>
    struct apply
    {
        BOOST_STATIC_ASSERT((boost::is_same<typename T0::tag_type,typename T1::tag_type>::value == true));
        typedef boost::units::dim<typename T0::tag_type, typename mpl::plus<typename T0::value_type, typename T1::value_type>::type> type;
    };
};

template<>
struct minus_impl<boost::units::detail::dim_tag,boost::units::detail::dim_tag>
{
    template<class T0, class T1>
    struct apply
    {
        BOOST_STATIC_ASSERT((boost::is_same<typename T0::tag_type,typename T1::tag_type>::value == true));
        typedef boost::units::dim<typename T0::tag_type, typename mpl::minus<typename T0::value_type, typename T1::value_type>::type> type;
    };
};

template<>
struct times_impl<boost::units::detail::dim_tag,boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::dim<typename T0::tag_type, typename mpl::times<typename T0::value_type, T1>::type> type;
    };
};

template<>
struct times_impl<boost::units::detail::static_rational_tag,boost::units::detail::dim_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::dim<typename T1::tag_type, typename mpl::times<T0, typename T1::value_type>::type> type;
    };
};

template<>
struct divides_impl<boost::units::detail::dim_tag,boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::dim<typename T0::tag_type, typename mpl::divides<typename T0::value_type, T1>::type> type;
    };
};

template<>
struct divides_impl<boost::units::detail::static_rational_tag,boost::units::detail::dim_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::dim<typename T1::tag_type, typename mpl::divides<T0, typename T1::value_type>::type> type;
    };
};

template<>
struct negate_impl<boost::units::detail::dim_tag>
{
    template<class T0>
    struct apply
    {
        typedef boost::units::dim<typename T0::tag_type,typename mpl::negate<typename T0::value_type>::type> type;
    };
};

} // namespace mpl

} // namespace boost

#endif // BOOST_UNITS_DIM_HPP
