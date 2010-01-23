///////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::joint_view::bind_iterator.hpp               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_JOINT_VIEW_BINDER_ITERATOR_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_JOINT_VIEW_BINDER_ITERATOR_HPP_ER_2009
#include <boost/utility/result_of.hpp>
#include <boost/range.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace fusion{
namespace joint_view{
namespace result_of{

	// e.g Binder = joint_view::binder<>
	template<typename Binder,typename It>
    struct bind_iterator
    {
        typedef typename iterator_value<It>::type 				seq2_;
        typedef typename boost::add_const<seq2_>::type 			c_;
        typedef typename boost::add_reference<c_>::type 		cref_;
        typedef typename boost::result_of<binder_(cref_)>::type res_;
        typedef boost::function<res_(cref_)> 					fun_; 
        typedef boost::transform_iterator<fun_,It>  			type;
        static type call(const binder_& binder,It it)
        {
            return type(it,binder);
        }
    };

}

    template<typename Binder,typename It>
    typename detail::joint_view::result_of::bind_iterator<Binder,It>::type
    bind_iterator(const Binder& binder,It it)
    {
    	typedef detail::joint_view::result_of::bind_iterator<Binder,It> meta_;
    	return meta_::call(binder,it);
    }
    
}// joint_view
}// fusion
}// detail
}// statistics
}// boost

#endif