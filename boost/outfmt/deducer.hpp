// (C) Copyright 2003: Reece H. Dunn

#ifndef BOOST__IOFM__DEDUCER__HPP
#define BOOST__IOFM__DEDUCER__HPP
#  include <boost/outfmt/detail/config.hpp>

#  if !defined(BOOST_IOFM__NO_OUTPUT_DEDUCTION)
#     include <boost/outfmt/detail/type_deducer.hpp>
#     include <boost/outfmt/detail/output_deducer.hpp>
#  else
#     error I/O Formatting Manipulators: type deduction is not supported on this configuration
#  endif
#endif
