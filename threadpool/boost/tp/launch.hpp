//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_LAUNCH_H
#define BOOST_TP_LAUNCH_H

#include <boost/utility/result_of.hpp>

#include <boost/tp/default_pool.hpp>
#include <boost/tp/task.hpp>

namespace boost { namespace tp
{
template< typename Act >
task< typename result_of< Act() >::type > lauch_in_pool( Act const& act)
{ return get_default_pool().submit( act); }
} }

#endif // BOOST_TP_LAUNCH_H
