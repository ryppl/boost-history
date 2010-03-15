//////////////////////////////////////////////////////////////////////////////
// assign::detail::convert.hpp                                            //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CONVERTER_ER_2010_HPP
#include <boost/mpl/assert.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>

// Usage:
//   convert_range<T>(r)

// There must be a better way to convert than 'transform'. An iterator adaptor
// that alters the reference type should suffice.

namespace boost{
namespace assign{
namespace detail{
namespace adaptor{

template<typename T>
struct convert{

    convert(){}

    typedef T result_type;

    template<typename U>
    T operator()(U& arg)const{ return arg; }

    template<typename U>
    T operator()(const U& arg)const{ return arg; }

};

}// adaptor

namespace result_of{
	
    template<typename T,typename Rng,
        typename U = typename boost::range_reference<Rng>::type>
    struct convert_range
    {
        typedef adaptor::convert<T> adaptor_;
        typedef boost::transform_range<adaptor_,Rng> type;

/*
template<typename I,typename T>
class iterator_converter : boost::iterator_adaptor<
    iterator_converter<I,T>, 
    I, 
    use_default,
    typename boost::remove_reference<T>::type, 
    T, 
    use_default
>{
    typedef boost::iterator_adaptor<
        iterator_converter<I,T>, 
        I, 
        use_default,
        typename boost::remove_reference<T>::type, 
       T, 
       use_default
    > super_;
    
    iterator_converter(const I& base):super_(base){}
}; 
*/
        static void internal_check(){
            BOOST_MPL_ASSERT((boost::is_convertible<U,T>));
            typedef typename boost::range_reference<type>::type new_ref_;
            BOOST_MPL_ASSERT((boost::is_same<new_ref_,T>));
        }
        static type call(Rng& r){
            internal_check();
            return type(adaptor_(),r);
        }
    };
    
}

    template<typename T,typename Rng>
    typename detail::result_of::convert_range<T,Rng>::type 
    convert_range( Rng& r)
    {    
        typedef detail::result_of::convert_range<T,Rng> caller_;
        return caller_::call( r );   
    }

    template<typename T,typename Rng>
    typename detail::result_of::convert_range<T,const Rng>::type 
    convert_range( const Rng& r)
    {    
        return convert_range<T,const Rng>(r);
    }
    
    template<typename T,typename Rng>
    inline typename detail::result_of
    	::convert_range<T,Rng>::type 
    operator|( Rng& r, const detail::adaptor::convert<T>& f )
    {
        return convert_range<T>(r);   
    }

    template<typename T,typename Rng>
    inline typename detail::result_of
    	::convert_range<T,const Rng>::type 
    operator|( const Rng& r, const detail::adaptor::convert<T>& f )
    {
        return convert_range<T>(r);   
    }


}// detail
}// assign
}// boost

#endif
