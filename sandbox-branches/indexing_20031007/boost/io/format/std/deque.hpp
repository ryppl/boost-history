///////////////////////////////////////////////////////////////////////////////
// Output operators and manipulators for composite objects
//
// Copyright Terje Slettebø 2003.
//
// Contributions by Vladimir Prus, Paul A. Bristow and Jason House.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_IO_FORMAT_STD_DEQUE_HPP
#define BOOST_IO_FORMAT_STD_DEQUE_HPP

#include <deque>
#include <boost/io/format/detail/sequence.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::deque)
///////////////////////////////////////////////////////////////////////////////

template output_operator_sequence<std::deque>;

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_STD_DEQUE_HPP
