//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_ADAPTOR_GET_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_ADAPTOR_GET_ER_2010_HPP
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
namespace ref{
namespace range_aux{
namespace result_of{

    template<typename R>
    struct range_get{
    
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
    };

}// result_of

    template<typename R>
    typename range_aux::result_of::range_get<R>::type
    range_get(R& r)
    {
        typedef typename range_aux::result_of::range_get<R>::type result_;
        #ifdef BOOST_MSVC
		return result_(
        	boost::make_transform_iterator( boost::begin(r), get_functor() ),
        	boost::make_transform_iterator( boost::end(r), get_functor() )
        );
        #else
        return result_(get_functor(), r);
		#endif
    }
    template<typename R>
    typename range_aux::result_of::range_get<R const>::type
    range_get(R const& r)
    {
        typedef typename range_aux::result_of::range_get<R const>::type result_;
        #ifdef BOOST_MSVC
		return result_(
        	boost::make_transform_iterator( boost::begin(r), get_functor() ),
        	boost::make_transform_iterator( boost::end(r), get_functor() )
        );
        #else
        return result_(get_functor(), r);
		#endif
    }

    struct get_adaptor{};
    
    template<typename R>
    typename range_aux::result_of::range_get<R>::type
    operator|(R& r, get_adaptor){
        return range_get( r );
    }

    template<typename R>
    typename range_aux::result_of::range_get<R const>::type
    operator|(R const& r, get_adaptor){
        return range_get( r );
    }

}// range_aux
namespace result_of{


    template<typename R>
    struct range_get : boost::lazy_enable_if<
    	boost::is_reference_wrapper<
            typename boost::range_value<
                typename boost::remove_cv<R>::type
            >::type
        >,
        range_aux::result_of::range_get<R>
    >{};

}// result_of

namespace{
    range_aux::get_adaptor const _get = range_aux::get_adaptor();
}

}// ref
}// v2
}// assign
}// boost

#endif
