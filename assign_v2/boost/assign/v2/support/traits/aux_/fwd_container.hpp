//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_FWD_CONTAINER_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_FWD_CONTAINER_ER_2011_HPP
#include <cstddef>

// Consider this as partial replacement (not complete)
// #include <boost/detail/container_fwd.hpp>

namespace std{

    // www.sgi.com/tech/stl/Deque.html
    template<typename T, typename A> class deque;

    // http://www.sgi.com/tech/stl/List.html
    template<typename T, typename A> class list;

    // http://www.sgi.com/tech/stl/Map.html
    template<typename K, typename T, typename C, typename A> class map;

    // http://www.sgi.com/tech/stl/queue.html
    template<typename T, typename S> class queue;

    // http://www.sgi.com/tech/stl/set.html
    template<typename T, typename C, typename A> class set;

    // http://www.sgi.com/tech/stl/stack.html
    template<typename T, typename S> class stack;

    // http://www.sgi.com/tech/stl/Vector.html
    template<typename T, typename A> class vector;

}// std

#include <boost/circular_buffer_fwd.hpp>
#include <boost/unordered/unordered_map_fwd.hpp>
#include <boost/unordered/unordered_set_fwd.hpp>
// Include fwd headers as they become available

namespace boost{

    // http://www.boost.org/doc/libs/release/doc/html/array.html
    template<typename T, std::size_t size> class array;

    // http://www.boost.org/doc/libs/release/libs/circular_buffer/index.html
    //template<typename T, typename Alloc> class circular_buffer;

namespace detail{
namespace multi_array{

    struct multi_array_base;

}// multi_array
}// detail

/*
   template <
        class Key,
        class Hash, 
        class Pred, 
        class Alloc
    >
    class unordered_set;

    template <
        class Key, 
        class Mapped,
        class Hash,
        class Pred,
        class Alloc
    >
    class unordered_map;

    template<
        class Key, 
        class Mapped,
        class Hash,
        class Pred,
        class Alloc
    >
    class unordered_multimap;

    template<
        class Key,
        class Hash, 
        class Pred, 
        class Alloc
    > 
    class unordered_multiset;
*/

    // POINTER-CONTAINERS
    
    template
    <
        class T,
        std::size_t N, 
        class CloneAllocator
    >
    class ptr_array;

    template
    <
        class T,
        class CloneAllocator,
        class Allocator
    >
    class ptr_circular_buffer;

    template
    <
        class T,
        class CloneAllocator,
        class Allocator
    >
    class ptr_deque;

    template
    <
        class T,
        class CloneAllocator,
        class Allocator
    >
    class ptr_list;

    template
    <
        class Key,
        class T,
        class Compare,
        class CloneAllocator,
        class Allocator
    >
    class ptr_map;

    template
    <
        class Key,
        class Compare,
        class CloneAllocator,
        class Allocator
    >
    class ptr_set;

    template
    <
        class Key,
        class Mapped,
        class Hash,
        class Pred,
        class CloneAllocator,
        class Allocator
    >
    class ptr_unordered_map;

    template
    <
        class Key,
        class Hash,
        class Pred,
        class CloneAllocator,
        class Allocator
    >
    class ptr_unordered_set;


    template
    <
        class T,
        class CloneAllocator,
        class Allocator
    >
    class ptr_vector;
    
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_TRAITS_AUX_FWD_CONTAINER_ER_2011_HPP
