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

#ifndef BOOST_IO_FORMAT_STD_MAP_HPP
#define BOOST_IO_FORMAT_STD_MAP_HPP

#include <map>
#include <boost/io/format/std/pair.hpp>
#include <boost/io/format/detail/sequence.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// output_operator_map
///////////////////////////////////////////////////////////////////////////////

template<template<class Key,class T,class Predicate = std::less<Key>,
         class Allocator = std::allocator<T> > class Map>
class output_operator_map
{
private:
  template<class CharType,class CharTraits,class Key,class T,class Predicate,class Allocator>
  friend std::basic_ostream<CharType,CharTraits> &operator<<
    (std::basic_ostream<CharType,CharTraits> &stream,
     const Map<Key,T,Predicate,Allocator> &value)
  {
    const format_data<CharType> &format=
      detail::select_template_format<Map<Key,T,Predicate,Allocator>,Map<_,_> >(stream);

    return detail::output_sequence(stream,format,value.begin(),value.end());
  }
};

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::map)
///////////////////////////////////////////////////////////////////////////////

template output_operator_map<std::map>;

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::multimap)
///////////////////////////////////////////////////////////////////////////////

template output_operator_map<std::multimap>;

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_STD_MAP_HPP
