// (C) Copyright 2004: Reece H. Dunn. Distributed under the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/gui/log.hpp>

#if defined(_DEBUG) || defined(BOOST_GUI_ENABLE_LOG)
#  if !defined(BOOST_GUI_LOG_TO_STREAM)
      std::ofstream boost::gui::log( "boost-gui-log.txt" );
#  endif
#endif

#if defined(__GNUC__) && (__GNUC__ >= 3)
   int boost::detail::demangler::status = 0;
#endif
