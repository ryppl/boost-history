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
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/assign/auto_size/conversion/iterator.hpp>
#include <boost/assign/auto_size/traits/reference_traits.hpp>

// Usage:
//   convert_range<T>(r)
// changes U = range_reference<R>::type to T.
//   convert_range(r)
// changes U to reference_traits::convert_to<U>::type

namespace boost{
namespace assign{
namespace detail{
namespace adaptor{

struct use_default;

template<typename T = use_default>
struct convert_range{ convert_range(){} };

}// adaptor

namespace result_of{

    template<
        typename Rng,
        typename T = typename reference_traits::convert_to<
            typename boost::range_reference<Rng>::type
        >::type
    >
    struct convert_range
    {
        typedef adaptor::convert_range<T> adaptor_;
        typedef typename boost::range_iterator<Rng>::type base_it_;
        typedef detail::iterator_converter<base_it_,T> new_it_;
        typedef boost::iterator_range<new_it_> type;

        static void internal_check(){
            typedef typename boost::range_reference<Rng>::type u_;
            BOOST_MPL_ASSERT((boost::is_convertible<u_,T>));
            typedef typename boost::range_reference<type>::type new_ref_;
            BOOST_MPL_ASSERT((boost::is_same<new_ref_,T>));
        }
        static type call(Rng& r){
            internal_check();
            return type(
                detail::convert_iterator<T>(boost::begin(r)),
                detail::convert_iterator<T>(boost::end(r))
            );
        }
    };

}

   // convert_range

    template<typename T,typename Rng>
    typename detail::result_of::convert_range<Rng,T>::type 
    convert_range( Rng& r)
    {    
        typedef detail::result_of::convert_range<Rng,T> caller_;
        return caller_::call( r );   
    }

    template<typename T,typename Rng>
    typename detail::result_of::convert_range<const Rng,T>::type 
    convert_range( const Rng& r)
    {    
        typedef detail::result_of::convert_range<const Rng,T> caller_;
        return caller_::call( r );   
    }

/*
    template<typename Rng>
    typename detail::result_of::convert_range<Rng>::type 
    convert_range( Rng& r)
    {    
        typedef detail::result_of::convert_range<Rng> caller_;
        return caller_::call( r );   
    }

    template<typename Rng>
    typename detail::result_of::convert_range<const Rng>::type 
    convert_range( const Rng& r)
    {    
        typedef detail::result_of::convert_range<const Rng> caller_;
        return caller_::call( r );   
    }

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
}// detail
}// assign
}// boost

#endif
