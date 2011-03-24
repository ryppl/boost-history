//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_GET_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_GET_ER_2010_HPP
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/config.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/value_type.hpp>

#ifdef BOOST_MSVC
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/iterator/transform_iterator.hpp>
#else
#include <boost/range/adaptor/transformed.hpp>
#endif

#include <boost/assign/v2/ref/wrapper/get.hpp>

namespace boost{
namespace assign{
namespace v2{
//[syntax_ref_array_get
namespace ref{
namespace result_of{

    template<
        typename R // Range of reference-wrappers
    >
    struct transform_get/*<-*/{

        typedef ref::get_functor f_;
        #ifdef BOOST_MSVC
        typedef boost::iterator_range<
            boost::transform_iterator< f_,
                BOOST_DEDUCED_TYPENAME boost::range_iterator<R>::type
            >
        > type;
        #else
        typedef boost::range_detail::transform_range<f_, R> type;
        #endif
    }/*->*/;

}// result_of

    // Transforms rw by member function `get()`
    template<typename R>
    typename result_of::transform_get<R>::type
    transform_get(R& rw)/*<-*/
    {
        typedef typename result_of::transform_get<R>::type result_;
        #ifdef BOOST_MSVC
        return result_(
            boost::make_transform_iterator( boost::begin( rw ), get_functor() ),
            boost::make_transform_iterator( boost::end( rw ), get_functor() )
        );
        #else
        return result_( get_functor(), rw );
        #endif
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/
    
    template<typename R>
    typename result_of::transform_get<R const>::type
    transform_get(R const& rw )/*<-*/
    {
        typedef typename result_of::transform_get<R const>::type result_;
        #ifdef BOOST_MSVC
        return result_(
            boost::make_transform_iterator( boost::begin( rw ), get_functor() ),
            boost::make_transform_iterator( boost::end( rw ), get_functor() )
        );
        #else
        return result_( get_functor(), rw );
        #endif
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    struct get_adaptor/*<-*/{}/*->*/;

    template<typename R>
    typename result_of::transform_get<R>::type
    operator|(R& rw, get_adaptor)/*<-*/{
        return transform_get( rw );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

    template<typename R>
    typename result_of::transform_get<R const>::type
    operator|(R const& rw, get_adaptor)/*<-*/{
        return transform_get( rw );
    }BOOST_ASSIGN_V2_IGNORE(/*->*/;/*<-*/)/*->*/

//<-
namespace{
//->

    get_adaptor const _get/*<-*/
        = get_adaptor()/*->*/;

//<-
}
//->
}// ref
//]
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_ARRAY_GET_ER_2010_HPP
