// (C) Copyright 2003-2004: Reece H. Dunn

#ifndef BOOST_IOFM_DEDUCER_HPP
#define BOOST_IOFM_DEDUCER_HPP
#  include <boost/outfmt/detail/config.hpp>
#  if defined(BOOST_HAS_PRAGMA_ONCE)
#     pragma once
#  endif

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION)
#     include <boost/outfmt/detail/type_deducer.hpp>
#     include <boost/outfmt/detail/format_deducer.hpp>
#  else
#     error I/O Formatting Manipulators: type deduction is not supported on this configuration
#  endif
#endif
