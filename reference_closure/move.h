// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef MOVE_DWA200934_H
# define MOVE_DWA200934_H

namespace std
{
  template <class T>
  struct rvalue : T
  {};

  template <class T>
  inline rvalue<T>& move(T& x) { return static_cast<rvalue<T>&>(x); }
}

#endif // MOVE_DWA200934_H
