//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_CATEGORY_ER_2010_HPP
#define BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_CATEGORY_ER_2010_HPP
#include <boost/assign/v2/detail/traits/value_container/fwd.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace container_aux{

    template<typename C> struct is_fifo : ::boost::mpl::false_{};

    template<typename V>
    struct is_lifo : ::boost::mpl::false_{};

    template<typename C>
    struct is_associative : ::boost::mpl::false_{};

    template<typename C>
    struct is_sorted : ::boost::mpl::false_{};

    template<typename C> struct is_array : ::boost::mpl::false_{};

    // FIFO
    template<typename T, typename A>
    struct is_fifo< std::queue<T, A> > : ::boost::mpl::true_{};

    // LIFO
    template<typename T, typename A>
    struct is_lifo< std::stack<T,A> > : ::boost::mpl::true_{};

}// container_aux

#define BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORY(SeqPar, IsCategory, Identifier, SeqArg)\
namespace container_aux{\
	template<BOOST_PP_SEQ_ENUM(SeqPar)>\
    struct IsCategory <\
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
	(typename T)( std::size_t sz), (is_array),
    boost::array, (T)(sz)
)    
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename K)(typename T)(typename C)(typename A), (is_sorted)(is_associative),
	std::map, (K)(T)(C)(A)
)
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename T)(typename C)(typename A), (is_sorted)(is_associative),
	std::set, (T)(C)(A)
)
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename K)(typename M)(typename H)(typename P)(typename A), (is_associative),
	boost::unordered_map, (K)(M)(H)(P)(A)
)
BOOST_ASSIGN_V2_TRAITS_CONTAINER_CATEGORIES( 
    (typename K)(typename H)(typename P)(typename A), (is_associative),
	boost::unordered_set, (K)(H)(P)(A)
)

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_TRAITS_VALUE_CONTAINER_CATEGORY_ER_2010_HPP
