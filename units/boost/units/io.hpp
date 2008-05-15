// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_IO_HPP
#define BOOST_UNITS_IO_HPP

#include <string>
#include <iosfwd>

#include <boost/mpl/size.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/units/heterogeneous_system.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/static_rational.hpp>
#include <boost/units/unit.hpp>

namespace boost {

namespace serialization {

/// Boost Serialization library support for units.
template<class Archive,class System,class Dim>
inline void serialize(Archive& ar,boost::units::unit<Dim,System>&,const unsigned int /*version*/)
{ }

/// Boost Serialization library support for quantities.
template<class Archive,class Unit,class Y>
inline void serialize(Archive& ar,boost::units::quantity<Unit,Y>& q,const unsigned int /*version*/)
{
    ar & boost::serialization::make_nvp("value", units::quantity_cast<Y&>(q));
}
        
} // namespace serialization

namespace units {

/// Write integral-valued @c static_rational to @c std::basic_ostream.
template<class Char, class Traits, integer_type N>
inline std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os,const static_rational<N>&)
{
    os << N;
    return os;
}

/// Write @c static_rational to @c std::basic_ostream.
template<class Char, class Traits, integer_type N, integer_type D>
inline std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os,const static_rational<N, D>&)
{
    os << '(' << N << '/' << D << ')';
    return os;
}

/// traits template for unit names
template<class BaseUnit>
struct base_unit_info
{
    /// The full name of the unit (returns BaseUnit::name() by default)
    static std::string name()
    {
        return(BaseUnit::name());
    }
    /// The symbol for the base unit (Returns BaseUnit::symbol() by default)
    static std::string symbol()
    {
        return(BaseUnit::symbol());
    }
};

namespace detail {

template<class T>
const T& adapt_for_print(const T& t)
{
    return(t);
}

template<class Char, class Traits, class Allocator>
const Char* adapt_for_print(const std::basic_string<Char, Traits, Allocator>& s)
{
    return(s.c_str());
}

template<class T, class Os>
void print_base_unit(Os& os, const T&)
{
    os << (adapt_for_print)(base_unit_info<typename T::tag_type>::symbol()) << '^' << typename T::value_type();
}

template<class Unit, class Os>
void print_base_unit(Os& os, const heterogeneous_system_dim<Unit, static_rational<1> >&)
{
    os << (adapt_for_print)(base_unit_info<Unit>::symbol());
}

template<int N>
struct print_impl
{
    template<class Begin, class Os>
    struct apply
    {
        typedef typename print_impl<N-1>::template apply<typename mpl::next<Begin>::type, Os> next;
        static void value(Os& os)
        {
            (print_base_unit)(os, typename mpl::deref<Begin>::type());
            os << ' ';
            next::value(os);
        }
    };
};

template<>
struct print_impl<1>
{
    template<class Begin, class Os>
    struct apply
    {
        static void value(Os& os)
        {
            (print_base_unit)(os, typename mpl::deref<Begin>::type());
        };
    };
};

template<>
struct print_impl<0>
{
    template<class Begin, class Os>
    struct apply
    {
        static void value(Os& os)
        {
            os << "dimensionless";
        }
    };
};

} // namespace detail

/// Print an @c unit as a list of base units and exponents e.g "m s^-1"
template<class Char, class Traits, class Dimension, class System>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, const unit<Dimension, System>&)
{
    os << typename reduce_unit<unit<Dimension, System> >::type();
    return(os);
}

/// INTERNAL ONLY
template<class Char, class Traits, class Dimension, class System>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, const unit<Dimension, heterogeneous_system<System> >&)
{
    detail::print_impl<mpl::size<typename System::type>::value>::template apply<
        typename mpl::begin<typename System::type>::type,
        std::basic_ostream<Char, Traits> >::value(os);
    return(os);
}

/// Print a @c quantity. Prints the value followed by the unit
template<class Char, class Traits, class Unit, class T>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, const quantity<Unit, T>& q)
{
    os << q.value() << ' ' << Unit();
    return(os);
}

} // namespace units

} // namespace boost

#endif
