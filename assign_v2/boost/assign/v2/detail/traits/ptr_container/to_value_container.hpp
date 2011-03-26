//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TRAITS_PTR_CONTAINER_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TRAITS_PTR_CONTAINER_TRAITS_ER_2010_HPP
#include <cstddef>
#include <memory> // std::allocator
#include <boost/assign/v2/detail/traits/value_container/fwd.hpp>
#include <boost/assign/v2/detail/traits/ptr_container/fwd.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_aux{

	// VALUE

    template<typename PtrC>
    struct to_value_value : boost::remove_reference<
        typename PtrC::reference
    >{};

	template<typename PtrC>
    struct to_value_mapped : boost::remove_reference<
		typename PtrC::mapped_reference    
    >{};

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

    template<typename PtrC, template<typename, typename, typename, typename> class C>
    struct to_value_map{

        typedef C<
            typename PtrC::key_type,
            typename to_value_mapped<PtrC>::type,
            typename PtrC::key_compare,
			typename to_value_allocator<PtrC>::type
        > type;

    };

	// SET

    template<typename PtrC, template<typename, typename, typename> class C>
    struct to_value_set{

        typedef C<
            typename PtrC::key_type,
            typename PtrC::key_compare,
            typename to_value_allocator<PtrC>::type
        > type;

    };

	// UNORDERED MAP

    template<typename PtrC,
        template<typename, typename, typename, typename, typename> class C>
    struct to_value_unordered_map{

        typedef C<
            typename PtrC::key_type,
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
            typename PtrC::key_type,
            typename PtrC::hasher,
            typename PtrC::key_equal,
            typename to_value_allocator<PtrC>::type
        > type;

    };

    // TO_VALUE_CONTAINER

    template<typename PtrC>
    struct to_value_container
    {
        typedef PtrC type;
    };

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
        boost::ptr_unordered_map<Key, Mapped, Hash, Pred, CloneAllocator, Allocator>
    > : to_value_unordered_map<
        boost::ptr_unordered_map<Key, Mapped, Hash, Pred, CloneAllocator, Allocator>,
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
        boost::ptr_unordered_set<Key, Hash, Pred, CloneAllocator, Allocator>
    > : to_value_unordered_set<
        boost::ptr_unordered_set<Key, Hash, Pred, CloneAllocator, Allocator>,
        boost::unordered_set
    >{};

}// container_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_TRAITS_CONTAINER_FWD_PTR_ER_2010_HPP
