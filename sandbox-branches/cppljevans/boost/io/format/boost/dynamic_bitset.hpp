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

#ifndef BOOST_IO_FORMAT_BOOST_DYNAMIC_BITSET_HPP
#define BOOST_IO_FORMAT_BOOST_DYNAMIC_BITSET_HPP

#include <ostream>
#include <boost/dynamic_bitset.hpp>
#include <boost/io/format/wrap.hpp>
#include <boost/io/format/format.hpp>
#include <boost/io/format/detail/sequence.hpp>
#include <boost/io/format/detail/index_iterator.hpp>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// operator<<(stream,boost::dynamic_bitset)
///////////////////////////////////////////////////////////////////////////////

template<class CharType,class CharTraits,class Block,class Allocator>
std::basic_ostream<CharType,CharTraits> &operator<<
  (std::basic_ostream<CharType,CharTraits> &stream,
   const wrap_type<boost::dynamic_bitset<Block,Allocator> > &value)
{
  const format_data<CharType> &format=
    detail::select_template_format<boost::dynamic_bitset<Block,Allocator>,
                                   boost::dynamic_bitset<> >(stream);

  return detail::output_sequence(stream,format,
    detail::index_iterator<bool,boost::dynamic_bitset<Block,Allocator> >(value.value()),
    detail::index_iterator<bool,boost::dynamic_bitset<Block,Allocator> >(value.value(),
      value.value().size()));
}

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_BOOST_DYNAMIC_BITSET_HPP
