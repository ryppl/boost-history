
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/function_types/function_type.hpp>
#include <boost/function_types/function_pointer.hpp>
#include <boost/function_types/function_reference.hpp>
#include <boost/function_types/is_callable_builtin.hpp>

namespace ft = boost::function_types;
namespace mpl = boost::mpl;

typedef ft::stdcall_cc cc;


BOOST_MPL_ASSERT((
  ft::is_callable_builtin< 
    ft::function_type<mpl::vector<void,int>, cc>::type 
  >
));

BOOST_MPL_ASSERT((
  ft::is_callable_builtin<
    ft::function_pointer<mpl::vector<void,int>, cc>::type 
  >
));

BOOST_MPL_ASSERT((
  ft::is_callable_builtin<
    ft::function_reference<mpl::vector<void,int>, cc>::type 
  >
));

