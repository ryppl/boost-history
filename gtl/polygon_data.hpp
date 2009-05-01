/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_DATA_HPP
#define GTL_POLYGON_DATA_HPP
namespace gtl {
  struct polygon_concept;
  template <typename T>
  class polygon_data : public polygon_45_data<T> {
  public:
    typedef polygon_concept geometry_type;
  };
  


}
#endif

