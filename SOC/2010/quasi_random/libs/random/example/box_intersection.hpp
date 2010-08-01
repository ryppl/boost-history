// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef HEADER_BOX_INTERSECTION_HPP_INCLUDED
#define HEADER_BOX_INTERSECTION_HPP_INCLUDED

#include "vector_n.hpp"

namespace example {

// A simple ray structure
struct ray
{
  typedef math::vector_n<float, 3> vector_t;

  explicit ray(const vector_t& o, const vector_t& d)
    : origin(o)
    , direction(d)
    , inv_direction(1.f / d.x(), 1.f / d.y(), 1.f / d.z())
  {
    for(int i = 0; i < 3; ++i)
      sign[i] = (inv_direction(i) < 0.f);
  }

  vector_t origin;
  vector_t direction;
  vector_t inv_direction;
  int sign[3];
};


// Axis aligned box
class box
{
public:
  typedef math::vector_n<float, 3> vector_t;

  explicit box(const vector_t& minc, const vector_t& maxc)
  {
    bounds[0] = minc;
    bounds[1] = maxc;
  }

  explicit box(float width, float height, float depth)
  {
    bounds[0] = 0.f, 0.f, 0.f;
    bounds[1] = width, height, depth;
  }

  const vector_t& min_corner() const { return bounds[0]; }
  const vector_t& max_corner() const { return bounds[1]; }

  bool intersect(const ray& r, float t0, float t1) const
  {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    tmin = (bounds[r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
    tmax = (bounds[1-r.sign[0]].x() - r.origin.x()) * r.inv_direction.x();
    tymin = (bounds[r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
    tymax = (bounds[1-r.sign[1]].y() - r.origin.y()) * r.inv_direction.y();
    if ( (tmin > tymax) || (tymin > tmax) )
      return false;
    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;
    tzmin = (bounds[r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
    tzmax = (bounds[1-r.sign[2]].z() - r.origin.z()) * r.inv_direction.z();
    if ( (tmin > tzmax) || (tzmin > tmax) )
      return false;
    if (tzmin > tmin)
      tmin = tzmin;
    if (tzmax < tmax)
      tmax = tzmax;
    return ( (tmin < t1) && (tmax > t0) );
  }

  float surface_area() const
  {
    vector_t diagonal = bounds[1] - bounds[0];
    return 2.f * (diagonal.x()*diagonal.y() + diagonal.y()*diagonal.z() + diagonal.z()*diagonal.x());
  }

private:
  vector_t bounds[2];
};

} // namespace example

#endif // HEADER_BOX_INTERSECTION_HPP_INCLUDED
