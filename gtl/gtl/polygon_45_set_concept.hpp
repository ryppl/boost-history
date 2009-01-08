/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_POLYGON_45_SET_CONCEPT_HPP
#define GTL_POLYGON_45_SET_CONCEPT_HPP
namespace gtl {

  template <typename T, typename T2>
  struct is_either_polygon_45_set_type {
    typedef typename gtl_or<typename is_polygon_45_set_type<T>::type, typename is_polygon_45_set_type<T2>::type >::type type;
  };

  template <typename T>
  struct is_polygon_45_or_90_set_type {
    typedef typename gtl_or<typename is_polygon_45_set_type<T>::type, typename is_polygon_90_set_type<T>::type >::type type;
  };

  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_polygon_45_or_90_set_type<polygon_set_type>::type>::type,
                       typename polygon_45_set_traits<polygon_set_type>::iterator_type>::type
  begin_45_set_data(const polygon_set_type& polygon_set) {
    return polygon_45_set_traits<polygon_set_type>::begin(polygon_set);
  }
  
  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_polygon_45_or_90_set_type<polygon_set_type>::type>::type,
                       typename polygon_45_set_traits<polygon_set_type>::iterator_type>::type
  end_45_set_data(const polygon_set_type& polygon_set) {
    return polygon_45_set_traits<polygon_set_type>::end(polygon_set);
  }
  
  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_polygon_45_set_type<polygon_set_type>::type>::type,
                       bool>::type
  clean(const polygon_set_type& polygon_set) {
    return polygon_45_set_traits<polygon_set_type>::clean(polygon_set);
  }

  //assign
  template <typename polygon_set_type_1, typename polygon_set_type_2>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type_1>::type>::type,
                       typename gtl_if<typename is_polygon_45_or_90_set_type<polygon_set_type_2>::type>::type,
                       polygon_set_type_1>::type &
  assign(polygon_set_type_1& lvalue, const polygon_set_type_2& rvalue) {
    if(clean(rvalue))
      polygon_45_set_mutable_traits<polygon_set_type_1>::set(lvalue, begin_45_set_data(rvalue), end_45_set_data(rvalue));
    else {
      polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type_2>::coordinate_type> ps;
      ps.insert(begin_45_set_data(rvalue), end_45_set_data(rvalue));
      ps.clean();
      polygon_45_set_mutable_traits<polygon_set_type_1>::set(lvalue, ps.begin(), ps.end());
    }
    return lvalue;
  }

  //get trapezoids
  template <typename output_container_type, typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_polygon_45_set_type<polygon_set_type>::type>::type,
                       void>::type
  get_trapezoids(output_container_type& output, const polygon_set_type& polygon_set) {
    clean(polygon_set);
    polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type>::coordinate_type> ps;
    assign(ps, polygon_set);
    ps.get_trapezoids(output);
  }

  //equivalence
  template <typename polygon_set_type_1, typename polygon_set_type_2>
  typename requires_3< typename gtl_if<typename is_polygon_45_or_90_set_type<polygon_set_type_1>::type>::type,
                       typename gtl_if<typename is_polygon_45_or_90_set_type<polygon_set_type_2>::type>::type,
                       typename gtl_if<typename is_either_polygon_45_set_type<polygon_set_type_1, polygon_set_type_2>::type>::type,
                       bool>::type 
  equivalence(const polygon_set_type_1& lvalue,
              const polygon_set_type_2& rvalue) {
    polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type_1>::coordinate_type> ps1;
    assign(ps1, lvalue);
    polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type_2>::coordinate_type> ps2;
    assign(ps2, rvalue);
    return ps1 == ps2;
  }

  //clear
  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       void>::type
  clear(polygon_set_type& polygon_set) {
    polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type>::coordinate_type> ps;
    assign(polygon_set, ps);
  }

  //empty
  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       bool>::type
  empty(const polygon_set_type& polygon_set) {
    if(clean(polygon_set)) return begin_45_set_data(polygon_set) == end_45_set_data(polygon_set);
    polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type>::coordinate_type> ps;
    assign(ps, polygon_set);
    ps.clean();
    return ps.empty();
  }
 
  //extents
  template <typename polygon_set_type, typename rectangle_type>
  typename requires_2< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       typename is_mutable_rectangle_concept<typename geometry_concept<rectangle_type>::type>::type,
                       bool>::type
  extents(rectangle_type& extents_rectangle, 
          const polygon_set_type& polygon_set) {
    clean(polygon_set);
    polygon_45_set_data<typename polygon_45_set_traits<polygon_set_type>::coordinate_type> ps;
    assign(ps, polygon_set);
    return ps.extents(extents_rectangle);
  }

  //area
  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::area_type>::type
  area(const polygon_set_type& polygon_set) {
    typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
    typedef polygon_45_with_holes_data<Unit> p_type;
    typedef typename coordinate_traits<Unit>::area_type area_type;
    std::vector<p_type> polys;
    assign(polys, polygon_set);
    area_type retval = (area_type)0;
    for(unsigned int i = 0; i < polys.size(); ++i) {
      retval += area(polys[i]);
    }
    return retval;
  }

//   //self_intersect
//   template <typename polygon_set_type>
//   typename requires_1< typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
//                        polygon_set_type>::type &
//   self_intersect(polygon_set_type& polygon_set) {
//     typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
//     //TODO
//   }

  template <typename polygon_set_type, typename coord_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  resize(polygon_set_type& polygon_set, coord_type resizing, 
         RoundingOption rounding = CLOSEST, CornerOption corner = INTERSECTION) {
    typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
    clean(polygon_set);
    polygon_45_set_data<Unit> ps;
    assign(ps, polygon_set);
    ps.resize(resizing, rounding, corner);
    assign(polygon_set, ps);
    return polygon_set;
  }

  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  bloat(polygon_set_type& polygon_set, 
        typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type bloating) {
    return resize(polygon_set, bloating);
  }

  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  shrink(polygon_set_type& polygon_set, 
        typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type shrinking) {
    return resize(polygon_set, -(typename polygon_45_set_traits<polygon_set_type>::coordinate_type)shrinking);
  }

//   template <typename polygon_set_type>
//   typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
//                        polygon_set_type>::type &
//   grow_and(polygon_set_type& polygon_set, 
//         typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type bloating) {
//     typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
//     std::vector<polygon_45_data<Unit> > polys;
//     assign(polys, polygon_set);
//     clear(polygon_set);
//     polygon_45_set_data<Unit> ps;
//     for(unsigned int i = 0; i < polys.size(); ++i) {
//       polygon_45_set_data<Unit> tmpPs;
//       tmpPs.insert(polys[i]);
//       bloat(tmpPs, bloating);
//       tmpPs.clean(); //apply implicit OR on tmp polygon set
//       ps.insert(tmpPs);
//     }
//     self_intersect(ps);
//     assign(polygon_set, ps);
//     return polygon_set;
//   }

  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  scale_up(polygon_set_type& polygon_set, 
           typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type factor) {
    typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
    clean(polygon_set);
    polygon_45_set_data<Unit> ps;
    assign(ps, polygon_set);
    ps.scale_up(factor);
    assign(polygon_set, ps);
    return polygon_set;
  }

  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  scale_down(polygon_set_type& polygon_set, 
           typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type factor) {
    typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
    clean(polygon_set);
    polygon_45_set_data<Unit> ps;
    assign(ps, polygon_set);
    ps.scale_down(factor);
    assign(polygon_set, ps);
    return polygon_set;
  }

  //transform
  template <typename polygon_set_type, typename transformation_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  transform(polygon_set_type& polygon_set,
            const transformation_type& transformation) {
    typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
    clean(polygon_set);
    polygon_45_set_data<Unit> ps;
    assign(ps, polygon_set);
    ps.transform(transformation);
    assign(polygon_set, ps);
    return polygon_set;
  }

  //keep
  template <typename polygon_set_type>
  typename requires_1< typename gtl_if<typename is_mutable_polygon_45_set_type<polygon_set_type>::type>::type,
                       polygon_set_type>::type &
  keep(polygon_set_type& polygon_set, 
       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::area_type min_area,
       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::area_type max_area,
       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type min_width,
       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type max_width,
       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type min_height,
       typename coordinate_traits<typename polygon_45_set_traits<polygon_set_type>::coordinate_type>::unsigned_area_type max_height) {
    typedef typename polygon_45_set_traits<polygon_set_type>::coordinate_type Unit;
    typedef typename coordinate_traits<Unit>::unsigned_area_type uat;
    std::list<polygon_45_data<Unit> > polys;
    assign(polys, polygon_set);
    typename std::list<polygon_45_data<Unit> >::iterator itr_nxt;
    for(typename std::list<polygon_45_data<Unit> >::iterator itr = polys.begin(); itr != polys.end(); itr = itr_nxt){
      itr_nxt = itr;
      ++itr_nxt;
      rectangle_data<Unit> bbox;
      extents(bbox, *itr);
      uat pwidth = delta(bbox, HORIZONTAL);
      if(pwidth > min_width && pwidth <= max_width){
        uat pheight = delta(bbox, VERTICAL);
        if(pheight > min_height && pheight <= max_height){
          typename coordinate_traits<Unit>::area_type parea = area(*itr);
          if(parea <= max_area && parea >= min_area) {
            continue;
          }
        }
      }
      polys.erase(itr);
    }
    assign(polygon_set, polys);
    return polygon_set;
  }

}
#endif
