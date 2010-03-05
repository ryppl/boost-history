/*=============================================================================
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_MEMBER_OBJECT_POINTER_HELPER_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_MEMBER_OBJECT_POINTER_HELPER_HPP

#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/functional/invocation/detail/that_ptr.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/remove_pointer.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename>
    struct split_object_pointer;

    template<class Type,class Class>
    struct split_object_pointer<Type(Class::*)>
    {
        typedef Type type;
        typedef Class class_type;
    };

    template<class Type,class Class>
    struct split_object_pointer<Type(Class::* const)>
    {
        typedef Type type;
        typedef Class class_type;
    };

    template<class Type,class Class>
    struct split_object_pointer<Type(Class::* volatile)>
    {
        typedef Type type;
        typedef Class class_type;
    };

    template<class Type,class Class>
    struct split_object_pointer<Type(Class::* const volatile)>
    {
        typedef Type type;
        typedef Class class_type;
    };

    template<typename Instance, typename Sig>
    struct get_object_pointer_result_type
    {
        typedef typename split_object_pointer<Sig>::type  object_type;
        typedef typename split_object_pointer<Sig>::class_type class_type;
        typedef typename
            remove_reference<Instance>::type
        nonref_class_instance;
        typedef typename identity<Instance>::type identity_class_instance;

        typedef typename
            is_base_of<
                class_type
              , identity_class_instance
            >::type
        is_directly_convertible;

        typedef typename
            mpl::if_<
                mpl::or_<
                    is_const<object_type>
                  , mpl::and_<
                        is_directly_convertible
                      , is_const<nonref_class_instance>
                    >
                  , mpl::and_<
                        mpl::not_<is_directly_convertible>
                      , const_pointee<Instance>
                    >
                >
              , class_type const
              , class_type
            >::type
        const_class_type;
        typedef typename
            mpl::if_<
                mpl::or_<
                    is_volatile<object_type>
                  , mpl::and_<
                        is_directly_convertible
                      , is_volatile<nonref_class_instance>
                    >
                  , mpl::and_<
                        mpl::not_<is_directly_convertible>
                      , volatile_pointee<Instance>
                    >
                >
              , const_class_type volatile
              , const_class_type
            >::type
        cv_class_type;

        typedef typename forward_as<cv_class_type, object_type>::type type;
    };

    template<typename Sig, typename SeqRef>
    struct get_object_pointer_result_type_seq
      : get_object_pointer_result_type<
            typename result_of::front<SeqRef>::type
          , Sig
        >
    {};
}}}

#endif
