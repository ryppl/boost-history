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

#ifndef BOOST_IO_FORMAT_STD_COMPLEX_HPP
#define BOOST_IO_FORMAT_STD_COMPLEX_HPP

#include <complex>
#include <boost/io/format/wrap.hpp>
#include <boost/io/format/format.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::complex)
///////////////////////////////////////////////////////////////////////////////

template<class CharType,class CharTraits,class T>
std::basic_ostream<CharType,CharTraits> &operator<<
  (std::basic_ostream<CharType,CharTraits> &stream,const wrap_type<std::complex<T> > &value)
{
  const format_data<CharType> &format=
    detail::select_template_format<std::complex<T>,std::complex<_> >(stream);

  return stream << format.start << format.start_element << value.value().real() << format.end_element
                << format.delimiter << format.start_element << value.value().imag() << format.end_element
                << format.end;
}

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_STD_COMPLEX_HPP
