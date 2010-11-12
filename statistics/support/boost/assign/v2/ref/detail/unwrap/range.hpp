//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_UNWRAP_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_UNWRAP_RANGE_ER_2010_HPP
#include <boost/type_traits/add_reference.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/assign/v2/ref/detail/unwrap/iterator.hpp>

// Note : In Boost.Assign.v2, this is deprecated but it could be used 
// elsewhere.


// See iterator.hpp

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace unwrap_aux{

	template<typename R>
    struct is_range : is_unwrapped_iterator<
        typename boost::range_iterator<R>::type
    >{};

// TODO remove
// I guess this is subsumed by range_reference
//    template<typename R>
//    struct reference_of_unwrapped_range
//     : unwrap::reference_of_iterator<
//         typename boost::range_iterator<R>::type
//     >{};

    template<typename R, bool = unwrap_aux::is_range<R>::value>
    struct result_of_unwrap_range
    {
        typedef typename boost::range_iterator<R>::type base_it_;
        typedef typename result_of::unwrap_iterator<base_it_>::type it_;
        typedef boost::iterator_range<it_> type;

        static type call( R& r){
            return type(
                ref::unwrap_iterator( boost::begin( r ) ),
                ref::unwrap_iterator( boost::end( r ) )
            );
        }
    };

    // No need to unwrap twice
    template<typename R>
    struct result_of_unwrap_range<R,true>
    {
        typedef R type; // don't change this to R&
        static type call( R& r){ return r; }
    };

}// unwrap_aux

	template<typename R>
    struct is_unwrapped_range : unwrap_aux::is_range<R>{};

namespace result_of{

	template<typename R>
    struct unwrap_range : unwrap_aux::result_of_unwrap_range<R>{};

}// result_of

    template<typename R>
    typename result_of::unwrap_range<R>::type 
    unwrap_range( R& r)
    {    
        typedef result_of::unwrap_range<R> result_;
        return result_::call( r );
    }

    template<typename R>
    typename result_of::unwrap_range<R const>::type 
    unwrap_range( R const& r)
    {    
        typedef result_of::unwrap_range<R const> result_;
        return result_::call( r );
    }

}// ref
}// v2
}// assign
}// boost

#endif
