
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

namespace boost {
namespace xint {

/*! \brief A convenience class for cryptographically-secure random numbers.

    Used with xint::set_random_generator to set the XInt library to use the
    system-supplied strong random number generator.

    \see \ref random
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
        random_generator(T *g): genobj(g), gen(*g,
            dist_t((std::numeric_limits<result_type>::min)(),
                (std::numeric_limits<result_type>::max)())) { }
        result_type operator()() { return gen(); }
        std::auto_ptr<T> genobj;
        gen_t gen;
    };

    //! Support function \see xint::set_random_generator
    void set_random_generator(base_random_generator *obj);

    /*!
        \return An unsigned int from the current random generator.
        \see xint::set_random_generator
    */
    unsigned int get_random();
} // namespace detail

////////////////////////////////////////////////////////////////////////////////
// Template function definitions

/*! \brief Change the random generator that the library uses.

\param[in] gen A pointer to the new generator to use.

\warning The generator must be a heap-allocated pointer (one created with
<code>new</code>), which the XInt library assumes ownership of. Don't pass in a
stack-created item, don't try to delete the item that you passed in, and don't
attempt to use it separately after passing it to this function, or the behavior
is undefined.

\remarks
See the \ref random "Random Number page" for an example of how to use it.
*/
template <typename T>
void set_random_generator(T *gen) {
    detail::random_generator<T> *obj=new detail::random_generator<T>(gen);
    detail::set_random_generator(obj);
}

} // namespace xint
} // namespace boost

#endif // BOOST_INCLUDED_XINT_RANDOM_HPP
