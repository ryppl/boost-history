
/*
    The Extended Integer (XInt) Library
    A fast, portable C++ library for multi-precision integer math
    Copyright 2010 by Chad Nelson

    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt

    See http://www.boost.org/libs/xint for library home page.
*/

/*! \file
    \brief Declares items relating to random numbers.
*/

#ifndef BOOST_INCLUDED_XINT_RANDOM_HPP
#define BOOST_INCLUDED_XINT_RANDOM_HPP

#include "internals.hpp"
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace boost {
namespace xint {

/*! \brief A convenience class for cryptographically-secure random numbers.

    \see \ref random
    \see \ref boost::xint::random_by_size
    \see \ref boost::xint::random_prime
*/
class strong_random_generator {
    public:
    typedef unsigned int result_type;

    strong_random_generator();
    ~strong_random_generator();
    result_type operator()();

    static const bool has_fixed_range;
    static const result_type min_value;
    static const result_type max_value;
    result_type min() const;
    result_type max() const;
    double entropy() const;

    private:
    struct impl_t;
    impl_t *impl;
};

/*! \brief A convenience class for general-purpose (insecure) random numbers.

    This is nothing more than a very thin shell around one of the faster
    pseudo-random number generators provided by Boost.Random.

    \see \ref random
    \see \ref boost::xint::random_by_size
    \see \ref boost::xint::random_prime
*/
class default_random_generator: public boost::mt19937 {
    public:
    default_random_generator(): mt19937(time(0)+clock()) { }
};

//! @cond detail
namespace detail {
    //! The base class for random_generator.
    struct base_random_generator {
        typedef unsigned int result_type;
        virtual result_type operator()()=0;
        virtual ~base_random_generator() { }
    };

    //! Support class for the random generator code.
    template <class T>
    struct random_generator: public base_random_generator {
        typedef boost::uniform_int<result_type> dist_t;
        typedef boost::variate_generator<T&, dist_t> gen_t;
        random_generator(T& g): gen(g,
            dist_t((std::numeric_limits<result_type>::min)(),
                (std::numeric_limits<result_type>::max)())) { }
        result_type operator()() { return gen(); }
        gen_t gen;
    };

    integer random_by_size(base_random_generator& gen, size_t bits, bool
        high_bit_on, bool low_bit_on, bool can_be_negative);
    integer random_prime(base_random_generator& gen, size_t size_in_bits,
        callback_t callback);
} // namespace detail
//! @endcond detail

////////////////////////////////////////////////////////////////////////////////
// Template function definitions

/*! \brief Generates a random integer with specific attributes.

- Complexity: O(n)

\param[in] gen The random number generator to use. This can be
boost::xint::default_random_generator, boost::xint::strong_random_generator, or
any other class that supplies the member functions required by the Boost.Random
random number generators.
\param[in] bits The maximum number of bits that you want the returned number to
have.
\param[in] high_bit_on If \c true, the returned number will have exactly the
requested size. If \c false, the upper bits may be zero, resulting in a number
that is slightly smaller than requested.
\param[in] low_bit_on If \c true, the returned number will always be odd. If
\c false, it has an equal chance of being odd or even.
\param[in] can_be_negative If \c true, the returned value has an equal chance
of being positive or negative. If \c false, it will always be positive.

\returns A random integer with the requested attributes.

\see \ref random
*/
template <class T>
integer random_by_size(T& gen, size_t bits, bool high_bit_on, bool low_bit_on,
    bool can_be_negative)
{
    detail::random_generator<T> generator(gen);
    integer r(detail::random_by_size(generator, bits, high_bit_on, low_bit_on,
        can_be_negative));
    return BOOST_XINT_MOVE(r);
}

/*! \brief Generate a randomly-generated prime number of a particular bit-size.

- Complexity: Varies

\param[in] gen The random number generator to use. This can be
boost::xint::default_random_generator, boost::xint::strong_random_generator, or
any other class that supplies the member functions required by the Boost.Random
random number generators.
\param[in] size_in_bits The number of bits that you want the returned value to
have.
\param[in] callback An optional function that will be called regularly during
the operation. If it returns \c false, the function will immediately return.

\returns A randomly-generated prime integer with the specified number of bits,
or zero if the provided callback function cancelled the operation.

\exception exceptions::invalid_argument if \c size_in_bits is less than two.

\note This function uses xint::is_prime. See the description of it for details
of its limitations.

\par
See the \link random Random Number Functions page\endlink for details on how to
get cryptographically-secure random numbers.

\see \ref primes
*/
template <class T>
integer random_prime(T& gen, size_t size_in_bits, callback_t callback) {
    detail::random_generator<T> generator(gen);
    return BOOST_XINT_MOVE(detail::random_prime(generator, size_in_bits,
        callback));
}

} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_RANDOM_HPP
