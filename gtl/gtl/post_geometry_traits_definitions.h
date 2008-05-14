/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

template <typename geometry_type>
struct component_type { 
  typedef typename geometry_traits<geometry_type>::geometry_concept::template
  registration<geometry_type>::component_type type;
};

