// Copyright David Abrahams 2003.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef PREFIX_DWA2003531_HPP
# define PREFIX_DWA2003531_HPP

// The rule is that <Python.h> must be included before any system
// headers (so it can get control over some awful macros).
// Unfortunately, Boost.Python needs to #include <limits.h> first, at
// least... but this gets us as close as possible.

# include <pyconfig.h>
# if defined(_SGI_COMPILER_VERSION) && _SGI_COMPILER_VERSION == 741
#  undef _POSIX_C_SOURCE
#  undef _XOPEN_SOURCE
# endif
# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/detail/config.hpp>

#endif // PREFIX_DWA2003531_HPP
