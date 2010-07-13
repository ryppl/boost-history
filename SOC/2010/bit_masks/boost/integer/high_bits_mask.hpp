//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org for updates, documentation, and revision history. 


#ifndef BOOST_HIGH_BITS_MASK_HPP
#define BOOST_HIGH_BITS_MASK_HPP





#include <boost/config.hpp>
#include <boost/mpl/bitwise.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/integer/detail/high_low_impl.hpp>
#include <boost/integer/integral_mask.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/make_unsigned.hpp>







namespace boost {

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4309)
// #pragma warning(disable : 4305)
namespace detail {
    template<typename T, unsigned int Width>
    struct evaluate_for_msvc_08 {
        typedef typename make_unsigned<T>::type unsigned_type;
        BOOST_STATIC_CONSTANT(T, val = (~(mpl::shift_right<
            mpl::size_t<
                unsigned_type,
                ~(T(0))
            >,
            mpl::size_t<Width>
        >::type::value)
        ));
        typedef integral_constant<T, val>       type;
    };
}

#pragma warning(pop)
#endif

/** Creates a mask of the supplied width in side type T, from the lower portion 
 *  of the integer starting from the left most bit moving towards the right.
 */
template <typename T, unsigned int Width>
struct high_bits_mask
    :detail::high_bits_preconditions<T,Width>,

#ifdef BOOST_MSVC
    detail::evaluate_for_msvc_08<T,Width>::type
#else
    // GNU likes this version.
    integral_mask<T,  (~( ~ (typename make_unsigned<T>::type(0)) >> Width)) >
#endif
{    
    typedef high_bits_mask<T,Width>  type;

    BOOST_STATIC_CONSTANT(unsigned int, offset = 0);

    BOOST_STATIC_CONSTANT(unsigned int, width  = Width);
};

} // namespace boost 

#endif
