//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TRAITS_CONTAINER_FWD_PTR_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TRAITS_CONTAINER_FWD_PTR_ER_2010_HPP
#include <cstddef>

namespace boost{

    template
    <
        class T,
        std::size_t N, // TODO verify
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
        class T,
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

#endif
