// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_STL_IO_HPP
#define BOOST_IOFM_STL_IO_HPP
#  include <boost/outfmt/formatob.hpp>
#  include <iostream>

#  include <vector> // std::vector
#  include <list>   // std::list
#  include <deque>  // std::deque
#  include <map>    // std::map, std::multimap
#  include <set>    // std::set, std::multiset

#  if defined(BOOST_IOFM_HASH_CONTAINERS)
#     include <hash_map> // std::hash_map, std::hash_multimap
#     include <hash_set> // std::hash_set, std::hash_multiset
#  endif

#  if defined(BOOST_HAS_SLIST)
#     include <slist>    // std::slist
#  endif

#  if !defined(BOOST_IOFM_NO_BASIC_STREAM)
#     include <boost/outfmt/detail/stl_io_basic.hpp> // std::basic_ostream supported
#  else
#     include <boost/outfmt/detail/stl_io.hpp>       // old I/O stream library implementation
#  endif
#endif
