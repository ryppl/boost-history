//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_VALUE_CONTAINER_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_VALUE_CONTAINER_ER_2011_HPP
#include <boost/assign/v2/support/traits/aux_/fwd_container.hpp>
#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/multi_array/base.hpp> 
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_aux{

// CATEGORIES

    template<typename C> 
    struct value_is_fifo : ::boost::mpl::false_{};

    template<typename V>
    struct value_is_lifo : ::boost::mpl::false_{};

    template<typename C>
    struct value_is_associative : ::boost::mpl::false_{};

    template<typename C>
    struct value_is_sorted : ::boost::mpl::false_{};

    template<typename C> 
    struct value_is_array : ::boost::mpl::false_{};

    template<typename C> 
    struct value_is_map : ::boost::mpl::false_{};

    typedef ::boost::detail::multi_array::multi_array_base multi_array_base_;

    template<typename C>
    struct value_is_multi_array
        : boost::is_base_of<multi_array_base_, C>
    {};

}// container_aux

#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORY(SeqPar, IsCategory, Identifier, SeqArg)\
namespace container_aux{\
    template<BOOST_PP_SEQ_ENUM(SeqPar)>\
    struct BOOST_PP_CAT(value_,IsCategory) <\
        Identifier<BOOST_PP_SEQ_ENUM(SeqArg)> \
    > : ::boost::mpl::true_{};\
}\
/**/
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES_HELPER(r, data, elem)\
    BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORY(\
        BOOST_PP_SEQ_ELEM(0, data),\
        elem,\
        BOOST_PP_SEQ_ELEM(1, data),\
        BOOST_PP_SEQ_ELEM(2, data)\
    )
/**/
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES(SeqPar, SeqIsCategory, Identifier, SeqArg)\
    BOOST_PP_SEQ_FOR_EACH(\
        BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES_HELPER,\
        (SeqPar)(Identifier)(SeqArg),\
        SeqIsCategory\
    )\
/**/
    
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename T)( std::size_t sz ), (is_array),
    boost::array, (T)(sz)
)    
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename K)(typename T)(typename C)(typename A), 
    (is_associative)(is_map)(is_sorted),
    std::map, (K)(T)(C)(A)
)
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename T)(typename A), (is_fifo),
    std::queue, (T)(A)
) 
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename T)(typename C)(typename A), (is_sorted)(is_associative),
    std::set, (T)(C)(A)
)
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename T)(typename A), (is_lifo),
    std::stack, (T)(A)
) 
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename K)(typename M)(typename H)(typename P)(typename A), 
    (is_associative)(is_map),
    boost::unordered_map, (K)(M)(H)(P)(A)
)
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename K)(typename H)(typename P)(typename A), (is_associative),
    boost::unordered_set, (K)(H)(P)(A)
)

namespace container_aux{

// DATA-MEMBER

    template<
        typename C    // Ptr or value container
    >
    struct value_key{ typedef typename C::key_type type; };

    template<
        typename C    // Ptr or value multi-array
    >
    struct value_element{ typedef typename C::element type; };

    template<
        typename C    // Ptr or value container
    >
    struct value_value{ typedef typename C::value_type type; };

    template<
        typename C    // Ptr or value container
    >
    struct value_mapped{
        typedef typename C::mapped_type type;
    };


// HAS_VALUE

    template<typename T>
    struct value_has_value_type{

        typedef typename boost::type_traits::yes_type yes_;
        typedef typename boost::type_traits::no_type no_;


        template<typename U>
        static yes_ test(U*, typename U::value_type* p = 0);
        static no_ test(...);

        BOOST_STATIC_CONSTANT(
            bool,
            value = sizeof( test((T*)0) ) == sizeof( yes_ )
        );
        typedef ::boost::mpl::bool_<value> type;
    };
    
// HAS_PUSH

    template<typename C, typename T = typename C::value_type>
    struct value_has_push{

        typedef typename boost::type_traits::yes_type yes_;
        typedef typename boost::type_traits::no_type no_;

        template<typename U,void (U::*fp)(T const&)>
        struct helper{
            // See http://lists.boost.org/Archives/boost/2002/03/27233.php
        };

        template<typename U>
        static yes_ test(U*, helper<U, &U::push>* p = 0);
        static no_ test(...);

        BOOST_STATIC_CONSTANT(
            bool,
            value = sizeof( test((C*)0) ) == sizeof( yes_ )
        );

        typedef ::boost::mpl::bool_<value> type;

    };

    template<typename C>
    struct value_has_push_deduced_value : value_has_push<C>{};

}// container_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_VALUE_CONTAINER_ER_2011_HPP