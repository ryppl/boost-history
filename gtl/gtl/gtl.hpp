/*
  Copyright 2008 Intel Corporation
 
  Use, modification and distribution are subject to the Boost Software License,
  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt).
*/
#ifndef GTL_GTL_HPP
#define GTL_GTL_HPP
//external
#include <math.h>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <algorithm>
#include <limits>
#include <iterator>

//isotropy types
#include "isotropy.hpp"

//data types
#include "point_data.hpp"
#include "interval_data.hpp"
#include "rectangle_data.hpp"
#include "point_3d_data.hpp"

//traits types
#include "point_traits.hpp"
#include "interval_traits.hpp"
#include "rectangle_traits.hpp"
#include "point_3d_traits.hpp"
#include "polygon_traits.hpp"
#include "polygon_with_holes_traits.hpp"

//concept types
#include "point_concept.hpp"
#include "interval_concept.hpp"
#include "rectangle_concept.hpp"
#include "point_3d_concept.hpp"

//algorithms needed by polygon types
#include "iterator_points_to_compact.hpp"
#include "iterator_compact_to_points.hpp"

#include "polygon_90_data.hpp"
#include "polygon_90_with_holes_data.hpp"
#include "polygon_45_data.hpp"
#include "polygon_45_with_holes_data.hpp"
#include "polygon_data.hpp"
#include "polygon_with_holes_data.hpp"

//polygon concept types
#include "polygon_90_concept.hpp"
#include "polygon_90_with_holes_concept.hpp"
#include "polygon_45_concept.hpp"
#include "polygon_45_with_holes_concept.hpp"
#include "polygon_concept.hpp"
#include "polygon_with_holes_concept.hpp"

//definitions
#include "post_concept_definitions.hpp"

//manhattan boolean op algorithms
#include "boolean_op.hpp"
#include "polygon_formation.hpp"
#include "rectangle_formation.hpp"
#include "property_merge.hpp"
#include "iterator_vertex_orient_conversion.hpp"
#include "iterator_geometry_to_set.hpp"

//geometry traits
#include "geometry_concept.hpp"

//polygon set trait types
#include "polygon_set_traits.hpp"
//polygon set concepts
#include "polygon_set_view.hpp"
//polygon set data types
#include "polygon_set_data.hpp"

#include "polygon_set_wrapper.hpp"

//defintions
#include "post_geometry_traits_definitions.hpp"

//general scanline
#include "scan_arbitrary.hpp"

namespace gtl {

//immutable unary functions

//accepts: point rectangle point_3d prism
template <typename geometry_type>
typename component_type<geometry_type>::type
get(const geometry_type& geometry_object, orientation_2d orient) {
  return geometry_concept<geometry_type>::type::get(geometry_object, orient);
}

//accepts: point_3d prism
template <typename geometry_type>
typename component_type<geometry_type>::type
get(const geometry_type& geometry_object, orientation_3d orient) {
  return geometry_concept<geometry_type>::type::get(geometry_object, orient);
}

//accepts: interval
template <typename geometry_type>
typename component_type<geometry_type>::type
get(const geometry_type& geometry_object, direction_1d dir) {
  return geometry_concept<geometry_type>::type::get(geometry_object, dir);
}

//accepts: interval
template <typename geometry_type>
typename component_type<geometry_type>::type
high(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::high(geometry_object);
}

//accepts: interval
template <typename geometry_type>
typename component_type<geometry_type>::type
low(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::low(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename component_type<geometry_type>::type
horizontal(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::horizontal(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename component_type<geometry_type>::type
vertical(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::vertical(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
xl(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::xl(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
yl(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::yl(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
xh(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::xh(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
yh(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::yh(geometry_object);
}

//accepts: interval
template <typename geometry_type>
typename center_type<geometry_type>::type
center(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::center(geometry_object);
}

//accepts: interval
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
delta(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::delta(geometry_object);
}

//accepts: rectangle prism
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
delta(const geometry_type& geometry_object, orientation_2d orient) {
  return geometry_concept<geometry_type>::type::delta(geometry_object, orient);
}

//accepts: prism
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
delta(const geometry_type& geometry_object, orientation_3d orient) {
  return geometry_concept<geometry_type>::type::delta(geometry_object, orient);
}

//accepts: rectangle, polygon
template <typename geometry_type>
typename area_type<geometry_type>::type
area(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::area(geometry_object);
}

//accepts: rectangle
template <typename geometry_type>
typename coordinate_type<geometry_type>::type
half_perimeter(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::half_perimeter(geometry_object);
}

//accepts: rectangle
template <typename geometry_type>
typename distance_type<geometry_type>::type
perimeter(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::perimeter(geometry_object);
}

template <typename geometry_type>
orientation_2d guess_orientation(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::guess_orientation(geometry_object);
}


//mutable unary functions

template <typename geometry_type>
void set(geometry_type& geometry_object, direction_1d dir, typename component_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::set(geometry_object, dir, value);
}

//accepts: point,coordinate point_3d,coordinate rectangle,interval prism,interval
template <typename geometry_type_1, typename geometry_type_2>
void set(geometry_type_1& geometry_object, orientation_2d orient, const geometry_type_2& value) {
  geometry_concept<geometry_type_1>::type::set(geometry_object, orient, value);
}

//accepts: point_3d,coordinate prism,interval
template <typename geometry_type_1, typename geometry_type_2>
void set(geometry_type_1& geometry_object, orientation_3d orient, const geometry_type_2& value) {
  geometry_concept<geometry_type_1>::type::set(geometry_object, orient, value);
}

template <typename geometry_type, typename point_type_1, typename point_type_2>
geometry_type& set_points(geometry_type& geometry_object, const point_type_1& point1, const point_type_2& point2) {
  return geometry_concept<geometry_type>::type::set_points(geometry_object, point1, point2);
}

template <typename geometry_type>
void high(geometry_type& geometry_object, typename component_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::high(geometry_object, value);
}

template <typename geometry_type>
void low(geometry_type& geometry_object, typename component_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::low(geometry_object, value);
}

//accepts: rectangle prism
template <typename geometry_type>
void xl(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::xl(geometry_object, value);
}

//accepts: rectangle prism
template <typename geometry_type>
void xh(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::xh(geometry_object, value);
}

//accepts: rectangle prism
template <typename geometry_type>
void yl(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::yl(geometry_object, value);
}

//accepts: rectangle prism
template <typename geometry_type>
void yh(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type value) {
  geometry_concept<geometry_type>::type::yh(geometry_object, value);
}

//accepts: rectangle prism
template <typename geometry_type_1, typename geometry_type_2>
void horizontal(geometry_type_1& geometry_object, const geometry_type_2& value) {
  geometry_concept<geometry_type_1>::type::horizontal(geometry_object, value);
}

//accepts: rectangle prism
template <typename geometry_type_1, typename geometry_type_2>
void vertical(geometry_type_1& geometry_object, const geometry_type_2& value) {
  geometry_concept<geometry_type_1>::type::vertical(geometry_object, value);
}

//immutable binary functions

template <typename geometry_type_1, typename geometry_type_2>
bool contains(const geometry_type_1& geometry_object, const geometry_type_2& contained_geometry_object, 
              bool consider_touch = true) {
  return geometry_concept<geometry_type_1>::type::contains(geometry_object, contained_geometry_object,
                                                           consider_touch, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool equivalence(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  typename geometry_concept<geometry_type_1>::type concept_instantiation;
  return concept_instantiation.equivalence(lvalue, rvalue);
}

template <typename geometry_type_1, typename geometry_type_2>
bool intersects(geometry_type_1& lvalue, geometry_type_2& rvalue, bool consider_touch = true) {
  return geometry_concept<geometry_type_1>::type::intersects(lvalue, rvalue, consider_touch, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool boundaries_intersect(geometry_type_1& lvalue, const geometry_type_2& rvalue, bool consider_touch = true) {
  return geometry_concept<geometry_type_1>::type::boundaries_intersect(lvalue, rvalue, consider_touch, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool abuts(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::abuts(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool abuts(geometry_type_1& lvalue, const geometry_type_2& rvalue, direction_1d dir) {
  return geometry_concept<geometry_type_1>::type::abuts(lvalue, rvalue, dir, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type>
geometry_type get_half(const geometry_type& geometry_object, direction_1d dir) {
  return geometry_concept<geometry_type>::type::get_half(geometry_object, dir);
}

//mutable binary functions

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& assign(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::assign(lvalue, rvalue);
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& convolve(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::convolve(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& deconvolve(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::deconvolve(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& reflected_convolve(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::reflected_convolve(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& reflected_deconvolve(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::reflected_deconvolve(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
typename distance_type<geometry_type_1>::type distance(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::distance(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool encompass(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::encompass(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool intersect(geometry_type_1& lvalue, geometry_type_2& rvalue, bool consider_touch = true) {
  return geometry_concept<geometry_type_1>::type::intersect(lvalue, rvalue, consider_touch, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
geometry_type_1& generalized_intersect(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::generalized_intersect(lvalue, rvalue, typename geometry_concept<geometry_type_2>::type());
}

template <typename geometry_type_1, typename geometry_type_2>
bool join_with(geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return geometry_concept<geometry_type_1>::type::join_with(lvalue, rvalue);
}

template <typename geometry_type>
geometry_type& flip(geometry_type& geometry_object, typename center_type<geometry_type>::type pivot) {
  return geometry_concept<geometry_type>::type::flip(geometry_object, pivot);
}

template <typename geometry_type>
geometry_type& scale(geometry_type& geometry_object, double factor) {
  return geometry_concept<geometry_type>::type::scale(geometry_object, factor);
}

template <typename geometry_type>
geometry_type& move(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type displacement) {
  return geometry_concept<geometry_type>::type::move(geometry_object, displacement);
}

//accepts: point point_3d rectangle prism
template <typename geometry_type>
geometry_type& move(geometry_type& geometry_object, orientation_2d orient,
                    typename coordinate_type<geometry_type>::type displacement) {
  return geometry_concept<geometry_type>::type::move(geometry_object, orient, displacement);
}

//accepts: point_3d prism
template <typename geometry_type>
geometry_type& move(geometry_type& geometry_object, orientation_3d orient,
                    typename coordinate_type<geometry_type>::type displacement) {
  int untested = 0;
  return geometry_concept<geometry_type>::type::move(geometry_object, orient, displacement);
}

template <typename geometry_type>
geometry_type& bloat(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type bloating) {
  return geometry_concept<geometry_type>::type::bloat(geometry_object, bloating);
}

template <typename geometry_type>
geometry_type& shrink(geometry_type& geometry_object, typename coordinate_type<geometry_type>::type shrinking) {
  return geometry_concept<geometry_type>::type::shrink(geometry_object, shrinking);
}

//polygonal functions

template <typename geometry_type>
rectangle_data<typename coordinate_type<geometry_type>::type> 
bounding_box(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::bounding_box(geometry_object);
}

template <typename geometry_type>
direction_1d winding(const geometry_type& geometry_object) {
  return geometry_concept<geometry_type>::type::winding(geometry_object);
}

template <typename geometry_type, typename rectangle_type>
void set_rectangle(geometry_type& geometry_object, const rectangle_type& rectangle) {
  geometry_concept<geometry_type>::type::set_rectangle(geometry_object, rectangle);
}

template <typename geometry_type, typename iterator_type>
void set_vertices(geometry_type& geometry_object, iterator_type input_begin, iterator_type input_end) {
  geometry_concept<geometry_type>::type::set(geometry_object, input_begin, input_end);
}

template <typename geometry_type, typename iterator_type>
void set_compact(geometry_type& geometry_object, iterator_type input_begin, iterator_type input_end) {
  geometry_concept<geometry_type>::type::set_compact(geometry_object, input_begin, input_end);
}

//boolean operator functions

template <typename geometry_type_1, typename geometry_type_2>
polygon_set_view<typename geometry_type_1::operator_arg_type,
                 typename geometry_type_2::operator_arg_type,
                 boolean_op::BinaryOr, 
                 typename geometry_type_1::operator_storage_tag, 
                 typename geometry_type_2::operator_storage_tag> 
operator|(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return polygon_set_view<geometry_type_1, geometry_type_2,
    boolean_op::BinaryOr, 
    typename geometry_type_1::operator_storage_tag, 
    typename geometry_type_2::operator_storage_tag>(lvalue, rvalue, 
                                                    polygon_set_traits<geometry_type_1>::orient(lvalue),
                                                    boolean_op::BinaryOr());
}

template <typename geometry_type_1, typename geometry_type_2>
polygon_set_view<typename geometry_type_1::operator_arg_type,
                 typename geometry_type_2::operator_arg_type,
                 boolean_op::BinaryOr, 
                 typename geometry_type_1::operator_storage_tag, 
                 typename geometry_type_2::operator_storage_tag> 
operator+(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return polygon_set_view<geometry_type_1, geometry_type_2,
    boolean_op::BinaryOr, 
    typename geometry_type_1::operator_storage_tag, 
    typename geometry_type_2::operator_storage_tag>(lvalue, rvalue, 
                                                    polygon_set_traits<geometry_type_1>::orient(lvalue),
                                                    boolean_op::BinaryOr());
}

template <typename geometry_type_1, typename geometry_type_2>
polygon_set_view<typename geometry_type_1::operator_arg_type,
                 typename geometry_type_2::operator_arg_type,
                 boolean_op::BinaryAnd, 
                 typename geometry_type_1::operator_storage_tag, 
                 typename geometry_type_2::operator_storage_tag> 
operator*(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return polygon_set_view<geometry_type_1, geometry_type_2,
    boolean_op::BinaryAnd, 
    typename geometry_type_1::operator_storage_tag, 
    typename geometry_type_2::operator_storage_tag>(lvalue, rvalue, 
                                                    polygon_set_traits<geometry_type_1>::orient(lvalue),
                                                    boolean_op::BinaryAnd());
}

template <typename geometry_type_1, typename geometry_type_2>
polygon_set_view<typename geometry_type_1::operator_arg_type,
                 typename geometry_type_2::operator_arg_type,
                 boolean_op::BinaryAnd, 
                 typename geometry_type_1::operator_storage_tag, 
                 typename geometry_type_2::operator_storage_tag> 
operator&(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return polygon_set_view<geometry_type_1, geometry_type_2,
    boolean_op::BinaryAnd, 
    typename geometry_type_1::operator_storage_tag, 
    typename geometry_type_2::operator_storage_tag>(lvalue, rvalue, 
                                                     polygon_set_traits<geometry_type_1>::orient(lvalue),
                                                     boolean_op::BinaryAnd());
}

template <typename geometry_type_1, typename geometry_type_2>
polygon_set_view<typename geometry_type_1::operator_arg_type,
                 typename geometry_type_2::operator_arg_type,
                 boolean_op::BinaryXor, 
                 typename geometry_type_1::operator_storage_tag, 
                 typename geometry_type_2::operator_storage_tag> 
operator^(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return polygon_set_view<geometry_type_1, geometry_type_2,
    boolean_op::BinaryXor, 
    typename geometry_type_1::operator_storage_tag, 
    typename geometry_type_2::operator_storage_tag>(lvalue, rvalue, 
                                                    polygon_set_traits<geometry_type_1>::orient(lvalue),
                                                    boolean_op::BinaryXor());
}
 
template <typename geometry_type_1, typename geometry_type_2>
polygon_set_view<typename geometry_type_1::operator_arg_type,
                 typename geometry_type_2::operator_arg_type,
                 boolean_op::BinaryNot, 
                 typename geometry_type_1::operator_storage_tag, 
                 typename geometry_type_2::operator_storage_tag> 
operator-(const geometry_type_1& lvalue, const geometry_type_2& rvalue) {
  return polygon_set_view<geometry_type_1, geometry_type_2,
    boolean_op::BinaryNot, 
    typename geometry_type_1::operator_storage_tag, 
    typename geometry_type_2::operator_storage_tag>(lvalue, rvalue, 
                                                    polygon_set_traits<geometry_type_1>::orient(lvalue),
                                                    boolean_op::BinaryNot());
}

}
#endif
