// (C) Copyright 2003: Reece H. Dunn

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#ifndef BOOST_IOFM_DEDUCER_HPP
#define BOOST_IOFM_DEDUCER_HPP
#  include <boost/outfmt/detail/config.hpp>

#  if !defined(BOOST_IOFM_NO_OUTPUT_DEDUCTION)
#     include <boost/outfmt/detail/type_deducer.hpp>
#     include <boost/outfmt/detail/output_deducer.hpp>
#  else
#     error I/O Formatting Manipulators: type deduction is not supported on this configuration
#  endif
#endif
