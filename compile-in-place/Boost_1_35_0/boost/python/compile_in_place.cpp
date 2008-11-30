/* compile in-place support for Boost.Python

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// The code must be set up for in-place compilation
#if !defined(BOOST_ALL_COMPILE_IN_PLACE) && !defined(BOOST_PYTHON_COMPILE_IN_PLACE)
#  error "this file should only be included when compiling Boost.Python in place"
#endif

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_PYTHON_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

/* Note: this isn't a working include guard, but it's the best we can do here.
If the user includes this in more than one translation unit they will simply
get linker errors. */
#define BOOST_PYTHON_COMPILE_IN_PLACE_CPP_INCLUDED

#include "../../libs/python/src/dict.cpp"
#include "../../libs/python/src/errors.cpp"
#include "../../libs/python/src/exec.cpp"
#include "../../libs/python/src/import.cpp"
#include "../../libs/python/src/list.cpp"
#include "../../libs/python/src/long.cpp"
#include "../../libs/python/src/module.cpp"
#include "../../libs/python/src/numeric.cpp"
#include "../../libs/python/src/object_operators.cpp"
#include "../../libs/python/src/object_protocol.cpp"
#include "../../libs/python/src/slice.cpp"
#include "../../libs/python/src/str.cpp"
#include "../../libs/python/src/tuple.cpp"
#include "../../libs/python/src/wrapper.cpp"
#include "../../libs/python/src/converter/arg_to_python_base.cpp"
#include "../../libs/python/src/converter/builtin_converters.cpp"
#include "../../libs/python/src/converter/from_python.cpp"
#include "../../libs/python/src/converter/registry.cpp"
#include "../../libs/python/src/converter/type_id.cpp"
#include "../../libs/python/src/object/class.cpp"
#include "../../libs/python/src/object/enum.cpp"
#include "../../libs/python/src/object/function.cpp"
#include "../../libs/python/src/object/function_doc_signature.cpp"
#include "../../libs/python/src/object/inheritance.cpp"
#include "../../libs/python/src/object/life_support.cpp"
#include "../../libs/python/src/object/pickle_support.cpp"
#include "../../libs/python/src/object/stl_iterator.cpp"

