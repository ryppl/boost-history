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

#ifndef BOOST_IO_FORMAT_WRAP_HPP
#define BOOST_IO_FORMAT_WRAP_HPP

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// wrap_type
///////////////////////////////////////////////////////////////////////////////

template<class T>
class wrap_type
{
public:
  explicit wrap_type(const T &new_value) : v(new_value) {}

  const T &value() const
  {
    return v;
  }

private:
  const T &v;
};

///////////////////////////////////////////////////////////////////////////////
// wrap
///////////////////////////////////////////////////////////////////////////////

template<class T>
wrap_type<T> wrap(const T &value)
{
  return wrap_type<T>(value);
}

} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_WRAP_HPP
