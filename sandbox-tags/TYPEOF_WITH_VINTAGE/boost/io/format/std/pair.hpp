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

#ifndef BOOST_IO_FORMAT_STD_PAIR_HPP
#define BOOST_IO_FORMAT_STD_PAIR_HPP

#include <utility>
#include <boost/io/format/format.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::pair)
///////////////////////////////////////////////////////////////////////////////

template<class CharType,class CharTraits,class First,class Second>
std::basic_ostream<CharType,CharTraits> &operator<<
  (std::basic_ostream<CharType,CharTraits> &stream,const std::pair<First,Second> &value)
{
  const format_data<CharType> &format=
    detail::select_template_format<std::pair<First,Second>,std::pair<_,_> >(stream);

  return stream << format.start << format.start_element << value.first << format.end_element
                << format.delimiter << format.start_element << value.second << format.end_element
                << format.end;
}

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_STD_PAIR_HPP
