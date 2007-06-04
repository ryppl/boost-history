// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_HETEROGENEOUS_SYSTEM_HPP
#define BOOST_UNITS_HETEROGENEOUS_SYSTEM_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/front.hpp>

#include <boost/units/config.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/dimension.hpp>
#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/detail/push_front_if.hpp>
#include <boost/units/detail/linear_algebra.hpp>

namespace boost {

namespace units {

namespace detail {

// A normal system is a sorted list of base units.
// A heterogeneous system is a sorted list of base unit/exponent pairs.
// As long as we don't need to convert heterogeneous systems
// directly everything is cool

template<class T>
struct is_zero : mpl::false_ {};

template<>
struct is_zero<static_rational<0> > : mpl::true_ {};

}

/// INTERNAL ONLY
template<class L, class Dimensions>
struct heterogeneous_system_pair
{
    typedef L type;
    typedef Dimensions dimensions;
};

/// A system that can represent any possible combination
/// of units at the expense of not preserving information
/// about how it was created.  Do not create specializations
/// of this template directly. Instead use reduce_unit and
/// base_unit<...>::unit_type.
template<class T>
struct heterogeneous_system : T {};

/// INTERNAL ONLY
struct heterogeneous_system_dim_tag {};

/// INTERNAL ONLY
template<class Unit, class Exponent>
struct heterogeneous_system_dim
{
    typedef heterogeneous_system_dim_tag tag;
    typedef heterogeneous_system_dim type;
    typedef Unit tag_type;
    typedef Exponent value_type;
};

} // namespace units

} // namespace boost


#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TEMPLATE(boost::units::heterogeneous_system_pair, (class)(class))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::units::heterogeneous_system, (class))
BOOST_TYPEOF_REGISTER_TEMPLATE(boost::units::heterogeneous_system_dim, (class)(class))

#endif

namespace boost {

namespace units {

namespace detail {

template<class Unit1, class Exponent1, class Unit2, class Exponent2>
struct less<heterogeneous_system_dim<Unit1,Exponent1>,heterogeneous_system_dim<Unit2,Exponent2> > :
    mpl::less<Unit1, Unit2>
{};

template<class Unit1, class Exponent1>
struct is_empty_dim<heterogeneous_system_dim<Unit1,Exponent1> > : detail::is_zero<Exponent1> {};

} // namespace detail

} // namespace units

namespace mpl {

/// INTERNAL ONLY
template<>
struct plus_impl<boost::units::heterogeneous_system_dim_tag, boost::units::heterogeneous_system_dim_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::heterogeneous_system_dim<
            typename T0::tag_type,
            typename mpl::plus<typename T0::value_type,typename T1::value_type>::type
        > type;
    };
};

/// INTERNAL ONLY
template<>
struct times_impl<boost::units::heterogeneous_system_dim_tag, boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::heterogeneous_system_dim<
            typename T0::tag_type,
            typename mpl::times<typename T0::value_type,T1>::type
        > type;
    };
};

/// INTERNAL ONLY
template<>
struct divides_impl<boost::units::heterogeneous_system_dim_tag, boost::units::detail::static_rational_tag>
{
    template<class T0, class T1>
    struct apply
    {
        typedef boost::units::heterogeneous_system_dim<
            typename T0::tag_type,
            typename mpl::divides<typename T0::value_type,T1>::type
        > type;
    };
};

/// INTERNAL ONLY
template<>
struct negate_impl<boost::units::heterogeneous_system_dim_tag>
{
    template<class T>
    struct apply
    {
        typedef boost::units::heterogeneous_system_dim<typename T::tag_type, typename mpl::negate<typename T::value_type>::type> type;
    };
};

} // namespace mpl

namespace units {

namespace detail {

template<int N>
struct make_heterogeneous_system_impl
{
    template<class UnitsBegin, class ExponentsBegin>
    struct apply
    {
        typedef typename push_front_if<!(is_zero<typename mpl::deref<ExponentsBegin>::type>::value)>::template apply<
            typename make_heterogeneous_system_impl<N-1>::template apply<
                typename mpl::next<UnitsBegin>::type,
                typename mpl::next<ExponentsBegin>::type
            >::type,
            heterogeneous_system_dim<typename mpl::deref<UnitsBegin>::type, typename mpl::deref<ExponentsBegin>::type>
        >::type type;
    };
};

template<>
struct make_heterogeneous_system_impl<0>
{
    template<class UnitsBegin, class ExponentsBegin>
    struct apply
    {
        typedef dimensionless_type type;
    };
};

template<class Dimensions, class System>
struct make_heterogeneous_system
{
    typedef typename calculate_base_unit_exponents<typename System::type, Dimensions>::type exponents;
    typedef typename make_heterogeneous_system_impl<mpl::size<typename System::type>::value>::template apply<
        typename mpl::begin<typename System::type>::type,
        typename mpl::begin<exponents>::type
    >::type unit_list;
    typedef heterogeneous_system<heterogeneous_system_pair<unit_list,Dimensions> > type;
};

template<class Dimensions, class T>
struct make_heterogeneous_system<Dimensions, heterogeneous_system<T> >
{
    typedef heterogeneous_system<T> type;
};

template<class T0, class T1>
struct multiply_systems
{
    typedef heterogeneous_system<
        heterogeneous_system_pair<
            typename mpl::times<typename T0::type, typename T1::type>::type,
            typename mpl::times<typename T0::dimensions, typename T1::dimensions>::type
        >
    > type;
};

template<class T0, class T1>
struct divide_systems
{
    typedef heterogeneous_system<
        heterogeneous_system_pair<
            typename mpl::divides<typename T0::type, typename T1::type>::type,
            typename mpl::divides<typename T0::dimensions, typename T1::dimensions>::type
        >
    > type;
};

} // namespace detail

/// INTERNAL ONLY
template<class S, long N, long D>
struct static_power<heterogeneous_system<S>, static_rational<N,D> >
{
    typedef heterogeneous_system<
        heterogeneous_system_pair<
            typename static_power<typename S::type, static_rational<N,D> >::type,
            typename static_power<typename S::dimensions, static_rational<N,D> >::type
        >
    > type;
};

/// INTERNAL ONLY
template<class S, long N, long D>
struct static_root<heterogeneous_system<S>, static_rational<N,D> >
{
    typedef heterogeneous_system<
        heterogeneous_system_pair<
            typename static_root<typename S::type, static_rational<N,D> >::type,
            typename static_root<typename S::dimensions, static_rational<N,D> >::type
        >
    > type;
};

/// Returns a unique type for every unit.
template<class Unit>
struct reduce_unit
{
    typedef unit<
        typename Unit::dimension_type,
        typename detail::make_heterogeneous_system<
            typename Unit::dimension_type,
            typename Unit::system_type
        >::type
    > type;
};

namespace detail {

/// add an instantiation of dim to Sequence.
template<bool>
struct push_front_or_add_impl;

template<>
struct push_front_or_add_impl<true>
{
    template<typename Sequence, typename T>
    struct apply
    {
        typedef typename mpl::plus<T, typename mpl::front<Sequence>::type>::type item;
        typedef typename push_front_if<!is_empty_dim<item>::value>::template apply<
            typename mpl::pop_front<Sequence>::type,
            item
        > type;
    };
};

template<>
struct push_front_or_add_impl<false>
{
    template<typename Sequence, typename T>
    struct apply
    {
        typedef typename mpl::push_front<Sequence, T>::type type;
    };
};

template<typename Sequence, typename T>
struct push_front_or_add
{
    typedef typename push_front_or_add_impl<boost::is_same<typename T::tag_type, typename mpl::front<Sequence>::type::tag_type>::value>::template apply<
        Sequence,
        T
    >::type type;
};

template<typename T>
struct push_front_or_add<dimensionless_type, T>
{
    typedef typename mpl::push_front<dimensionless_type, T>::type type;
};

template<int N>
struct unscale_heterogeneous_system_impl
{
    template<class Begin>
    struct apply
    {
        typedef typename push_front_or_add<
            typename unscale_heterogeneous_system_impl<N-1>::template apply<
                typename mpl::next<Begin>::type
            >::type,
            typename unscale<typename mpl::deref<Begin>::type>::type
        >::type type;
    };
};

template<>
struct unscale_heterogeneous_system_impl<0>
{
    template<class Begin>
    struct apply
    {
        typedef dimensionless_type type;
    };
};

}

/// Unscale all the base units. e.g
/// km s -> m s
/// cm km -> m^2
/// INTERNAL ONLY
template<class T>
struct unscale<heterogeneous_system<T> >
{
    typedef heterogeneous_system<
        heterogeneous_system_pair<
            typename detail::unscale_heterogeneous_system_impl<
                mpl::size<typename T::type>::value
            >::template apply<
                typename mpl::begin<typename T::type>::type
            >::type,
            typename T::dimensions
        >
    > type;
};

/// INTERNAL ONLY
template<class Unit, class Exponent>
struct unscale<heterogeneous_system_dim<Unit, Exponent> >
{
    typedef heterogeneous_system_dim<typename unscale<Unit>::type, Exponent> type;
};

namespace detail {

template<int N>
struct get_scale_list_of_heterogeneous_system_impl
{
    template<class Begin>
    struct apply
    {
        typedef typename mpl::times<
            typename get_scale_list_of_heterogeneous_system_impl<N-1>::template apply<
                typename mpl::next<Begin>::type
            >::type,
            typename get_scale_list<typename mpl::deref<Begin>::type>::type
        >::type type;
    };
};

template<>
struct get_scale_list_of_heterogeneous_system_impl<0>
{
    template<class Begin>
    struct apply
    {
        typedef dimensionless_type type;
    };
};

} // namespace detail

/// INTERNAL ONLY
template<class T>
struct get_scale_list<heterogeneous_system<T> >
{
    typedef typename detail::get_scale_list_of_heterogeneous_system_impl<
        mpl::size<typename T::type>::value
    >::template apply<typename T::type>::type type;
};

/// INTERNAL ONLY
template<class Unit, class Exponent>
struct get_scale_list<heterogeneous_system_dim<Unit, Exponent> >
{
    typedef typename static_power<typename get_scale_list<Unit>::type, Exponent>::type type;
};

namespace detail {

template<class System, class Dimension>
struct check_system : mpl::false_ {};

template<class System, class Dimension>
struct check_system<heterogeneous_system<heterogeneous_system_pair<System, Dimension> >, Dimension> : mpl::true_ {};

} // namespace detail

} // namespace units

} // namespace boost

#endif
