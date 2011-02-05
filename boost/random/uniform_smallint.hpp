/* boost random/uniform_smallint.hpp header file
 *
 * Copyright Jens Maurer 2000-2001
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org for most recent version including documentation.
 *
 * $Id$
 *
 * Revision history
 *  2001-04-08  added min<max assertion (N. Becker)
 *  2001-02-18  moved to individual header files
 */

#ifndef BOOST_RANDOM_UNIFORM_SMALLINT_HPP
#define BOOST_RANDOM_UNIFORM_SMALLINT_HPP

#include <cassert>
#include <istream>
#include <iosfwd>
#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/random/detail/config.hpp>
#include <boost/random/detail/operators.hpp>
#include <boost/random/detail/signed_unsigned_tools.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/detail/workaround.hpp>

namespace boost {
namespace random {

// uniform integer distribution on a small range [min, max]

/**
 * The distribution function uniform_smallint models a \random_distribution.
 * On each invocation, it returns a random integer value uniformly distributed
 * in the set of integer numbers {min, min+1, min+2, ..., max}. It assumes
 * that the desired range (max-min+1) is small compared to the range of the
 * underlying source of random numbers and thus makes no attempt to limit
 * quantization errors.
 *
 * Let \f$r_{\mathtt{out}} = (\mbox{max}-\mbox{min}+1)\f$ the desired range of
 * integer numbers, and
 * let \f$r_{\mathtt{base}}\f$ be the range of the underlying source of random
 * numbers. Then, for the uniform distribution, the theoretical probability
 * for any number i in the range \f$r_{\mathtt{out}}\f$ will be
 * \f$\displaystyle p_{\mathtt{out}}(i) = \frac{1}{r_{\mathtt{out}}}\f$.
 * Likewise, assume a uniform distribution on \f$r_{\mathtt{base}}\f$ for
 * the underlying source of random numbers, i.e.
 * \f$\displaystyle p_{\mathtt{base}}(i) = \frac{1}{r_{\mathtt{base}}}\f$.
 * Let \f$p_{\mathtt{out\_s}}(i)\f$ denote the random
 * distribution generated by @c uniform_smallint. Then the sum over all
 * i in \f$r_{\mathtt{out}}\f$ of
 * \f$\displaystyle
 * \left(\frac{p_{\mathtt{out\_s}}(i)}{p_{\mathtt{out}}(i)} - 1\right)^2\f$
 * shall not exceed
 * \f$\displaystyle \frac{r_{\mathtt{out}}}{r_{\mathtt{base}}^2}
 * (r_{\mathtt{base}} \mbox{ mod } r_{\mathtt{out}})
 * (r_{\mathtt{out}} - r_{\mathtt{base}} \mbox{ mod } r_{\mathtt{out}})\f$.
 *
 * The template parameter IntType shall denote an integer-like value type.
 *
 * Note: The property above is the square sum of the relative differences
 * in probabilities between the desired uniform distribution
 * \f$p_{\mathtt{out}}(i)\f$ and the generated distribution
 * \f$p_{\mathtt{out\_s}}(i)\f$.
 * The property can be fulfilled with the calculation
 * \f$(\mbox{base\_rng} \mbox{ mod } r_{\mathtt{out}})\f$, as follows:
 * Let \f$r = r_{\mathtt{base}} \mbox{ mod } r_{\mathtt{out}}\f$.
 * The base distribution on \f$r_{\mathtt{base}}\f$ is folded onto the
 * range \f$r_{\mathtt{out}}\f$. The numbers i < r have assigned
 * \f$\displaystyle
 * \left\lfloor\frac{r_{\mathtt{base}}}{r_{\mathtt{out}}}\right\rfloor+1\f$
 * numbers of the base distribution, the rest has only \f$\displaystyle
 * \left\lfloor\frac{r_{\mathtt{base}}}{r_{\mathtt{out}}}\right\rfloor\f$.
 * Therefore,
 * \f$\displaystyle p_{\mathtt{out\_s}}(i) =
 * \left(\left\lfloor\frac{r_{\mathtt{base}}}
 * {r_{\mathtt{out}}}\right\rfloor+1\right) /
 * r_{\mathtt{base}}\f$ for i < r and \f$\displaystyle p_{\mathtt{out\_s}}(i) =
 * \left\lfloor\frac{r_{\mathtt{base}}}
 * {r_{\mathtt{out}}}\right\rfloor/r_{\mathtt{base}}\f$ otherwise.
 * Substituting this in the
 * above sum formula leads to the desired result.
 *
 * Note: The upper bound for
 * \f$(r_{\mathtt{base}} \mbox{ mod } r_{\mathtt{out}})
 * (r_{\mathtt{out}} - r_{\mathtt{base}} \mbox{ mod } r_{\mathtt{out}})\f$ is
 * \f$\displaystyle \frac{r_{\mathtt{out}}^2}{4}\f$.  Regarding the upper bound
 * for the square sum of the relative quantization error of
 * \f$\displaystyle \frac{r_\mathtt{out}^3}{4r_{\mathtt{base}}^2}\f$, it
 * seems wise to either choose \f$r_{\mathtt{base}}\f$ so that
 * \f$r_{\mathtt{base}} > 10r_{\mathtt{out}}^2\f$ or ensure that
 * \f$r_{\mathtt{base}}\f$ is
 * divisible by \f$r_{\mathtt{out}}\f$.
 */
template<class IntType = int>
class uniform_smallint
{
public:
    typedef IntType input_type;
    typedef IntType result_type;

    class param_type
    {
    public:

        typedef uniform_smallint distribution_type;

        /** constructs the parameters of a @c uniform_smallint distribution. */
        param_type(IntType min_arg = 0, IntType max_arg = 9)
          : _min(min_arg), _max(max_arg)
        {
            assert(_min <= _max);
        }

        /** Returns the minimum value. */
        IntType a() const { return _min; }
        /** Returns the maximum value. */
        IntType b() const { return _max; }
        

        /** Writes the parameters to a @c std::ostream. */
        BOOST_RANDOM_DETAIL_OSTREAM_OPERATOR(os, param_type, parm)
        {
            os << parm._min << " " << parm._max;
            return os;
        }
    
        /** Reads the parameters from a @c std::istream. */
        BOOST_RANDOM_DETAIL_ISTREAM_OPERATOR(is, param_type, parm)
        {
            is >> parm._min >> std::ws >> parm._max;
            return is;
        }

        /** Returns true if the two sets of parameters are equal. */
        BOOST_RANDOM_DETAIL_EQUALITY_OPERATOR(param_type, lhs, rhs)
        { return lhs._min == rhs._min && lhs._max == rhs._max; }

        /** Returns true if the two sets of parameters are different. */
        BOOST_RANDOM_DETAIL_INEQUALITY_OPERATOR(param_type)

    private:
        IntType _min;
        IntType _max;
    };

    /**
     * Constructs a @c uniform_smallint. @c min and @c max are the
     * lower and upper bounds of the output range, respectively.
     */
    explicit uniform_smallint(IntType min_arg = 0, IntType max_arg = 9)
      : _min(min_arg), _max(max_arg) {}

    /**
     * Constructs a @c uniform_smallint from its parameters.
     */
    explicit uniform_smallint(const param_type& parm)
      : _min(parm.a()), _max(parm.b()) {}

    /** Returns the minimum value of the distribution. */
    result_type a() const { return _min; }
    /** Returns the maximum value of the distribution. */
    result_type b() const { return _max; }
    /** Returns the minimum value of the distribution. */
    result_type min BOOST_PREVENT_MACRO_SUBSTITUTION () const { return _min; }
    /** Returns the maximum value of the distribution. */
    result_type max BOOST_PREVENT_MACRO_SUBSTITUTION () const { return _max; }

    /** Returns the parameters of the distribution. */
    param_type param() const { return param_type(_min, _max); }
    /** Sets the parameters of the distribution. */
    void param(const param_type& parm)
    {
        _min = parm.a();
        _max = parm.b();
    }

    /**
     * Effects: Subsequent uses of the distribution do not depend
     * on values produced by any engine prior to invoking reset.
     */
    void reset() { }

    /** Returns a value uniformly distributed in the range [min(), max()]. */
    template<class Engine>
    result_type operator()(Engine& eng) const
    {
        typedef typename Engine::result_type base_result;
        return generate(eng, boost::is_integral<base_result>());
    }

    /** Returns a value uniformly distributed in the range [param.a(), param.b()]. */
    template<class Engine>
    result_type operator()(Engine& eng, const param_type& parm) const
    { return uniform_smallint(parm)(eng); }

    /** Writes the distribution to a @c std::ostream. */
    BOOST_RANDOM_DETAIL_OSTREAM_OPERATOR(os, uniform_smallint, ud)
    {
        os << ud._min << " " << ud._max;
        return os;
    }
    
    /** Reads the distribution from a @c std::istream. */
    BOOST_RANDOM_DETAIL_ISTREAM_OPERATOR(is, uniform_smallint, ud)
    {
        is >> ud._min >> std::ws >> ud._max;
        return is;
    }

    /**
     * Returns true if the two distributions will produce identical
     * sequences of values given equal generators.
     */
    BOOST_RANDOM_DETAIL_EQUALITY_OPERATOR(uniform_smallint, lhs, rhs)
    { return lhs._min == rhs._min && lhs._max == rhs._max; }
    
    /**
     * Returns true if the two distributions may produce different
     * sequences of values given equal generators.
     */
    BOOST_RANDOM_DETAIL_INEQUALITY_OPERATOR(uniform_smallint)

private:
    
    // \cond
    template<class Engine>
    result_type generate(Engine& eng, boost::mpl::true_) const
    {
        // equivalent to (eng() - eng.min()) % (_max - _min + 1) + _min,
        // but guarantees no overflow.
        typedef typename Engine::result_type base_result;
        typedef typename boost::make_unsigned<base_result>::type base_unsigned;
        typedef typename boost::make_unsigned<result_type>::type range_type;
        range_type range = random::detail::subtract<result_type>()(_max, _min);
        base_unsigned base_range =
            random::detail::subtract<result_type>()((eng.max)(), (eng.min)());
        base_unsigned val =
            random::detail::subtract<base_result>()(eng(), (eng.min)());
        if(range >= base_range) {
            return boost::random::detail::add<range_type, result_type>()(
                static_cast<range_type>(val), _min);
        } else {
            base_unsigned modulus = static_cast<base_unsigned>(range) + 1;
            return boost::random::detail::add<range_type, result_type>()(
                static_cast<range_type>(val % modulus), _min);
        }
    }
    
    template<class Engine>
    result_type generate(Engine& eng, boost::mpl::false_) const
    {
        typedef typename Engine::result_type base_result;
        typedef typename boost::make_unsigned<result_type>::type range_type;
        range_type range = random::detail::subtract<result_type>()(_max, _min);
        base_result val = boost::uniform_01<base_result>()(eng);
        // what is the worst that can possibly happen here?
        // base_result may not be able to represent all the values in [0, range]
        // exactly.  If this happens, it will cause round off error and we
        // won't be able to produce all the values in the range.  We don't
        // care about this because the user has already told us not to by
        // using uniform_smallint.  However, we do need to be careful
        // to clamp the result, or floating point rounding can produce
        // an out of range result.
        range_type offset = static_cast<range_type>(val * (static_cast<base_result>(range) + 1));
        if(offset > range) return _max;
        return boost::random::detail::add<range_type, result_type>()(offset , _min);
    }
    // \endcond

    result_type _min;
    result_type _max;
};

} // namespace random

using random::uniform_smallint;

} // namespace boost

#endif // BOOST_RANDOM_UNIFORM_SMALLINT_HPP
