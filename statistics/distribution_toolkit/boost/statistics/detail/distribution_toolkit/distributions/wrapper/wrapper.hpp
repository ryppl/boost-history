/////////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::distributions::wrapper::wrapper.hpp                   //
//                                                                             //
//  (C) Copyright 2009 Erwann Rogard                                           //
//  Use, modification and distribution are subject to the                      //
//  Boost Software License, Version 1.0. (See accompanying file                //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)           //
/////////////////////////////////////////////////////////////////////////////////
/*
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_WRAPPER_WRAPPER_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_WRAPPER_WRAPPER_HPP_ER_2009
#include <boost/call_traits.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/not.hpp>
#include <boost/statistics/detail/distribution_common/meta/inherit_policy.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{

	// TODO deprecated

    // Useful for serializing or as a reference wrapper (D = const E&)
    template<typename D>
    class wrapper : distribution::meta::inherit_policy<
        typename remove_cv<
            typename remove_reference<D>::type
        >::type 
    >{
        public:

        typedef typename remove_cv<
            typename remove_reference<D>::type
        >::type distribution_type;

        typedef typename 
            distribution::common::meta::value<distribution_type>::type 
                value_type;
        
        wrapper(){}
        wrapper(typename call_traits<D>::param_type d):d_(d){}
        wrapper(const wrapper& that):d_(that.d_){}
    
        typename call_traits<D>::const_reference 
        const_reference()const{ return this->d_; }
    
        operator distribution_type ()const{ return this->d_; }
    
        private:

        //template<class A>
        //void serialize(
        //    A & ar, 
        //    const unsigned int version
        //)
        //{
        //    BOOST_MPL_ASSERT((
        //        mpl::not_<
        //            is_reference<D>
        //        >
        //    ));
		//
        //    ar & d_;
        //}
        
        typename call_traits<D>::value_type d_;
    };

}// distribution
}// toolkit

}// detail
}// statistics
}// boost

#endif
*/

