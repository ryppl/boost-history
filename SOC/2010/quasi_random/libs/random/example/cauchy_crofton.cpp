// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#include <boost/random/niederreiter_base2.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/math/constants/constants.hpp>

#include <iostream>

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// Samples the surface area of a box using the Cauchy-Crofton method.
// The example is intentionally simple. It is important to note that while
// the surface area of a box is easy to compute, it is not always the case,
// especially if one needs to compute the surface area of an isosurface. This,
// for example, is useful when one computes solvent excluded protein surface area;
// in that case one would replace a 'box' with, for example, a 'protein_model'.

#include "box_intersection.hpp"


// In this example uniformly distributed chord entry and exit points on a S^2 ball
// are push_back'ed into a std::vector. This is not optimal for this particular example,
// but if you use a special device to do the sampling as, for instance, is possible
// with nVidia CUDA, you will need to store your points in heap anyway.
template<typename Engine, typename Vector, typename T>
void uniform_chords(Engine& gen, const Vector& center, T radius, std::size_t n_chords,
    std::vector<Vector>& points)
{
  const T two_pi = 2 * boost::math::constants::pi<T>();

  const std::size_t n_points = n_chords * 2;

  boost::uniform_real<T> rnd;

  Vector point_on_sphere;

  // Using formula from Rovira 05 to compute points uniformly
  // distributed on the surface of a sphere
  for(std::size_t i = 0; i != n_points; ++i)
  {
    T cos_theta = 1 - 2 * rnd(gen);
    T sin_theta = std::sqrt(1 - (cos_theta * cos_theta));
    T phi = two_pi * rnd(gen);
    T sin_phi = std::sin(phi), cos_phi = std::cos(phi); // consider using sincos function

    point_on_sphere = sin_theta*sin_phi, cos_theta, sin_theta*cos_phi;

    point_on_sphere *= radius;
    point_on_sphere += center;

    points.push_back(point_on_sphere);
  } // surface point generation
}


// Computes a bounding sphere for a given box and then counts a number of
// chord entry and exit points on the box.
template<typename Engine>
void cauchy_crofton(Engine& gen, const example::box& model, std::size_t n_chords)
{
  typedef typename example::box::vector_t vector_t;
  typedef typename vector_t::value_type value_t;

  std::cout << "Sample surface area, number of chords = " << n_chords << "\n";

  vector_t bounding_sphere_center = (model.min_corner() + model.max_corner()) * 0.5f;
  value_t bounding_sphere_radius = math::distance(bounding_sphere_center, model.max_corner());
  value_t bounding_sphere_area = 4 * boost::math::constants::pi<value_t>() *
      (bounding_sphere_radius * bounding_sphere_radius);

  std::vector<vector_t> points;

  // Generate uniformly distributed chord entry and exit points on a S^2 ball
  uniform_chords(gen, bounding_sphere_center, bounding_sphere_radius, n_chords, points);

  // Compute the number of chords that intersect a given Model
  std::size_t points_on_model = 0;
  for(std::size_t i = 0; i != points.size(); i += 2)
  {
    example::ray r(points[i], points[i+1] - points[i]);
    if( model.intersect(r, 0, 1) ) {
      points_on_model += 2; // here we do not account for those cases when there is
    }                       // only 1 entry/exit point.
  }

  value_t sampled_area =
      static_cast<value_t>(points_on_model) /
        static_cast<value_t>(points.size())
    * bounding_sphere_area;

  std::cout
    << "Bounding sphere area: " << bounding_sphere_area << "\n"
    << "Points on model: " << points_on_model << "\n"
    << "Points on sphere: " << points.size() << "\n"
    << "Sampled area: " << sampled_area << "\n"
    ;
}


int main()
{
  example::box aab(1.f, 0.3f, 0.2f); // a simple slightly oblong box

  std::cout << "The analytical surface area of a box: " << aab.surface_area() << "\n";

  boost::niederreiter_base2_4d gen_4d; // QRNG must be 4-dimensional!
  cauchy_crofton(gen_4d, aab, 500);

  gen_4d.seed(); // restart the generator in order to repeat the experiment above
  cauchy_crofton(gen_4d, aab, 5000);

  return 0;
}
