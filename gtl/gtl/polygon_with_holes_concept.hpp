/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/

#ifndef GTL_POLYGON_WITH_HOLES_CONCEPT_HPP
#define GTL_POLYGON_WITH_HOLES_CONCEPT_HPP
namespace gtl {

struct polygon_with_holes_concept : virtual polygon_concept, polygon_45_with_holes_concept {
public:
  inline polygon_with_holes_concept() {}
  //inherits its behaviors

  //needed to disambiguate between multiple base class implementations of same function
  template <typename polygon_with_holes_type_1, typename polygon_with_holes_type_2>
  static polygon_with_holes_type_1& assign(polygon_with_holes_type_1& lvalue, const polygon_with_holes_type_2& rvalue) {
    return polygon_45_with_holes_concept::assign(lvalue, rvalue);
  }
};

}
#endif
