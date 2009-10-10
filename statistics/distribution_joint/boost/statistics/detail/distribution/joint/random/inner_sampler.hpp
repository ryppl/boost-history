//////////////////////////////////////////////////////////////////////////////
// distribution::joint::random::inner_sampler.hpp                           //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_RANDOM_INNER_SAMPLER_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_RANDOM_INNER_SAMPLER_HPP_ER_2009
#include <algorithm>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{


    // Usage:
    // std::for_each(
    //      b,  // *b is a range
    //      e,
    //      make_inner_sampler(urng,random_dist)
    // );
    //
    // U = V, V&
    template<typename U, typename D>
    class inner_sampler : D{

        public:
        inner_sampler(){}
        inner_sampler(
            typename call_traits<U>::param_type u, 
            const D& d
        ):D(d),urng_(u){}
        inner_sampler(const inner_sampler& that)
            :D(static_cast<const D&>(that)),urng_(that.urng_){}
        inner_sampler& operator=(const inner_sampler& that)
        {
            static_cast<D&>(*this) = static_cast<const D&>( that );
            this->urng_ = that.urng_;

        }
        
        template<typename R>
        void operator()(R& r){
            typedef typename D::result_type res_;
            
            res_  res = static_cast<D&>(*this)(this->ref_urng());
            std::copy(
                boost::begin(res),
                boost::end(res),
                boost::begin(r)
            );
        
        }
        
        private:
        typename call_traits<U>::reference 
        ref_urng(){
            // useful in case a pointer
            return this->urng_;
        }
        U urng_;
        
    };

    template<typename U,typename D>
    inner_sampler<U&,D>
    make_inner_sampler(U& urng,const D& random_dist){
        return inner_sampler<U&,D>(urng,random_dist);
    }


}// joint
}// distribution
}// detail
}// statistics
}// boost


#endif
    