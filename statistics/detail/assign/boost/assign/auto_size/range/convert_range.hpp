//////////////////////////////////////////////////////////////////////////////
// assign::detail::range::convert.hpp                                       //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_RANGE_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_RANGE_CONVERT_ER_2010_HPP
#include <boost/mpl/assert.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/assign/auto_size/range/iterator_converter.hpp>
#include <boost/assign/auto_size/traits/reference_traits.hpp>

// Usage:
//   convert_range<T>(r)
// changes U = range_reference<R>::type to T.
//   convert_range(r)
// changes U to reference_traits::convert_to<U>::type

namespace boost{
namespace assign{
namespace detail{

template<typename Rng,typename V>
struct convert_range_reference 
 : detail::converted_iterator_reference<
     typename boost::range_iterator<Rng>::type,
     V
 >{};

namespace result_of{
    
    template<typename Rng,typename V,
         typename R = typename convert_range_reference<Rng,V>::type>
    struct convert_range
    {
        typedef typename boost::range_iterator<Rng>::type base_it_;
        typedef detail::converted_iterator<base_it_,V,R> new_it_;
        typedef boost::iterator_range<new_it_> type;

        static type call(Rng& r){
            return type(
                convert_iterator<V,R>(boost::begin(r)),
                convert_iterator<V,R>(boost::end(r))
            );
        }
    };

}// result_of
}// detail

    // convert_range

    template<typename V,typename R,typename Rng>
    typename detail::result_of::convert_range<Rng,V,R>::type 
    convert_range( Rng& r,
        typename boost::disable_if<boost::is_same<R,use_default> >::type* = 0)
    {    
        typedef detail::result_of::convert_range<Rng,V,R> caller_;
        return caller_::call( r );   
    }

    template<typename V,typename R,typename Rng>
    typename detail::result_of::convert_range<const Rng,V,R>::type 
    convert_range( const Rng& r,
        typename boost::disable_if<boost::is_same<R,use_default> >::type* = 0)
    {    
        typedef detail::result_of::convert_range<const Rng,V,R> caller_;
        return caller_::call( r );   
    }

    template<typename V,typename R,typename Rng>
    typename detail::result_of::convert_range<Rng,V>::type 
    convert_range( Rng& r,
        typename boost::enable_if<boost::is_same<R,use_default> >::type* = 0)
    {    
        typedef detail::result_of::convert_range<Rng,V> caller_;
        return caller_::call( r );   
    }

    template<typename V,typename R,typename Rng>
    typename detail::result_of::convert_range<const Rng,V>::type 
    convert_range( const Rng& r,
        typename boost::enable_if<boost::is_same<R,use_default> >::type* = 0)
    {    
        typedef detail::result_of::convert_range<const Rng,V> caller_;
        return caller_::call( r );   
    }

/*
   // operator|
       
    template<typename T,typename Rng>
    inline typename detail::result_of::convert_range<Rng,T>::type 
    operator|( Rng& r, const detail::adaptor::convert_range<T>& f )
    {
        return convert_range<T>(r);   
    }

    template<typename T,typename Rng>
    inline typename detail::result_of::convert_range<const Rng,T>::type 
    operator|( const Rng& r, const detail::adaptor::convert_range<T>& f )
    {
        return convert_range<T>(r);   
    }

    template<typename Rng>
    inline typename detail::result_of::convert_range<Rng>::type 
    operator|( Rng& r, const detail::adaptor::convert_range<>& f )
    {
        return convert_range(r);   
    }

    template<typename Rng>
    inline typename detail::result_of::convert_range<const Rng>::type 
    operator|( const Rng& r, const detail::adaptor::convert_range<>& f )
    {
        return convert_range(r);   
    }
*/
}// assign
}// boost

#endif
