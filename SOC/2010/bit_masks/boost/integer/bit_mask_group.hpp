//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASK_GROUP_HPP
#define BOOST_BIT_MASK_GROUP_HPP


#include <boost/integer/compound_mask.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/if.hpp>




namespace boost {

// TODO: move this into a sperate file
namespace details {
struct null_mask { };
typedef null_mask unused_parameter;

/** This is a metafunction which is used for filling an mpl::vector with
 *  types which arn't of type unused parameter.
 */
template <typename T, typename TypeVector>
struct add_to_vector {

    typedef add_to_vector< T, TypeVector>           type;
    typedef T                                       type_added;
    typedef typename mpl::push_back<TypeVector, T>::type vector_type;


    // adds to the back of the vector and calls this meta function again.
    template <typename NewT>
    struct add {
        typedef add_to_vector< NewT, vector_type> type;
    };


};

/** Specialization for unused parameters. 
 *  This is the case where the parameter wasn't used by the user and is instead
 *  going to be ignored, however its behavior is going to similar to that of
 *  the more general form above.
 */
template <typename TypeVector>
struct add_to_vector< unused_parameter, TypeVector > {
    typedef add_to_vector< unused_parameter, TypeVector>   type;
    typedef unused_parameter                               type_added;
    typedef TypeVector                                     vector_type;    

    // fake adding the parameter to the TypeVector instead do nothing.
    template <typename NewT>
    struct add {
        typedef add_to_vector<NewT, TypeVector> type;
    };
};

}


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
struct bit_mask_group
    :details::add_to_vector<Mask1,mpl::vector<Mask0> >::
    template add<Mask2>::type::
    template add<Mask3>::type::
    template add<Mask4>::type::
    template add<Mask5>::type::
    template add<Mask6>::type::
    template add<Mask7>::type::
    template add<Mask8>::type::
    template add<Mask9>::type::vector_type
{
    
    // typedef typename mpl::if_<
    //    is_same<Mask1,typename details::unused_parameter>,
    //    Mask1,
};


} // namespace

#endif
