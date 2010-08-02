/* boost random/detail/quasi_random_number_generator_base.hpp header file
 *
 * Copyright Justinas Vygintas Daugmaudis 2010
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_RANDOM_DETAIL_QRNG_BASE_HPP
#define BOOST_RANDOM_DETAIL_QRNG_BASE_HPP

#include <istream>
#include <ostream>

#include <stdexcept>

#include <boost/throw_exception.hpp>

//!\file
//!Describes the quasi-random number generator base class template.

namespace boost {
namespace random {

namespace detail {

template<typename DerivedT, typename LatticeT>
class qrng_base
{
protected:

  BOOST_STATIC_CONSTANT(std::size_t, dimension = LatticeT::dimension_value);

public:

  typedef typename LatticeT::result_type result_type;

  qrng_base()
    : lattice(std::size_t())
  {
    derived().seed();
  }

  explicit qrng_base(std::size_t init)
    : lattice(init)
  {
    derived().seed(); // acquire initial values and then move on if necessary
    derived().seed(init);
  }

  // default copy c-tor is fine

  // default assignment operator is fine

  //!Requirements: *this is mutable.
  //!
  //!Returns: Returns a successive element of an s-dimensional
  //!(s = X::dimension()) vector at each invocation. When all elements are
  //!exhausted, X::operator() begins anew with the starting element of a
  //!subsequent s-dimensional vector.
  //!
  //!Throws: overflow_error.
  result_type operator()()
  {
    return curr_elem != dimension ? load_saved(): next_state();
  }

  //!Requirements: *this is mutable.
  //!
  //!Effects: Advances *this state as if z consecutive
  //!X::operator() invocations were executed.
  //!
  //!Throws: overflow_error.
  void discard(std::size_t z)
  {
    std::size_t vec_n  = z / dimension;
    std::size_t elem_n = z - vec_n * dimension; // z % Dimension
    std::size_t vec_offset = vec_n + (curr_elem + elem_n) / dimension;
    // Discards vec_offset consecutive s-dimensional vectors
    discard_vector(vec_offset);
    // Sets up the proper position of the element-to-read
    curr_elem += (z - dimension * vec_offset);
  }

protected:
  DerivedT& derived() throw()
  {
    return *static_cast<DerivedT * const>(this);
  }

  //!Writes a @c DerivedT to a @c std::ostream.
  template<typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>&
  write_impl(std::basic_ostream<CharT, Traits>& os) const
  {
    os << curr_elem << " " << seq_count;
    return os;
  }

  //!Reads a @c DerivedT from a @c std::istream.
  template<typename CharT, typename Traits>
  std::basic_istream<CharT, Traits>&
  read_impl(std::basic_istream<CharT, Traits>& is)
  {
    std::size_t dim, init;
    if( is >> dim >> std::ws >> init ) // initialize iff success!
    {
      derived().seed(init);
      curr_elem = dim;
    }
    return is;
  }

  bool is_equal(const DerivedT& rhs) const
  {
    // compare normalized sequence position, but without the possible overflow
    // that would go with seq_count * dimension + curr_elem.
    return (seq_count + (curr_elem / dimension) ==
              rhs.seq_count + (rhs.curr_elem / dimension)) &&
           (curr_elem % dimension == rhs.curr_elem % dimension);
  }

private:

  // Load the result from the saved state.
  result_type load_saved()
  {
    return quasi_state[curr_elem++];
  }

  result_type next_state()
  {
    derived().compute_next(seq_count++);

    curr_elem = 0;
    return load_saved();
  }

  // Discards z consecutive s-dimensional vectors,
  // and preserves the position of the element-to-read
  void discard_vector(std::size_t z)
  {
    std::size_t tmp = curr_elem;
    std::size_t inc_seq_count = seq_count + z;
    // Here we check that no overflow occurs before we
    // begin seeding the new value
    if( inc_seq_count >= seq_count )
    {
      derived().seed(inc_seq_count);
    }
    else
    {
      boost::throw_exception( std::overflow_error("discard_vector") );
    }
    curr_elem = tmp;
  }

protected:
  LatticeT lattice;
  std::size_t curr_elem;
  std::size_t seq_count;
  result_type quasi_state[dimension];
};

}} // namespace detail::random

} // namespace boost

#endif // BOOST_RANDOM_DETAIL_QRNG_BASE_HPP
