//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BIT_MASK_GROUP_IMPL_HPP
#define BOOST_BIT_MASK_GROUP_IMPL_HPP
#include <boost/fusion/support/tag_of_fwd.hpp>
#include <boost/fusion/include/tag_of_fwd.hpp>
#include <boost/fusion/support/iterator_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/void.hpp>




namespace boost { namespace details {

typedef mpl::void_ unused_parameter;

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

} } // end details
#if 0

namespace boost { namespace fusion { namespace extension {
//TODO: May not need the impl overloads for the iterator because its gets 
// done implicitly somewhere else.


template <>
struct value_at_impl< boost::details::bit_mask_group_iterator_tag > {
    template<typename Iterator>
    struct apply;

    template<typename MaskGroup, unsigned int Pos>
    struct apply< boost::bit_mask_group_iterator<MaskGroup, Pos> > {
        typedef typename MaskGroup::template get_by_index<Pos>::type type;
    };
};



template<>
struct deref_impl<boost::details::bit_mask_group_iterator_tag >
{
    template<typename Iterator>
    struct apply;

    template<typename MaskGroup, unsigned int Pos>
    struct apply< boost::bit_mask_group_iterator<MaskGroup,Pos> > {
        typedef typename MaskGroup::template get_by_index<Pos>::type mask_type;

        static typename mask_type::value_type
        call(boost::bit_mask_group_iterator<MaskGroup,Pos> const&) {
            return mask_type::value;
        }
    };
};



template<>
struct next_impl< boost::details::bit_mask_group_iterator_tag > {

    template<typename Iterator>
    struct apply {
        typedef typename Iterator::mask_group mask_group;
        typedef typename Iterator::index index;
        typedef boost::bit_mask_group_iterator<
            mask_group,
            index::value + 1
        > type;

        static type call(Iterator const&) {
             return type();
        }
    };
};



template <>
struct prior_impl< boost::details::bit_mask_group_iterator_tag > {
    template <typename Iterator>
    struct apply {
        typedef typename Iterator::mask_group  mask_group;
        typedef typename Iterator::index        index;
        typedef typename boost::bit_mask_group_iterator<
            mask_group,
            index::value - 1
        > type;

        static type call(Iterator const&) {
            return type();
        }
    };
};



template <>
struct advance_impl< boost::details::bit_mask_group_iterator_tag > {

    template <typename Iterator, typename N>
    struct apply {
        typedef typename Iterator::mask_group mask_group;
        typedef typename Iterator::index index;
        typedef typename boost::bit_mask_group_iterator<
            mask_group,
            index::value+N::value
        > type;

        static type
        call(Iterator const&) {
            return type();
        }
    };
};



template <>
struct distance_impl< boost::details::bit_mask_group_iterator_tag > {
    template <typename First, typename Last>
    struct apply
        : mpl::minus< typename Last::index, typename First::index>
    {
        static typename mpl::minus<
            typename Last::index,
            typename First::index
        >::type
        call(First const&, Last const&) {
            typedef typename mpl::minus<
                typename Last::index,
                typename First::index
            >::type result;

            return result();
        }
    };
};



template <>
struct equal_to_impl< boost::details::bit_mask_group_iterator_tag > {
    template <typename I1, typename I2>
    struct apply             
        : is_same<
            typename I1::identity,
            typename I2::identity
        >::type
    { };
};


template<>
struct at_key_impl< boost::details::bit_mask_group_tag > {

    template<typename MaskGroup, typename Key>
    struct apply {
        typedef typename MaskGroup::
            template get_by_name<
                Key
            >::type
        type;

        static typename type::value_type call(MaskGroup&) {
            return type::value;
        }
    };
};


}}} // end boost::fusion::extention
#endif

#endif

