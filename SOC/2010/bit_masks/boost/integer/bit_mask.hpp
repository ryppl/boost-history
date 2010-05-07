//  Boost integer/bit_mask.hpp header file

//  (C) Copyright Brian Bartman 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history. 
#ifndef BOOST_INTEGER_BIT_MASK_HPP
#define BOOST_INTEGER_BIT_MASK_HPP

#include <boost/mpl/int.hpp>
#include <boost/mpl/bitwise.hpp>

#include <boost/integer/details/bit_mask_impl.hpp>

namespace boost {

// forward declaration of the bit mask structure which will all 
// a recursive definition of how
template <typename Value, typename SecondValue = mpl::int_<0> > struct bit_mask;


// termination specialization this will cause the psudo-recursive definition 
// being evalueated to finish.
template <int NewMaskValue, int CurrentMaskValue>
struct bit_mask< mpl::int_< NewMaskValue >, mpl::int_< CurrentMaskValue > >
    :mpl::bitor_<
        mpl::int_<NewMaskValue>,
        mpl::int_< CurrentMaskValue >
    >::type
{ };

// this is the recursive step for the bit_mask.
template <int MaskValue, int CurrentMaskValue>
struct bit_mask < bit_mask< mpl::int_<MaskValue> >, mpl::int_<CurrentMaskValue> >
    :mpl::bitor_<
        mpl::int_< CurrentMaskValue >,
        bit_mask< mpl::int_< MaskValue > >
    >::type
{};

} // namespace boost

#endif
