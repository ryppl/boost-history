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

#ifndef BOOST_IO_FORMAT_DETAIL_INDEX_ITERATOR_HPP
#define BOOST_IO_FORMAT_DETAIL_INDEX_ITERATOR_HPP

#include <cstddef>

namespace boost {
namespace io {
namespace detail {

  ///////////////////////////////////////////////////////////////////////////////
  // index_iterator
  ///////////////////////////////////////////////////////////////////////////////

  template<class ReturnType,class Sequence>
  class index_iterator
  {
  public:
    index_iterator(const Sequence &s,std::size_t i = 0) :
      sequence(s),index(i) {}

    ReturnType operator*() const
    {
      return sequence[index];
    }

    index_iterator &operator++()
    {
      ++index;

      return *this;
    }

    bool operator!=(const index_iterator &other) const
    {
      return index!=other.index;
    }

  private:
    const Sequence &sequence;
    std::size_t index;
  };

} // namespace detail
} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_DETAIL_INDEX_ITERATOR_HPP
