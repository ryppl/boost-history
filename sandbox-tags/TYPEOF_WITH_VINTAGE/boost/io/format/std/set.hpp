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

#ifndef BOOST_IO_FORMAT_STD_SET_HPP
#define BOOST_IO_FORMAT_STD_SET_HPP

#include <set>
#include <boost/io/format/detail/sequence.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// output_operator_set
///////////////////////////////////////////////////////////////////////////////

template<template<class Key,class Predicate = std::less<Key>,
         class Allocator = std::allocator<Key> > class Set>
class output_operator_set
{
private:
  template<class CharType,class CharTraits,class Key,class Predicate,class Allocator>
  friend std::basic_ostream<CharType,CharTraits> &operator<<
    (std::basic_ostream<CharType,CharTraits> &stream,
     const Set<Key,Predicate,Allocator> &value)
  {
    const format_data<CharType> &format=
      detail::select_template_format<Set<Key,Predicate,Allocator>,Set<_> >(stream);

    return detail::output_sequence(stream,format,value.begin(),value.end());
  }
};

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::set)
///////////////////////////////////////////////////////////////////////////////

template output_operator_set<std::set>;

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,std::multiset)
///////////////////////////////////////////////////////////////////////////////

template output_operator_set<std::multiset>;

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_STD_SET_HPP
