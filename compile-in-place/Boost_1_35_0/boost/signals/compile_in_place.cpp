/* compile in-place support for Boost.System

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// The code must be set up for in-place compilation
#if !defined(BOOST_ALL_COMPILE_IN_PLACE) && !defined(BOOST_SIGNALS_COMPILE_IN_PLACE)
#  error "this file should only be included when compiling Boost.Signals in place"
#endif

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_SIGNALS_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

#define BOOST_SIGNALS_COMPILE_IN_PLACE_CPP_INCLUDED


#include "../../libs/signals/src/connection.cpp"
#include "../../libs/signals/src/named_slot_map.cpp"
#include "../../libs/signals/src/signal_base.cpp"
#include "../../libs/signals/src/slot.cpp"
#include "../../libs/signals/src/trackable.cpp"
