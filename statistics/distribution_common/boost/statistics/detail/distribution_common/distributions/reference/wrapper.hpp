//////////////////////////////////////////////////////////////////////////////
// distribution::common::reference::wrapper.hpp                             //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_COMMON_DISTRIBUTIONS_REFERENCE_WRAPPER_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_COMMON_DISTRIBUTIONS_REFERENCE_WRAPPER_HPP_ER_2009
#include <boost/call_traits.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
#include <boost/statistics/detail/distribution_common/meta/distribution.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace common{
namespace reference{

    template<typename D>
    class wrapper{

        public:

        typedef typename remove_cv<
            typename remove_reference<D>::type
        >::type distribution_type;
        
        typedef typename meta::value<distribution_type>::type value_type;

        wrapper(){}
        wrapper(typename call_traits<D>::param_type dist):dist_(dist){}
        wrapper(const wrapper& that):dist_(that.dist_){}
        wrapper& operator=(const wrapper& that)
        {
            if(&that!=this)
            {
                this->dist_ = that.dist_;
            }
            return (*this);
        }

        
        typename call_traits<D>::const_reference 
        distribution()const
        {
            return this->dist_;
        }
                
        private:
        typename call_traits<D>::value_type dist_;
        
    };
    
}// reference
}// common
}// distribution
}// detail
}// statistics
}// boost

#endif