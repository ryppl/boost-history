//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASK_GROUP_HPP
#define BOOST_BIT_MASK_GROUP_HPP

#include <boost/integer/bit_mask_group_fwd.hpp>
#include <boost/integer/compound_mask.hpp>
#include <boost/integer/details/bit_mask_group_impl.hpp>
#include <boost/integer/details/tagged.hpp>


namespace boost {


/** \name bit_mask_group
 *  \brief This a psudo variadic class which uses default template parameters
 *  and allows for both named and unnamed mask types to be stored within this
 *  structure as types.
 */

template <  typename Mask0,
            typename Mask1  = details::unused_parameter,
            typename Mask2  = details::unused_parameter,
            typename Mask3  = details::unused_parameter,
            typename Mask4  = details::unused_parameter,
            typename Mask5  = details::unused_parameter,
            typename Mask6  = details::unused_parameter,
            typename Mask7  = details::unused_parameter,
            typename Mask8  = details::unused_parameter,
            typename Mask9  = details::unused_parameter
        >
struct bit_mask_group {

    /** This strange looking typedef is for adding all of the different mask 
     *  template parameters. This may one day be replaced by a macro for 
     *  creating psudo variadic behavior. (i.e. this will get more parameters 
     *  via value specific by another macro).
     */
    typedef typename details::bit_mask_group_impl_<
        Mask0,
            mpl::vector<>,
            mpl::map<>
        >::
        template add<Mask1>::type::
        template add<Mask2>::type::
        template add<Mask3>::type::
        template add<Mask4>::type::
        template add<Mask5>::type::
        template add<Mask6>::type::
        template add<Mask7>::type::
        template add<Mask8>::type::
        template add<Mask9>::type       _impl_type;

    typedef bit_mask_group<Mask0, Mask1, Mask2, Mask3, Mask4,
                           Mask5, Mask6, Mask7, Mask8, Mask9 > type;

    /** Vector and map types used to store the information about named and
     *  unnamed types.
     */
    typedef typename _impl_type::named_type_map      named_mask_map;
    typedef typename _impl_type::type_vector         mask_vector;



    /** Accessor meta functions which allow the user to retrieve a bit mask via
     *  name or the index in which it was stored. This is the compile time
     *  implementation of this functionality of the class.
     */
    //@{
    template <typename Name>
    struct get_by_name {
        typedef typename mpl::at<named_mask_map, Name>::type type;
    };

    template <unsigned int Index>
    struct get_by_index {
        typedef typename mpl::at<
            mask_vector,
            integral_constant< unsigned int,Index>
        >::type type;
    };
    //@}



    /** Runtime support functions for the bit mask group.
     *  The get functions are runtime support funtions. They provide the
     *  following inteface:
     *  get<name>() // will fetch a named mask and return the value of the mask.
     *  get< number >() // will fetch and retun a mask with an index the same 
     *                  // as number.
     */
    //@{
    template <typename Name>
    inline typename mpl::at<named_mask_map, Name>::type::value_type
    get() {
        return mpl::at<named_mask_map, Name>::type::value;
    }

    template <unsigned int Index>
    inline typename mpl::at<
        mask_vector,
        integral_constant<unsigned int, Index>
    >::type::value_type
    get() {
        return mpl::at<
            mask_vector,
            integral_constant<unsigned int, Index>
        >::type::value;
    }
    //@}
};


} // end boost

#include <boost/integer/details/bit_mask_group_fusion_ext/fusion_includes.hpp>

#endif
