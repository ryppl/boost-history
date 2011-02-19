//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_CONTAINER_PTR_TO_VALUE_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_PTR_TO_VALUE_ER_2010_HPP
#include <memory> // std::allocator
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/assign/v2/detail/traits/container/ptr_fwd.hpp>
#include <boost/assign/v2/detail/traits/container/fwd.hpp>
#include <boost/assign/v2/detail/traits/container/static_size.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_traits{

    template<typename V>
    struct ptr_to_value{
    
        struct is_applicable : ::boost::mpl::false_{};
    
    };

namespace ptr_to_value_aux{

    template<typename A,typename T> struct allocator{};
    
    template<typename T>
    struct allocator<std::allocator<void*>, T>
    {
        typedef std::allocator<T> type;
    };

    template<typename V>
    struct common
    {

        struct is_applicable : ::boost::mpl::true_{};

        typedef typename boost::remove_reference<
            typename V::reference
        >::type value_type;
        
        typedef typename ptr_to_value_aux::allocator<
            typename V::allocator_type,
            value_type
        >::type allocator_type;
    };

    template<typename V,template<typename, std::size_t> class F>
    struct array_like{    

        // there is no allocator, hence can't derive from common<V>

        struct is_applicable : ::boost::mpl::true_{};

        typedef typename boost::remove_reference<
            typename V::reference
        >::type value_type;
    
        typedef F<
            typename array_like::value_type,
            container_traits::static_size<V>::value 
        > container_type;
        
    };

    template<typename V,template<typename,typename> class F>
    struct seq_like : ptr_to_value_aux::common<V>{    

        typedef F<
            typename seq_like::value_type,
            typename seq_like::allocator_type
        > container_type;
        
    };

    template<typename V,template<typename,typename,typename,typename> class F>
    struct map_like : ptr_to_value_aux::common<V>{    

        typedef F<
            typename V::key_type,
            typename map_like::value_type,
            typename V::key_compare,
            typename map_like::allocator_type
        > container_type;
        
    };

    template<typename V,template<typename,typename,typename> class F>
    struct set_like : ptr_to_value_aux::common<V>{    

        typedef F<
            typename V::key_type,
            typename V::key_compare,
            typename set_like::allocator_type
        > container_type;
        
    };

    template<typename V,
        template<typename,typename,typename,typename,typename> class F>
    class unordered_map_like : ptr_to_value_aux::common<V>{

        typedef F<
            typename V::key_type,
            typename unordered_map_like::value_type,
            typename V::hasher,
            typename V::key_equal,
            typename unordered_map_like::allocator_type
        > container_type;
    
    };

    template<typename V,
        template<typename,typename,typename,typename> class F>
    class unordered_set_like : ptr_to_value_aux::common<V>{

        typedef F<
            typename V::key_type,
            typename V::hasher,
            typename V::key_equal,
            typename unordered_set_like::allocator_type
        > container_type;
    
    };

    
}// ptr_to_value_aux


    // Array
    template<
        typename T,
        std::size_t N,
        typename A
    > 
    struct ptr_to_value<
        boost::ptr_array<T,N,A>
    > : ptr_to_value_aux::array_like<
        boost::ptr_array<T,N,A>,
        boost::array
    >{};

    // Sequence

    template<
        typename T,
        typename C,
        typename A
    > 
    struct ptr_to_value<
        boost::ptr_deque<T,C,A>
    > : ptr_to_value_aux::seq_like<
        boost::ptr_deque<T,C,A>,
        std::deque
    >{};

    template<
        typename T,
        typename C,
        typename A
    > 
    struct ptr_to_value<
        boost::ptr_list<T,C,A>
    > : ptr_to_value_aux::seq_like<
        boost::ptr_list<T,C,A>,
        std::list
    >{};

    template<
        typename T,
        typename C,
        typename A
    > 
    struct ptr_to_value<
        boost::ptr_vector<T,C,A>
    > : ptr_to_value_aux::seq_like<
        boost::ptr_vector<T,C,A>,
        std::vector
    >{};

}// container_traits
}// v2
}// assign
}// boost

#endif
