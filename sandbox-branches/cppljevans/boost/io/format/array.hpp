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

#ifndef BOOST_IO_FORMAT_ARRAY_HPP
#define BOOST_IO_FORMAT_ARRAY_HPP

#include <ostream>
#include <boost/io/format/format.hpp>
#include <boost/io/format/detail/sequence.hpp>

namespace boost {
namespace io {
namespace detail {

  ///////////////////////////////////////////////////////////////////////////////
  // select_array_format(stream)
  ///////////////////////////////////////////////////////////////////////////////

  template<class T,std::size_t Size,class CharType,class CharTraits>
  format_data<CharType> &select_array_format
    (std::basic_ostream<CharType,CharTraits> &stream)
  {
    return format_type<CharType,T (&)[Size]>::set() ?               // Exact type and size?
             format_type<CharType,T (&)[Size]>::format(stream) :
             format_type<CharType,_ (&)[Size]>::set() ?             // Any type, exact size?
               format_type<CharType,_ (&)[Size]>::format(stream) :
               format_type<CharType,T (&)[N]>::set() ?              // Exact type, any size?
                 format_type<CharType,T (&)[N]>::format(stream) :
                 format_type<CharType,_ (&)[N]>::set() ?            // Any type and size?
                   format_type<CharType,_ (&)[N]>::format(stream) :
                   format_type<CharType,_>::format(stream);
  }

} // namespace detail

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,T[N])
///////////////////////////////////////////////////////////////////////////////

template<class CharType,class CharTraits,class T,std::size_t Size>
std::basic_ostream<CharType,CharTraits> &operator<<
  (std::basic_ostream<CharType,CharTraits> &stream,T (&array)[Size])
{
  const format_data<CharType> &format=detail::select_array_format<T,Size>(stream);

  return detail::output_sequence(stream,format,array+0,array+Size);
}

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_ARRAY_HPP
