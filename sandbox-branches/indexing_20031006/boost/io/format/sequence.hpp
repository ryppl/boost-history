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

#ifndef BOOST_IO_FORMAT_SEQUENCE_HPP
#define BOOST_IO_FORMAT_SEQUENCE_HPP

#include <boost/io/format/format.hpp>
#include <boost/io/format/detail/sequence.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// sequence_type
///////////////////////////////////////////////////////////////////////////////

template<class T,class InputIterator>
class sequence_type
{
public:
  sequence_type(const InputIterator &b,const InputIterator &e) : begin(b),end(e) {}

  template<class CharType,class CharTraits>
  friend std::basic_ostream<CharType,CharTraits> &operator<<
    (std::basic_ostream<CharType,CharTraits> &stream,
     const sequence_type<T,InputIterator> &value)
  {
    const format_data<CharType> &format=detail::select_template_format<T,T>(stream);

    return detail::output_sequence(stream,format,value.begin,value.end);
  }

  const InputIterator &begin;
  const InputIterator &end;
};

///////////////////////////////////////////////////////////////////////////////
// sequence(begin,end)
///////////////////////////////////////////////////////////////////////////////

template<class T,class InputIterator>
sequence_type<T,InputIterator> sequence(const InputIterator &begin,const InputIterator &end)
{
  return sequence_type<T,InputIterator>(begin,end);
}

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_SEQUENCE_HPP
