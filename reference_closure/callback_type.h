// Copyright David Abrahams 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef CALLBACK_TYPE_DWA200933_H
# define CALLBACK_TYPE_DWA200933_H

# include "trivial_closure.h"

# if REFERENCE_CLOSURE

typedef closure callback_type;

# elif STD_FUNCTION

#  include <tr1/functional>
typedef std::tr1::function<void()> callback_type;

# endif

#endif // CALLBACK_TYPE_DWA200933_H
