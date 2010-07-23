/* boost random/sobol.hpp header file
 *
 * Copyright Justinas Vygintas Daugmaudis 2010
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_RANDOM_SOBOL_HPP
#define BOOST_RANDOM_SOBOL_HPP

#include <boost/random/detail/gray_coded_qrng_base.hpp>
#include <boost/random/detail/config.hpp>
#include <boost/random/detail/operators.hpp>

#include <limits>
#include <boost/cstdint.hpp>
#include <boost/integer/static_log2.hpp>

#include <boost/static_assert.hpp>

#include <boost/mpl/vector/vector40_c.hpp>
#include <boost/mpl/at.hpp>

//!\file
//!Describes the quasi-random number generator class template sobol.
//!
//!\b Note: it is especially useful in conjunction with class template uniform_real.

namespace boost {
namespace random {

namespace detail {

namespace sbl {

// Primitive polynomials in binary encoding

template<std::size_t D>
struct primitive_polynomial;

/// @cond
#define BOOST_SOBOL_PRIM_POLY(D, V) \
  template<> \
   struct primitive_polynomial<D> { \
     static const int value = V; \
     static const int degree = ::boost::static_log2<V>::value; \
   } \
/// @endcond

BOOST_SOBOL_PRIM_POLY(0, 1);
BOOST_SOBOL_PRIM_POLY(1, 3);
BOOST_SOBOL_PRIM_POLY(2, 7);
BOOST_SOBOL_PRIM_POLY(3, 11);
BOOST_SOBOL_PRIM_POLY(4, 13);
BOOST_SOBOL_PRIM_POLY(5, 19);
BOOST_SOBOL_PRIM_POLY(6, 25);
BOOST_SOBOL_PRIM_POLY(7, 37);
BOOST_SOBOL_PRIM_POLY(8, 59);
BOOST_SOBOL_PRIM_POLY(9, 47);
BOOST_SOBOL_PRIM_POLY(10, 61);
BOOST_SOBOL_PRIM_POLY(11, 55);
BOOST_SOBOL_PRIM_POLY(12, 41);
BOOST_SOBOL_PRIM_POLY(13, 67);
BOOST_SOBOL_PRIM_POLY(14, 97);
BOOST_SOBOL_PRIM_POLY(15, 91);
BOOST_SOBOL_PRIM_POLY(16, 109);
BOOST_SOBOL_PRIM_POLY(17, 103);
BOOST_SOBOL_PRIM_POLY(18, 115);
BOOST_SOBOL_PRIM_POLY(19, 131);
BOOST_SOBOL_PRIM_POLY(20, 193);
BOOST_SOBOL_PRIM_POLY(21, 137);
BOOST_SOBOL_PRIM_POLY(22, 145);
BOOST_SOBOL_PRIM_POLY(23, 143);
BOOST_SOBOL_PRIM_POLY(24, 241);
BOOST_SOBOL_PRIM_POLY(25, 157);
BOOST_SOBOL_PRIM_POLY(26, 185);
BOOST_SOBOL_PRIM_POLY(27, 167);
BOOST_SOBOL_PRIM_POLY(28, 229);
BOOST_SOBOL_PRIM_POLY(29, 171);
BOOST_SOBOL_PRIM_POLY(30, 213);
BOOST_SOBOL_PRIM_POLY(31, 191);
BOOST_SOBOL_PRIM_POLY(32, 253);
BOOST_SOBOL_PRIM_POLY(33, 203);
BOOST_SOBOL_PRIM_POLY(34, 211);
BOOST_SOBOL_PRIM_POLY(35, 239);
BOOST_SOBOL_PRIM_POLY(36, 247);
BOOST_SOBOL_PRIM_POLY(37, 285);
BOOST_SOBOL_PRIM_POLY(38, 369);
BOOST_SOBOL_PRIM_POLY(39, 299);

#undef BOOST_SOBOL_PRIM_POLY


// inirandom sets up the random-number generator to produce a Sobol
// sequence of at most max dims-dimensional quasi-random vectors.
// Adapted from ACM TOMS algorithm 659, see

// http://doi.acm.org/10.1145/42288.214372

template<std::size_t D>
struct vinit40;

template<> struct vinit40<1> {
  typedef mpl::vector40_c<int,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1> type;
};

template<> struct vinit40<2>
{
  typedef mpl::vector40_c<int,
    0, 0, 1, 3, 1, 3, 1, 3, 3, 1,
    3, 1, 3, 1, 3, 1, 1, 3, 1, 3,
    1, 3, 1, 3, 3, 1, 3, 1, 3, 1,
    3, 1, 1, 3, 1, 3, 1, 3, 1, 3> type;
};

template<> struct vinit40<3>
{
  typedef mpl::vector40_c<int,
    0, 0, 0, 7, 5, 1, 3, 3, 7, 5,
    5, 7, 7, 1, 3, 3, 7, 5, 1, 1,
    5, 3, 3, 1, 7, 5, 1, 3, 3, 7,
    5, 1, 1, 5, 7, 7, 5, 1, 3, 3> type;
};

template<> struct vinit40<4>
{
  typedef mpl::vector40_c<int,
    0,  0,  0,  0,  0,  1,  7,  9, 13, 11,
    1,  3,  7,  9,  5, 13, 13, 11,  3, 15,
    5,  3, 15,  7,  9, 13,  9,  1, 11,  7,
    5, 15,  1, 15, 11,  5,  3,  1,  7,  9> type;
};

template<> struct vinit40<5>
{
  typedef mpl::vector40_c<int,
    0,  0,  0,  0,  0,  0,  0,  9,  3, 27,
    15, 29, 21, 23, 19, 11, 25,  7, 13, 17,
     1, 25, 29,  3, 31, 11,  5, 23, 27, 19,
    21,  5,  1, 17, 13,  7, 15,  9, 31,  9> type;
};

template<> struct vinit40<6>
{
  typedef mpl::vector40_c<int,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0, 37, 33,  7,  5, 11, 39, 63,
   27, 17, 15, 23, 29,  3, 21, 13, 31, 25,
    9, 49, 33, 19, 29, 11, 19, 27, 15, 25> type;
};

template<> struct vinit40<7>
{
  typedef mpl::vector40_c<int,
    0,   0,  0,  0,  0,  0,    0,  0,  0,   0,
    0,   0,  0,  0,  0,  0,    0,  0,  0,  13,
   33, 115, 41, 79, 17,  29, 119, 75, 73, 105,
    7,  59, 65, 21,  3, 113,  61, 89, 45, 107> type;
};

template<> struct vinit40<8>
{
  typedef mpl::vector40_c<int,
    0, 0, 0, 0, 0, 0, 0, 0,  0,  0,
    0, 0, 0, 0, 0, 0, 0, 0,  0,  0,
    0, 0, 0, 0, 0, 0, 0, 0,  0,  0,
    0, 0, 0, 0, 0, 0, 0, 7, 23, 39> type;
};


template<std::size_t D, std::size_t Iteration>
struct leading_elements
{
  template<typename T, int BitCount, std::size_t Dimension>
  static void assign(T (&cj)[BitCount][Dimension])
  {
    typedef typename vinit40<D>::type elems_t;
    typedef typename mpl::at_c<elems_t, Iteration>::type val_t;
    cj[D - 1][Iteration] = val_t::value;
    leading_elements<D - 1, Iteration>::assign(cj);
  }
};

template<std::size_t Iteration>
struct leading_elements<0, Iteration>
{
  template<typename T, int BitCount, std::size_t Dimension>
  static void assign(T (&)[BitCount][Dimension])
  {}
};


template<std::size_t D>
struct compute_lattice
{
  BOOST_STATIC_ASSERT( D != 0 );

  template<typename T, int BitCount, std::size_t Dimension>
  void operator()(T (&cj)[BitCount][Dimension]) const
  {
    enum {
      iteration  = Dimension - D + 1,
      px_value   = primitive_polynomial<iteration>::value,
      degree_i   = primitive_polynomial<iteration>::degree
    };

    // Leading elements for dimension i come from vinit<>.
    //for(int k = 0; k < degree_i; ++k )
    //  cj[k][iteration] = v_init[k][iteration];
    leading_elements<degree_i, iteration>::assign(cj);

    // Expand the polynomial bit pattern to separate
    // components of the logical array includ[].
    int p_i = px_value;
    bool includ[degree_i];
    for( int k = degree_i-1; k >= 0; --k, p_i >>= 1 )
      includ[k] = (p_i & 1);

    // Calculate remaining elements for this dimension,
    // as explained in Bratley+Fox, section 2.
    for(int j = degree_i; j < BitCount; ++j)
    {
      T p = 2;
      T w = cj[j - degree_i][iteration];
      for(int k = 0; k < degree_i; ++k, p <<= 1)
        if( includ[k] )
          w ^= (cj[j-k-1][iteration] * p);
      cj[j][iteration] = w;
    }

    compute_lattice<D - 1> ncj; ncj(cj);
  }
};

template<>
struct compute_lattice<1>
{
  template<typename T, int BitCount, std::size_t Dimension>
  void operator()(T (&)[BitCount][Dimension]) const
  {
    // recursion stop
  }
};

} // namespace sbl

template<typename IntType, std::size_t Dimension>
struct sobol_lattice
{
#if defined(BOOST_NO_STATIC_ASSERT)
  BOOST_STATIC_ASSERT( Dimension <= 40 );
#else
  static_assert(Dimension <= 40, "The Sobol quasi-random number generator only supports 40 dimensions.");
#endif

  typedef IntType result_type;
  BOOST_STATIC_CONSTANT(std::size_t, dimension_value = Dimension);

  BOOST_STATIC_CONSTANT(int, bit_count = std::numeric_limits<IntType>::digits - 1);

  // default copy c-tor is fine

  sobol_lattice(std::size_t) // c-tor to initialize the lattice
  {
    // Initialize direction table in dimension 0.
    for( int k = 0; k < bit_count; ++k )
      bits[k][0] = 1;

    // Initialize in remaining dimensions.
    sbl::compute_lattice<Dimension> compute; compute(bits);

    // Multiply columns of v by appropriate power of 2.
    IntType p = 2;
    for(int j = bit_count-1-1; j >= 0; --j, p <<= 1)
      for(std::size_t k = 0; k < Dimension; ++k )
        bits[j][k] *= p;
  }

  result_type operator()(int i, int j) const
  {
    return bits[i][j];
  }

private:
  IntType bits[bit_count][Dimension];
};

} // namespace detail

//!class template sobol implements a quasi-random number generator as described in
//! \blockquote
//![Bratley+Fox, TOMS 14, 88 (1988)]
//!and [Antonov+Saleev, USSR Comput. Maths. Math. Phys. 19, 252 (1980)]
//! \endblockquote
//!
//!\attention \b Important: This implementation supports up to 40 dimensions.
//!
//!In the following documentation @c X denotes the concrete class of the template
//!sobol returning objects of type @c IntType, u and v are the values of @c X.
//!
//!Some member functions may throw exceptions of type @c std::overflow_error. This
//!happens when the quasi-random domain is exhausted and the generator cannot produce
//!any more values. The length of the low discrepancy sequence is given by
//! \f$L=Dimension \times 2^{digits - 1}\f$, where digits = std::numeric_limits<IntType>::digits.
template<typename IntType, std::size_t Dimension, IntType c, IntType m>
class sobol : public detail::gray_coded_qrng_base<
                       sobol<IntType, Dimension, c, m>,
                       detail::sobol_lattice<IntType, Dimension> >
{
  typedef sobol<IntType, Dimension, c, m> self_t;
  typedef detail::sobol_lattice<IntType, Dimension> lattice_t;
  typedef detail::gray_coded_qrng_base<self_t, lattice_t> base_t;

public:
  typedef IntType result_type;

  /** @copydoc boost::random::niederreiter_base2::min() */
  static result_type min BOOST_PREVENT_MACRO_SUBSTITUTION () { return c == 0u ? 1u: 0u; }

  /** @copydoc boost::random::niederreiter_base2::max() */
  static result_type max BOOST_PREVENT_MACRO_SUBSTITUTION () { return m - 1u; }

  /** @copydoc boost::random::niederreiter_base2::dimension() */
  static std::size_t dimension() { return Dimension; }

  //!Effects: Constructs the default Sobol quasi-random number generator.
  //!
  //!Throws: nothing.
  sobol()
    : base_t()
  {}

  //!Effects: Constructs the Sobol quasi-random number generator,
  //!equivalent to X u; u.seed(init).
  //!
  //!Throws: overflow_error.
  explicit sobol(std::size_t init)
    : base_t(init)
  {}

  // default copy c-tor is fine

  /** @copydoc boost::random::niederreiter_base2::seed() */
  void seed()
  {
    base_t::reset_state();
  }

  /** @copydoc boost::random::niederreiter_base2::seed(std::size_t) */
  void seed(std::size_t init)
  {
    base_t::seed(init, "sobol::seed");
  }

  //=========================Doxygen needs this!==============================

  //!Requirements: *this is mutable.
  //!
  //!Returns: Returns a successive element of an s-dimensional
  //!(s = X::dimension()) vector at each invocation. When all elements are
  //!exhausted, X::operator() begins anew with the starting element of a
  //!subsequent s-dimensional vector.
  //!
  //!Throws: overflow_error.

  // Fixed in Doxygen 1.7.0 -- id 612458: Fixed problem handling @copydoc for function operators.
  result_type operator()()
  {
    return base_t::operator()();
  }

  /** @copydoc boost::random::niederreiter_base2::discard(std::size_t) */
  void discard(std::size_t z)
  {
    base_t::discard(z);
  }

  //!Writes a @c sobol to a @c std::ostream.
  BOOST_RANDOM_DETAIL_OSTREAM_OPERATOR(os, sobol, f)
  { return f.write_impl(os); }
  //!Reads a @c niederreiter_base2 from a @c std::istream.
  BOOST_RANDOM_DETAIL_ISTREAM_OPERATOR(is, sobol, f)
  { return f.read_impl(is); }

  //!Returns true if the two generators will produce identical sequences.
  BOOST_RANDOM_DETAIL_EQUALITY_OPERATOR(sobol, x, y)
  { return x.is_equal(y); }
  //!Returns true if the two generators will produce different sequences,
  BOOST_RANDOM_DETAIL_INEQUALITY_OPERATOR(sobol)
};

} // namespace random

/** @cond */
template<std::size_t Dimension>
struct sobol_generator
{
  typedef random::sobol<uint32_t, Dimension, 1, (uint32_t)1 << 31> type;
};
/** @endcond */

typedef sobol_generator< 1 >::type sobol_1d;
typedef sobol_generator< 2 >::type sobol_2d;
typedef sobol_generator< 3 >::type sobol_3d;
typedef sobol_generator< 4 >::type sobol_4d;
typedef sobol_generator< 5 >::type sobol_5d;
typedef sobol_generator< 6 >::type sobol_6d;
typedef sobol_generator< 7 >::type sobol_7d;
typedef sobol_generator< 8 >::type sobol_8d;
typedef sobol_generator< 9 >::type sobol_9d;
typedef sobol_generator< 10 >::type sobol_10d;
typedef sobol_generator< 11 >::type sobol_11d;
typedef sobol_generator< 12 >::type sobol_12d;

} // namespace boost

#endif // BOOST_RANDOM_SOBOL_HPP
