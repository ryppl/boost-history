//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_PTR_CONTAINER_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_PTR_CONTAINER_ER_2011_HPP
#include <cstddef>
#include <memory> // std::allocator
#include <utility> // std::pair
#include <boost/assign/v2/support/traits/aux_/fwd_container.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_aux{

    
    // DATA-MEMBER

    template<typename PtrC>
    struct to_value_key{ typedef typename PtrC::key_type type; };

    template<typename PtrC>
    struct to_value_value : boost::remove_reference<
        typename PtrC::reference
    >{};

    template<typename PtrC>
    struct to_value_mapped : boost::remove_reference<
        typename PtrC::mapped_reference
    >{};

    template
    <
        class Key,
        class Mapped,
        class Compare,
        class CloneAllocator,
        class Allocator
    >
    struct to_value_value<
        boost::ptr_map<Key, Mapped, Compare, CloneAllocator, Allocator>
    >{

        typedef boost::ptr_map<
            Key, Mapped, Compare, CloneAllocator, Allocator
        > ptr_c_;
        typedef std::pair<
            const typename to_value_key<ptr_c_>::type, 
            typename to_value_mapped<ptr_c_>::type
        > type;
    };
    


    // ALLOCATOR

    template<typename PtrC>
    struct to_value_allocator
    {
        typedef std::allocator<typename to_value_value<PtrC>::type> type;
    };


    // ARRAY

    template<typename PtrC> struct helper_size{};

    template<typename T, std::size_t N, typename C>
    struct helper_size< boost::ptr_array<T, N, C> >
        : ::boost::mpl::size_t<
            N
        >{};

    template<typename PtrC>
    struct to_value_array
    {
        typedef boost::array<
            typename to_value_value<PtrC>::type,
            helper_size<PtrC>::value
        > type;
    };

    // SEQUENCE

    template<typename PtrC, template<typename, typename> class C>
    struct to_value_sequence {

        typedef C<
            typename to_value_value<PtrC>::type,
            typename to_value_allocator<PtrC>::type
        > type;

    };

    // MAP

    template<
        typename PtrC, 
        template<typename, typename, typename, typename> class C
    >
    struct to_value_map{

        typedef C<
            typename to_value_key<PtrC>::type,
            typename to_value_mapped<PtrC>::type,
            typename PtrC::key_compare,
            typename to_value_allocator<PtrC>::type
        > type;

    };

    // SET

    template<typename PtrC, template<typename, typename, typename> class C>
    struct to_value_set{

        typedef C<
            typename to_value_key<PtrC>::type,
            typename PtrC::key_compare,
            typename to_value_allocator<PtrC>::type
        > type;

    };

    // UNORDERED MAP

    template<typename PtrC,
        template<typename, typename, typename, typename, typename> class C>
    struct to_value_unordered_map{

        typedef C<
            typename to_value_key<PtrC>::type,
            typename to_value_mapped<PtrC>::type,
            typename PtrC::hasher,
            typename PtrC::key_equal,
            typename to_value_allocator<PtrC>::type
        > type;

    };

    // UNORDERED SET

    template<typename PtrC,
        template<typename, typename, typename, typename> class C>
    struct to_value_unordered_set{

        typedef C<
            typename to_value_key<PtrC>::type,
            typename PtrC::hasher,
            typename PtrC::key_equal,
            typename to_value_allocator<PtrC>::type
        > type;

    };

    // TO_VALUE_CONTAINER

    template<
        typename C // Pointer or Value container
    >
    struct to_value_container{ typedef C type; };

    // Array
    template<
        typename T,
        std::size_t N,
        typename A
    >
    struct to_value_container<
        boost::ptr_array<T, N, A>
    > : to_value_array<
        boost::ptr_array<T, N, A>
    >{};

    // Sequence

    template<
        typename T,
        typename C,
        typename A
    >
    struct to_value_container<
        boost::ptr_deque<T, C, A>
    > : to_value_sequence<
        boost::ptr_deque<T, C, A>,
        std::deque
    >{};

    template<
        typename T,
        typename C,
        typename A
    >
    struct to_value_container<
        boost::ptr_list<T, C, A>
    > : to_value_sequence<
        boost::ptr_list<T, C, A>,
        std::list
    >{};

    template<
        typename T,
        typename C,
        typename A
    >
    struct to_value_container<
        boost::ptr_vector<T, C, A>
    > : to_value_sequence<
        boost::ptr_vector<T, C, A>,
        std::vector
    >{};

    // Map

    template
    <
        class Key,
        class Mapped,
        class Compare,
        class CloneAllocator,
        class Allocator
    >
    struct to_value_container<
        boost::ptr_map<Key, Mapped, Compare, CloneAllocator, Allocator>
    > : to_value_map<
        boost::ptr_map<Key, Mapped, Compare, CloneAllocator, Allocator>,
        std::map
    >{};

    // Set

    template
    <
        class Key,
        class Compare,
        class CloneAllocator,
        class Allocator
    >
    struct to_value_container<
        boost::ptr_set<Key, Compare, CloneAllocator, Allocator>
    > : to_value_set<
        boost::ptr_set<Key, Compare, CloneAllocator, Allocator>,
        std::set
    >{};

    // Unordered map

    template
    <
        class Key,
        class Mapped,
        class Hash,
        class Pred,
        class CloneAllocator,
        class Allocator
    >
    struct to_value_container<
        boost::ptr_unordered_map<
            Key, Mapped, Hash, Pred, CloneAllocator, Allocator
        >
    > : to_value_unordered_map<
        boost::ptr_unordered_map<
            Key, Mapped, Hash, Pred, CloneAllocator, Allocator
        >,
        boost::unordered_map
    >{};

    // Unordered set

    template
    <
        class Key,
        class Hash,
        class Pred,
        class CloneAllocator,
        class Allocator
    >
    struct to_value_container<
        boost::ptr_unordered_set<
            Key, Hash, Pred, CloneAllocator, Allocator
        >
    > : to_value_unordered_set<
        boost::ptr_unordered_set<
            Key, Hash, Pred, CloneAllocator, Allocator
        >,
        boost::unordered_set
    >{};

}// container_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_PTR_CONTAINER_ER_2011_HPP
