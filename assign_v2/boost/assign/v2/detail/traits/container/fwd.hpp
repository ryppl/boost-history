//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_TRAITS_CONTAINER_FWD_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_TRAITS_CONTAINER_FWD_ER_2010_HPP
#include <cstddef>

namespace std{

    // www.sgi.com/tech/stl/Deque.html
    template<typename T,typename A> class deque;

    // http://www.sgi.com/tech/stl/List.html
    template<typename T,typename A> class list;

    // http://www.sgi.com/tech/stl/Map.html
    template<typename K,typename T,typename C,typename A> class map;

    // http://www.sgi.com/tech/stl/queue.html
    template<typename T,typename S> class queue;

    // http://www.sgi.com/tech/stl/set.html
    template<typename T,typename C,typename A> class set;

    // http://www.sgi.com/tech/stl/stack.html
    template<typename T,typename S> class stack;

    // http://www.sgi.com/tech/stl/Vector.html
    template<typename T,typename A> class vector;

}// std
namespace boost{

    // http://www.boost.org/doc/libs/release/doc/html/array.html
    template<typename T, std::size_t size> class array;

    // http://www.boost.org/doc/libs/release/libs/circular_buffer/index.html
    template<typename T, typename Alloc> class circular_buffer;

}// boost

#endif
