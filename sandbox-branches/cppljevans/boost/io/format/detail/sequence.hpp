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

#ifndef BOOST_IO_FORMAT_DETAIL_SEQUENCE_HPP
#define BOOST_IO_FORMAT_DETAIL_SEQUENCE_HPP

#include <boost/io/format/format.hpp>

namespace boost {
namespace io {
namespace detail {

  ///////////////////////////////////////////////////////////////////////////////
  // output_sequence
  ///////////////////////////////////////////////////////////////////////////////

  template<class CharType,class CharTraits,class InputIterator>
  std::basic_ostream<CharType,CharTraits> &output_sequence
    (std::basic_ostream<CharType,CharTraits> &stream,
     const format_data<CharType> &format,const InputIterator &begin,const InputIterator &end)
  {
    stream << format.start;

    InputIterator i=begin;

    if(i!=end) {
      stream << format.start_element << *i << format.end_element;
      ++i;
    }

    for(; i != end; ++i)
      stream << format.delimiter << format.start_element << *i << format.end_element;

    return stream << format.end;
  }

} // namespace detail

///////////////////////////////////////////////////////////////////////////////
// output_operator_sequence
///////////////////////////////////////////////////////////////////////////////

template<template<class T,class = std::allocator<T> > class Sequence>
class output_operator_sequence
{
private:
  template<class CharType,class CharTraits,class T,class Allocator>
  friend std::basic_ostream<CharType,CharTraits> &operator<<
    (std::basic_ostream<CharType,CharTraits> &stream,
     const Sequence<T,Allocator> &value)
  {
    const format_data<CharType> &format=
      detail::select_template_format<Sequence<T,Allocator>,Sequence<_> >(stream);

    return detail::output_sequence(stream,format,value.begin(),value.end());
  }
};

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_DETAIL_SEQUENCE_HPP
