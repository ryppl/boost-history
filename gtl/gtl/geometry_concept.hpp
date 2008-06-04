/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_GEOMETRY_CONCEPT_HPP
#define GTL_GEOMETRY_CONCEPT_HPP
namespace gtl {
template <typename T>
struct geometry_concept { 
  typedef no_type type;
};

#define GTL_REGISTER_GEOMETRY_TRAITS(coordinate_type) \
template <>\
struct geometry_concept<point_data<coordinate_type> > {\
  typedef point_concept type;\
};\
template <>\
struct geometry_concept<rectangle_data<coordinate_type> > {\
  typedef rectangle_concept type;\
};\
template <>\
struct geometry_concept<interval_data<coordinate_type> > {\
  typedef interval_concept type;\
};\
template <>\
struct geometry_concept<point_3d_data<coordinate_type> > {\
  typedef point_3d_concept type;\
};\
template <>\
struct geometry_concept<polygon_90_data<coordinate_type> > {\
  typedef polygon_90_concept type;\
};\
template <>\
struct geometry_concept<polygon_90_with_holes_data<coordinate_type> > {\
  typedef polygon_90_with_holes_concept type;\
};\
template<>\
struct geometry_concept<polygon_formation::PolyLinePolygonData<true, coordinate_type> > {\
  typedef polygon_90_with_holes_concept type;\
};\
template<>\
struct geometry_concept<polygon_formation::PolyLinePolygonData<false, coordinate_type> > {\
  typedef polygon_90_with_holes_concept type;\
};\
template <>\
struct geometry_concept<polygon_formation::PolyLineHoleData<true, coordinate_type> > {\
  typedef polygon_90_concept type;\
};\
template <>\
struct geometry_concept<polygon_formation::PolyLineHoleData<false, coordinate_type> > {\
  typedef polygon_90_concept type;\
};\
template <>\
struct geometry_concept<polygon_45_data<coordinate_type> > {\
  typedef polygon_45_concept type;\
};\
template <>\
struct geometry_concept<polygon_45_with_holes_data<coordinate_type> > {\
  typedef polygon_45_with_holes_concept type;\
};\
template <>\
struct geometry_concept<polygon_data<coordinate_type> > {\
  typedef polygon_concept type;\
};\
template <>\
struct geometry_concept<polygon_with_holes_data<coordinate_type> > {\
  typedef polygon_with_holes_concept type;\
};\

//end of GTL_REGISTER_GEOMETRY_TRAITS macro

GTL_REGISTER_GEOMETRY_TRAITS(int)
GTL_REGISTER_GEOMETRY_TRAITS(long long)

}
#endif
