/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef 
#define 

#include <boost/mpl/bool.hpp>

namespace example
{
  struct example_sequence_tag;
}

namespace boost { namespace fusion 
{
  namespace extension
  {
    template<typename>
    struct is_view_impl;

    template<>
    struct is_view_impl<example::example_sequence_tag>
      : boost::mpl::false_
    {};
  }
}}

#endif
