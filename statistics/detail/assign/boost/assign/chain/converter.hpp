//////////////////////////////////////////////////////////////////////////////
// assign::detail::converter.hpp                                            //
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
#include <boost/type_traits/is_convertible.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace functional{

template<typename T>
struct converter{

    converter(){}

    typedef T result_type;

	template<typename U>
    T operator()(U& arg)const{ return arg; }

	template<typename U>
    T operator()(const U& arg)const{ return arg; }

};

}// functional

namespace result_of{
	
    template<typename T,typename Rng,
        typename U = typename boost::range_reference<Rng>::type>
    struct convert_range
    {
        
        BOOST_MPL_ASSERT((
            boost::is_convertible<U,T>
        ));
		typedef functional::converter<T> adaptor_;
        typedef boost::transform_range<adaptor_,Rng> type;
        static type call(Rng& r){
        	return type(adaptor_(),r);
        }
    };
    
}

    // This seems to take care of const Rng. Overloads with const Rng have
    // compile deficiencies. TODO verify this.

    template<typename T,typename Rng>
    typename detail::result_of::convert_range<T,Rng>::type 
    convert_range( Rng& r)
    {    
        typedef detail::result_of::convert_range<T,Rng> caller_;
        return caller_::call( r );   
    }
    
    template<typename T,typename Rng>
    inline typename detail::result_of
    	::convert_range<T,Rng>::type 
    operator|( Rng& r, const detail::functional::converter<T>& f )
    {
        return convert_range<T>(r);   
    }


}// detail
}// assign
}// boost

#endif
