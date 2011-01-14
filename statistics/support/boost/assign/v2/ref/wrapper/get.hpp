//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_GET_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_GET_ER_2010_HPP
#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace result_of{

    template<typename T>
    struct get : boost::add_reference<
        typename boost::unwrap_reference<
        	// cv taken care of by unwrap_reference
            typename boost::remove_reference<T>::type
        >::type
    >{};

}// result_of

    template<typename W>
    typename boost::lazy_enable_if<
    	boost::is_reference_wrapper<W>,
        ref::result_of::get<W>
    >::type
    get(W const& w)
    {
    	return w.get();	
    }

    // Functor

    struct get_functor
    {
    
    	template<typename S>
        struct result{};
        
        template<typename F, typename T>
        struct result<F(T)> : ref::result_of::get<T>{};
    
    	template<typename W>
    	typename boost::lazy_enable_if<
            boost::is_reference_wrapper<W>,
            ref::result_of::get<W>
    	>::type
        operator()(W const& w)const
        {
            return get( w );
    	}
        
    };

    // Range

namespace result_of{

    template<typename R>
    struct range_get_impl{
    
    	typedef ref::get_functor functor_;
    	typedef boost::range_detail::transform_range<functor_, R> type;
    
    };

    template<typename R>
    struct range_get : boost::lazy_enable_if<
    	boost::is_reference_wrapper<
            typename boost::range_value<
                typename boost::remove_cv<R>::type
            >::type
        >,
        range_get_impl<R>
    >{};

}// result_of

    template<typename R>
    typename ref::result_of::range_get<R>::type
    range_get(R& r)
    {
        typedef typename ref::result_of::range_get<R>::type result_;
        return result_(get_functor(), r);
    }
    template<typename R>
    typename ref::result_of::range_get<R const>::type
    range_get(R const& r)
    {
        typedef typename ref::result_of::range_get<R const>::type result_;
        return result_(get_functor(), r);
    }

    struct get_adaptor{};
    
namespace{
    get_adaptor const _get = get_adaptor();
}

    template<typename R>
    typename ref::result_of::range_get<R>::type
    operator|(R& r, get_adaptor){
        return range_get( r );
    }

    template<typename R>
    typename ref::result_of::range_get<R const>::type
    operator|(R const& r, get_adaptor){
        return range_get( r );
    }

}// ref
}// v2
}// assign
}// boost

#endif
