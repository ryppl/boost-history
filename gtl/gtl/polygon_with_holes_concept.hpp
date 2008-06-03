/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

struct polygon_with_holes_concept : virtual polygon_concept, polygon_45_with_holes_concept {
public:
  inline polygon_with_holes_concept() {}
  //inherits its behaviors
};
