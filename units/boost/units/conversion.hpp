// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_CONVERSION_HPP
#define BOOST_UNITS_CONVERSION_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/divides.hpp>

#include <boost/units/scaled_base_unit.hpp>
#include <boost/units/make_system.hpp>
#include <boost/units/heterogeneous_system.hpp>
#include <boost/units/detail/one.hpp>
#include <boost/units/detail/static_rational_power.hpp>
#include <boost/units/detail/heterogeneous_conversion.hpp>

namespace boost {

namespace units {

template<class Dimension, class System>
class unit;

template<class Unit, class T>
class quantity;

template<class From, class To>
struct conversion_helper;

#ifdef BOOST_UNITS_DOXYGEN

/// Template for defining conversions between
/// quanties.  This template should be specialized
/// for every quantity that allows conversions.
/// For example, if you have a two units
/// called pair and dozen you would write
/// @code
/// namespace boost {
/// namespace units {
/// template<class T0, class T1>
/// struct conversion_helper<quantity<dozen, T0>, quantity<pair, T1> >
/// {
///     static quantity<pair, T1> convert(const quantity<dozen, T0>& source)
///     {
///         return(quantity<pair, T1>::from_value(6 * source.value()));
///     }
/// };
/// }
/// }
/// @endcode
template<class From, class To>
struct conversion_helper {
    static To convert(const From&);
};

#endif

/// INTERNAL ONLY
template<class Source, class Destination>
struct select_base_unit_converter {
    typedef Source source_type;
    typedef Destination destination_type;
};

/// INTERNAL ONLY
template<class Source, class Destination>
struct base_unit_converter {
    typedef select_base_unit_converter<typename unscale<Source>::type, typename unscale<Destination>::type> selector;
    typedef typename selector::source_type source_type;
    typedef typename selector::destination_type destination_type;
    typedef base_unit_converter<source_type, destination_type> converter;
    typedef typename mpl::divides<typename get_scale_list<Source>::type, typename get_scale_list<source_type>::type>::type source_factor;
    typedef typename mpl::divides<typename get_scale_list<Destination>::type, typename get_scale_list<destination_type>::type>::type destination_factor;
    typedef typename mpl::divides<source_factor, destination_factor>::type factor;
    typedef eval_scale_list<factor> eval_factor;
    typedef typename multiply_typeof_helper<typename converter::type, typename eval_factor::type>::type type;
    static type value() {
        return(converter::value() * eval_factor::value());
    }
};

namespace detail {

template<bool try_inverse, bool trivial>
struct inverse_base_unit_converter_impl;

template<>
struct inverse_base_unit_converter_impl<false, false> {
    template<class Source, class Destination>
    struct apply {
        typedef select_base_unit_converter<typename unscale<Source>::type, typename unscale<typename Destination::unit_type>::type> selector;
        typedef typename selector::source_type source_type;
        typedef typename selector::destination_type destination_type;
        typedef base_unit_converter<source_type, destination_type> converter;
        typedef typename mpl::divides<typename get_scale_list<Source>::type, typename get_scale_list<source_type>::type>::type source_factor;
        typedef typename mpl::divides<typename get_scale_list<Destination>::type, typename get_scale_list<destination_type>::type>::type destination_factor;
        typedef typename mpl::divides<source_factor, destination_factor>::type factor;
        typedef eval_scale_list<factor> eval_factor;
        typedef typename multiply_typeof_helper<typename converter::type, typename eval_factor::type>::type type;
        static type value() {
            return(converter::value() * eval_factor::value());
        }
    };
};

template<>
struct inverse_base_unit_converter_impl<true, false> {
    template<class Source, class Destination>
    struct apply {
        typedef base_unit_converter<Destination, typename Source::unit_type> inverse;
        typedef typename inverse::type type;
        static type value() {
            return(one()/inverse::value());
        }
    };
};

template<bool dummy>
struct inverse_base_unit_converter_impl<dummy, true> {
    template<class Source, class Destination>
    struct apply {
        typedef one type;
        static type value() { return(type()); }
    };
};

template<class Source, class Dest>
struct use_inverse_conversion {
    typedef select_base_unit_converter<typename unscale<Source>::type, typename unscale<typename Dest::unit_type>::type> selector;
    enum { value = (boost::is_same<Source, typename selector::source_type>::value) &&
        (boost::is_same<typename Dest::unit_type, typename selector::destination_type>::value) };
};

}

/// INTERNAL ONLY
template<class Source, class Dest>
struct base_unit_converter<
    Source,
    unit<
        typename Source::dimension_type,
        heterogeneous_system<
            heterogeneous_system_pair<
                dimension_list<heterogeneous_system_dim<Dest, static_rational<1> >, dimensionless_type>,
                typename Source::dimension_type
            >
        >
    >
> : detail::inverse_base_unit_converter_impl<detail::use_inverse_conversion<Source, Dest>::value, boost::is_same<Source, Dest>::value>::template apply<Source, Dest> {
};

/// Defines the conversion factor from a base unit to any unit
/// with the correct dimensions.  Must appear at global scope.
/// If the destination unit is a unit that contains only one
/// base unit which is raised to the first power (e.g. feet->meters)
/// the reverse need not be defined.
#define BOOST_UNITS_DEFINE_CONVERSION(Source, Destination, type_, value_)\
namespace boost {\
namespace units {\
template<>\
struct select_base_unit_converter<unscale<Source>::type,unscale<reduce_unit<Destination>::type>::type> {\
    typedef Source source_type;\
    typedef Destination destination_type;\
};\
template<>\
struct base_unit_converter<Source, reduce_unit<Destination>::type> {\
    typedef type_ type;\
    static type value() { return(value_); }\
};\
}\
}\
void boost_units_require_semicolon()

namespace detail {

template<int N>
struct conversion_impl {
    template<class Begin, class DestinationSystem>
    struct apply {
        typedef typename conversion_impl<N-1>::template apply<
            typename mpl::next<Begin>::type,
            DestinationSystem
        > next_iteration;
        typedef typename mpl::deref<Begin>::type unit_pair;
        typedef typename unit_pair::tag_type unit;
        typedef typename unit::dimension_type dimensions;
        typedef typename reduce_unit<units::unit<dimensions, DestinationSystem> >::type reduced_unit;
        typedef base_unit_converter<unit, reduced_unit> converter;
        typedef typename multiply_typeof_helper<typename converter::type, typename next_iteration::type>::type type;
        static type value() { return(static_rational_power<typename unit_pair::value_type>(converter::value()) * next_iteration::value()); }
    };
};

template<>
struct conversion_impl<0> {
    template<class Begin, class DestinationSystem>
    struct apply {
        typedef one type;
        static type value() { return(one()); }
    };
};

} // namespace detail

/// conversions between homogeneous systems are defined
template<class D, class L1, class T1, class L2, class T2>
struct conversion_helper<quantity<unit<D, homogeneous_system<L1> >, T1>, quantity<unit<D, homogeneous_system<L2> >, T2> > {
    /// INTERNAL ONLY
    typedef quantity<unit<D, homogeneous_system<L2> >, T2> destination_type;
    /// INTERNAL ONLY
    typedef typename reduce_unit<unit<D, homogeneous_system<L1> > >::type source_unit;
    /// INTERNAL ONLY
    typedef typename source_unit::system_type::type unit_list;
    static destination_type convert(const quantity<unit<D, homogeneous_system<L1> >, T1>& source) {
        return(destination_type::from_value(source.value() * 
            detail::conversion_impl<mpl::size<unit_list>::value>::template apply<
                typename mpl::begin<unit_list>::type,
                homogeneous_system<L2>
            >::value()
            ));
    }
};

/// conversions between heterogeneous systems and homogeneous systems are defined
template<class D, class L1, class T1, class L2, class T2>
struct conversion_helper<quantity<unit<D, heterogeneous_system<L1> >, T1>, quantity<unit<D, homogeneous_system<L2> >, T2> > {
    /// INTERNAL ONLY
    typedef quantity<unit<D, homogeneous_system<L2> >, T2> destination_type;
    static destination_type convert(const quantity<unit<D, heterogeneous_system<L1> >, T1>& source) {
        return(destination_type::from_value(source.value() * 
            detail::conversion_impl<mpl::size<typename L1::type>::value>::template apply<
                typename mpl::begin<typename L1::type>::type,
                homogeneous_system<L2>
            >::value()
            ));
    }
};

// There is no simple algorithm for doing this conversion
// other than just defining it as the reverse of the
// heterogeneous->homogeneous case
/// conversions between heterogeneous systems and homogeneous systems are defined
template<class D, class L1, class T1, class L2, class T2>
struct conversion_helper<quantity<unit<D, homogeneous_system<L1> >, T1>, quantity<unit<D, heterogeneous_system<L2> >, T2> > {
    /// INTERNAL ONLY
    typedef quantity<unit<D, heterogeneous_system<L2> >, T2> destination_type;
    static destination_type convert(const quantity<unit<D, homogeneous_system<L1> >, T1>& source) {
        return(destination_type::from_value(source.value() /
            detail::conversion_impl<mpl::size<typename L2::type>::value>::template apply<
                typename mpl::begin<typename L2::type>::type,
                homogeneous_system<L1>
            >::value()
            ));
    }
};

template<class D, class S1, class T1, class S2, class T2>
struct conversion_helper<quantity<unit<D, heterogeneous_system<S1> >, T1>, quantity<unit<D, heterogeneous_system<S2> >, T2> > {
    typedef quantity<unit<D, heterogeneous_system<S1> >, T1> source_type;
    typedef quantity<unit<D, heterogeneous_system<S2> >, T2> destination_type;
    typedef typename detail::extract_base_units<mpl::size<typename S1::type>::value>::template apply<typename mpl::begin<typename S1::type>::type, mpl::list0<> >::type from_base_units;
    typedef typename detail::extract_base_units<mpl::size<typename S2::type>::value>::template apply<typename mpl::begin<typename S2::type>::type, from_base_units>::type all_base_units;
    typedef typename detail::make_homogeneous_system<all_base_units>::type system;
    static destination_type convert(const source_type& source) {
        return(destination_type::from_value(source.value() * 
            (detail::conversion_impl<mpl::size<typename S1::type>::value>::template apply<
                typename mpl::begin<typename S1::type>::type,
                system
            >::value() /
            detail::conversion_impl<mpl::size<typename S2::type>::value>::template apply<
                typename mpl::begin<typename S2::type>::type,
                system
            >::value()
            )
            ));
    }
};

namespace detail {

template<class Source, class Dest>
struct conversion_factor_helper;

template<class D, class L1, class L2>
struct conversion_factor_helper<unit<D, homogeneous_system<L1> >, unit<D, homogeneous_system<L2> > > {
    typedef typename reduce_unit<unit<D, homogeneous_system<L1> > >::type source_unit;
    typedef typename source_unit::system_type::type unit_list;
    typedef typename detail::conversion_impl<mpl::size<unit_list>::value>::template apply<
        typename mpl::begin<unit_list>::type,
        homogeneous_system<L2>
    > impl;
    typedef typename impl::type type;
    static type value() {
        return(impl::value());
    }
};

template<class D, class L1, class L2>
struct conversion_factor_helper<unit<D, heterogeneous_system<L1> >, unit<D, homogeneous_system<L2> > > {
    typedef typename detail::conversion_impl<mpl::size<typename L1::type>::value>::template apply<
        typename mpl::begin<typename L1::type>::type,
        homogeneous_system<L2>
    > impl;
    typedef typename impl::type type;
    static type convert() {
        return(impl::value());
    }
};

// There is no simple algorithm for doing this conversion
// other than just defining it as the reverse of the
// heterogeneous->homogeneous case
template<class D, class L1, class L2>
struct conversion_factor_helper<unit<D, homogeneous_system<L1> >, unit<D, heterogeneous_system<L2> > > {
    typedef typename detail::conversion_impl<mpl::size<typename L2::type>::value>::template apply<
        typename mpl::begin<typename L2::type>::type,
        homogeneous_system<L1>
    > impl;
    typedef typename impl::type type;
    static type value() {
        return(one() / impl::value());
    }
};

}

/// Find the conversion factor between two units.
template<class FromUnit,class ToUnit>
inline
typename detail::conversion_factor_helper<FromUnit, ToUnit>::type
conversion_factor(const FromUnit&,const ToUnit&)
{
    return(detail::conversion_factor_helper<FromUnit, ToUnit>::value());
}

/// Find the conversion factor between two units with an explicit return type.
/// e.g. conversion_factor<int>(newton, dyne) returns 100000
template<class Y, class FromUnit,class ToUnit>
inline
Y
conversion_factor(const FromUnit&,const ToUnit&)
{
    return(static_cast<Y>(detail::conversion_factor_helper<FromUnit, ToUnit>::value()));
}

} //namespace units

} // namespace boost

#endif
