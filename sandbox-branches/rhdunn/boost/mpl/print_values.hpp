//-----------------------------------------------------------------------------
// boost mpl/print_values.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_PRINT_VALUES_HPP_INCLUDED
#define BOOST_MPL_PRINT_VALUES_HPP_INCLUDED

#include <iosfwd>
#include "boost/mpl/for_each.hpp"

namespace boost {
namespace mpl {

namespace aux {

class value_printer
{
private: // representation

    std::ostream& out_;
    const char* delim_;

public: // structors

    value_printer(std::ostream& out, const char* delim)
        : out_(out)
        , delim_(delim)
    {
    }

public: // function interface

    template <typename T>
    void operator()(const T& t) const
    {
        out_ << t << delim_;
    }

};

} // namespace aux

template <typename Sequence>
void print_values(std::ostream& out, const char* delim = "\n")
{
    mpl::for_each< Sequence >( aux::value_printer(out,delim) );
}

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PRINT_VALUES_HPP_INCLUDED
