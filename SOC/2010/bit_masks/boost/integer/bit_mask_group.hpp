//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASK_GROUP_HPP
#define BOOST_BIT_MASK_GROUP_HPP


#include <boost/integer/compound_mask.hpp>
#include <boost/integer/bit_mask_group_impl.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>





namespace boost {




/** This is used for naming/tagging arguments within the bit_mask_group. */
template <typename T, typename NameType>
struct tagged {
    typedef NameType            name;
    typedef T                   value;
    typedef tagged<NameType, T> type;
};



// TODO: move this into a sperate file
namespace details {
struct null_mask { };
typedef null_mask unused_parameter;

/** This is a metafunction which is used for filling an mpl::vector with
 *  types which arn't of type unused parameter.
 */
template <typename T, typename TypeVector, typename NamedTypeMap >
struct bit_mask_group_impl_ {

    typedef bit_mask_group_impl_< T, TypeVector, NamedTypeMap>  type;
    typedef T                                                   type_added;
    typedef typename mpl::push_back<TypeVector, T>::type        type_vector;
    typedef NamedTypeMap                                        named_type_map;


    // adds to the back of the vector and calls this meta function again.
    template <typename NewT>
    struct add {
        typedef bit_mask_group_impl_< NewT, type_vector, NamedTypeMap> type;
    };
};

/** Specialization for unused parameters. 
 *  This is the case where the parameter wasn't used by the user and is instead
 *  going to be ignored, however its behavior is going to similar to that of
 *  the more general form above.
 */
template <typename TypeVector, typename NamedTypeMap>
struct bit_mask_group_impl_< unused_parameter, TypeVector, NamedTypeMap > {

    typedef bit_mask_group_impl_<
        unused_parameter,
        TypeVector,
        NamedTypeMap
    >  type;

    typedef unused_parameter type_added;
    typedef TypeVector type_vector;
    typedef NamedTypeMap named_type_map;

    // fake adding the parameter to the TypeVector instead do nothing.
    template <typename NewT>
    struct add {
        typedef bit_mask_group_impl_<NewT, TypeVector, NamedTypeMap > type;
    };
};


/** Used for dealing with the "named" types or types which basically contain a
 *  single tag which allows them to be referenced instead of an index.
 */
template <  typename Name,
            typename Value,
            typename TypeVector,
            typename NamedTypeMap>
struct bit_mask_group_impl_< tagged<Value, Name>, TypeVector, NamedTypeMap>
{
    typedef bit_mask_group_impl_< Value, TypeVector, NamedTypeMap> type;
    typedef Value type_added;
    typedef typename mpl::push_back<TypeVector, Value>::type type_vector;
    typedef typename mpl::insert<
        NamedTypeMap,mpl::pair<Name, Value>
    >::type named_type_map;

    template <typename NewT>
    struct add {
        typedef bit_mask_group_impl_<NewT, type_vector, named_type_map> type;
    };
};

} // namespace details.





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


} // namespace

#endif
