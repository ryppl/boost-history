///////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::joint_view::binder_iterator.hpp               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_FUSION_JOINT_VIEW_BINDER_ITERATOR_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_FUSION_JOINT_VIEW_BINDER_ITERATOR_HPP_ER_2009
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/statistics/detail/fusion/joint_view/binder.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace fusion{
namespace joint_view{

	template<typename Binder, bool is_left = true>
    struct binder_iterator
    {
		typedef Binder binder_;

        template<typename It>
		struct apply{
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

			typedef boost::iterator_range<type> range_type;
            static range_type call(const binder_& binder,It b,It e){
            	return range_type(
                	call(binder,b),
                    call(binder,e)
                );
            }

		};
    };
    
    template<typename Binder,typename It>
    typename binder_iterator<Binder>::template apply<It>::type
    make_binder_iterator(const Binder& binder,It it)
    {
    	typedef binder_iterator<Binder> meta_;
    	return meta_::template apply<It>::call(binder,it);
    }

    template<typename Binder,typename It>
    typename binder_iterator<Binder>::template apply<It>::range_type
    make_binder_range(const Binder& binder,It b,It e)
    {
    	typedef binder_iterator<Binder> meta_;
    	return meta_::template apply<It>::call(binder,b,e);
    }

    
}// joint_view
}// fusion
}// detail
}// statistics
}// boost

#endif