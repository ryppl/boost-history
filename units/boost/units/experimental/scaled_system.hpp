// mcs::units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2007 Matthias Christian Schabel
// Copyright (C) 2007 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SCALED_SYSTEM_HPP_INCLUDED
#define BOOST_UNITS_SCALED_SYSTEM_HPP_INCLUDED

#include <string>

#include <boost/units/io.hpp>
#include <boost/units/conversion.hpp>

namespace boost {

namespace units {

template<long Base, long Exponent>
struct scale;

#define BOOST_UNITS_SCALE_SPECIALIZATION(base,exponent,val,name,symbol)   \
template<>                                                                  \
struct scale<base, exponent>                                                \
{                                                                           \
    typedef double value_type;                                              \
    static value_type value() { return(val); }                            \
    static std::string name_prefix() { return(#name); }                     \
    static std::string symbol_prefix() { return(#symbol); }                 \
}

#define BOOST_UNITS_SCALE_DEF(exponent,name,symbol) \
    BOOST_UNITS_SCALE_SPECIALIZATION(10,exponent,1e ## exponent, name, symbol) \
/*
BOOST_UNITS_SCALE_DEF(-24, yocto, y);
BOOST_UNITS_SCALE_DEF(-21, zepto, z);
BOOST_UNITS_SCALE_DEF(-18, atto, a);
BOOST_UNITS_SCALE_DEF(-15, femto, f);
BOOST_UNITS_SCALE_DEF(-12, pico, p);
BOOST_UNITS_SCALE_DEF(-9, nano, n);
BOOST_UNITS_SCALE_DEF(-6, micro, u);
BOOST_UNITS_SCALE_DEF(-3, milli, m);
BOOST_UNITS_SCALE_DEF(-2, centi, c);
BOOST_UNITS_SCALE_DEF(-1, deci, d);

BOOST_UNITS_SCALE_DEF(1, deka, da);
BOOST_UNITS_SCALE_DEF(2, hecto, h);
BOOST_UNITS_SCALE_DEF(3, kilo, k);
BOOST_UNITS_SCALE_DEF(6, mega, M);
BOOST_UNITS_SCALE_DEF(9, giga, G);
BOOST_UNITS_SCALE_DEF(12, tera, T);
BOOST_UNITS_SCALE_DEF(15, peta, P);
BOOST_UNITS_SCALE_DEF(18, exa, E);
BOOST_UNITS_SCALE_DEF(21, zetta, Z);
BOOST_UNITS_SCALE_DEF(24, yotta, Y);
*/
#undef BOOST_UNITS_SCALE_DEF
#undef BOOST_UNITS_SCALE_SPECIALIZATION

template<class S, class Scale>
struct scaled_system
{
    typedef S system_type;
    typedef Scale scale_type;
};

template<class T>
struct unscale {
    typedef T type;
};

template<class S, class Scale>
struct unscale<scaled_system<S, Scale> > {
    typedef S type;
};

template<class D, class S>
struct unit;

template<class D, class S>
struct unscale<unit<D, S> > {
    typedef unit<D, typename unscale<S>::type> type;
};

template<class DimensionTag, class System, class Scale>
struct base_unit_info<DimensionTag, scaled_system<System, Scale> >
{
    typedef base_unit_info<DimensionTag, System> base;
    static std::string name() { return(Scale::name_prefix() + base::name()); }
    static std::string symbol() { return(Scale::symbol_prefix() + base::symbol()); }
};

}

}

#endif
