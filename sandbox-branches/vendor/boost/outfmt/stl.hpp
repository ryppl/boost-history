// (C) Copyright 2003-2004: Reece H. Dunn
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_IOFM_STL_HPP
#define BOOST_IOFM_STL_HPP
#  include <boost/outfmt/detail/config.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  include <boost/outfmt/formatob.hpp>

#  include <boost/outfmt/stl/pair.hpp>
#  include <boost/outfmt/stl/complex.hpp>

#  include <boost/outfmt/stl/list.hpp>
#  include <boost/outfmt/stl/vector.hpp>
#  include <boost/outfmt/stl/deque.hpp>

#  if defined(BOOST_HAS_SLIST)
#     include <boost/outfmt/stl/slist.hpp>
#  endif

#  include <boost/outfmt/stl/map.hpp>
#  include <boost/outfmt/stl/set.hpp>

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
#     include <boost/outfmt/stl/hash_map.hpp>
#     include <boost/outfmt/stl/hash_set.hpp>
#  endif
#endif
