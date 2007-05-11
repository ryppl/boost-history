// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_EXPERIMENTAL_CONVERSION_HPP
#define BOOST_UNITS_EXPERIMENTAL_CONVERSION_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>

namespace boost {

namespace units {

template<class Source, class Destination>
struct select_base_unit_converter {
    typedef Source source_type;
    typedef Destination destination_type;
};

template<class Source, class Destination>
struct base_unit_converter {
    typedef select_base_unit_converter<typename unscale<Source>::type, typename unscale<Destination>::type> selector;
    typedef typename selector::source_type source_type;
    typedef typename selector::destination_type destination_type;
    typedef base_unit_converter<source_type, destination_type> converter;
    typedef typename mpl::divides<typename get_scale_list<Source>::type, typename get_scale_list<source_type>::type>::type source_factor;
    typedef typename mpl::divides<typename get_scale_list<Destination>::type, typename get_scale_list<destination_type>::type>::type destination_factor;
    typedef typename mpl::divides<source_factor, destination_factor>::type factor;
    typedef typename eval_scale_list<factor> eval_factor;
    typedef typename multiply_typeof_helper<typename converter::type, typename eval_factor::type>::type type;
    static type value() {
        return(converter::value() * eval_factor::value());
    }
};

#define BOOST_UNITS_DEFINE_CONVERSION(Source, Destination, type_, value_)\
namespace boost {\
namespace units {\
template<>\
struct select_base_unit_converter<unscale<Source>::type,unscale<Destination>::type> {\
    typedef Source source_type;\
    typedef Desination destination_type;\
};\
template<>\
struct base_unit_converter<Source, Destination> {\
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
        typedef typename convert_heterogeneous_to_homogeneous_impl<N-1>::template apply<
            typename mpl::next<Begin>::type,
            DestinationSystem
        > next_iteration;
        typedef typename mpl::deref<Begin>::type unit_pair;
        typedef typename unit_pair::tag_type unit;
        typedef typename unit::dimensions dimensions;
        typedef typename reduce_unit<unit<dimensions, DestinationSystem> >::type reduced_unit;
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

}

template<class D, class L1, class T1, class L2, class T2>
struct conversion_helper<quantity<unit<D, compound_system<L1> >, T1>, quantity<unit<D, compound_system<L2> >, T2> > {
    typedef quantity<unit<D, compound_system<L2> >, T2> destination_type;
    typedef typename reduce_unit<unit<D, compound_system<L1> > >::type source_unit;
    typedef typename source_unit::system_type::type unit_list;
    static destination_type convert(const quantity<unit<D, compound_system<L1> >, T1>& source) {
        return(destination_type::from_value(source.value() * 
            detail::conversion_impl<mpl::size<unit_list>::value>::template apply<
                typename mpl::begin<unit_list>::type,
                compound_system<L2>
            >::value()
            ));
    }
};

template<class D, class L1, class T1, class L2, class T2>
struct conversion_helper<quantity<unit<D, heterogeneous_system<L1> >, T1>, quantity<unit<D, compound_system<L2> >, T2> > {
    typedef quantity<unit<D, compound_system<L2> >, T2> destination_type;
    static destination_type convert(const quantity<unit<D, heterogeneous_system<L1> >, T1>& source) {
        return(destination_type::from_value(source.value() * 
            detail::conversion_impl<mpl::size<typename L1::type>::value>::template apply<
                typename mpl::begin<typename L1::type>::type,
                compound_system<L2>
            >::value()
            ));
    }
};

// There is no simple algorithm for doing this conversion
// other than just defining it as the reverse of the
// heterogeneous->homogeneous case
template<class D, class L1, class T1, class L2, class T2>
struct conversion_helper<quantity<unit<D, compound_system<L1> >, T1>, quantity<unit<D, heterogeneous_system<L2> >, T2> > {
    typedef quantity<unit<D, heterogeneous_system<L2> >, T2> destination_type;
    static destination_type convert(const quantity<unit<D, heterogeneous_system<L1> >, T1>& source) {
        return(destination_type::from_value(source.value() /
            detail::conversion_impl<mpl::size<typename L2::type>::value>::template apply<
                typename mpl::begin<typename L2::type>::type,
                compound_system<L1>
            >::value()
            ));
    }
};

} //namespace units

} // namespace boost

#endif
