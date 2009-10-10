//////////////////////////////////////////////////////////////////////////////
// distribution::joint::distributions::iid::os.hpp                          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_JOINT_DISTRIBUTIONS_IID_RANDOM_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <boost/range.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/random.hpp>
#include <boost/statistics/detail/distribution_toolkit/random/generator.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace joint{

    template<typename D>
    class random_iid : joint::iid<D>{
        typedef joint::iid<D> iid_;
        typedef typename common::meta::value<iid_>::type value_;
        typedef std::vector<value_> cache_type;
        typedef typename toolkit::meta::random<D>::type rand_;
        
        public:
        typedef typename rand_::input_type input_type;
        typedef sub_range<const cache_type> result_type;
    
        random_iid(){}
        random_iid(iid_ jd) // can pass D (implicit conversion)
            :iid_(jd),cache_(this->dimension()){}
        random_iid(const random_iid& that)
            :iid_(static_cast<const iid_&>(that)),cache_(that.cache_){}
        random_iid& operator=(const random_iid& that)
        {
            if(&that!=this)
            {
                static_cast<iid_&>(*this) = static_cast<const iid_&>(that);
                this->cache_ = that.cache_;
            }
            return *this;
        }
        
        typename iid_::size_type
        dimension()const{
            return static_cast<const iid_&>(*this).dimension();
        }

        template<typename U>
        result_type operator()(U& urng)
        {
            std::generate_n(
                boost::begin(this->cache_),
                this->dimension(),
                make_random_generator(
                    urng,
                    static_cast<const iid_&>(*this).element()
                )
            );

            return result_type(this->cache_);
        
        }
    
        protected:
        cache_type cache_;
    
    };

}// joint
}// distribution
}// detail
}// statistics

    template<typename D>
    statistics::detail::distribution::joint::random_iid<D>
    make_random(const statistics::detail::distribution::joint::iid<D>& jd){
        typedef statistics::detail::distribution::joint::random_iid<D> r_;
        return r_(jd);
    }
    
}// boost

#endif