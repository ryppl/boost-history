/////////////////////////////////////////////////////////////////////////////
// initializer.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SAMPLER_ADAPTIVE_REJECTION_INITIALIZATER_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_SAMPLER_ADAPTIVE_REJECTION_INITIALIZATER_HPP_ER_2009
#include <utility>
#include <boost/assert.hpp>
#include <boost/mpl/void.hpp>
#include <boost/conditionally_specified_distribution/keyword/sampler.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace sampler{

    template<typename RealType,typename Id = mpl::void_>
    class pair_quantile_averaged_over_iterations{
        typedef std::size_t                         size_type;
        typedef RealType                            value_type;
        typedef kwd<Id>                             kwd;
        typedef std::pair<value_type,value_type>    interval_t;
        static interval_t default_interval(){
            static interval_t result
                = interval_t((value_type)(-1),(value_type)(1));
            return result;
        }
        static interval_t default_interval_cdf(){
            static interval_t result
                = interval_t((value_type)(0.2),(value_type)(0.8));
            return result;
        }

        public:
        typedef const interval_t& result_type;

        template<typename Args>
        pair_quantile_averaged_over_iterations(const Args& args)
        :l_(args[kwd::ars_init_interval|default_interval()].first),
        h_(args[kwd::ars_init_interval|default_interval()].second),
        result_(l_,h_),
        n_(1),
        low_cdf_(args[
            kwd::ars_interval_cdf|default_interval_cdf()].first),
        high_cdf_(args[
            kwd::ars_interval_cdf|default_interval_cdf()].second)
        {
            BOOST_ASSERT(high_cdf_>low_cdf_);
        }

        result_type
        operator()()const{
            BOOST_ASSERT(result_.second>result_.first);
            return result_;
        }

        template<typename Impl>
        void update(
            const Impl& impl //impl.inverse_cdf() needed
        ){

            l_ = impl.inverse_cdf(low_cdf_);
            h_ = impl.inverse_cdf(high_cdf_);
            value_type n_val = (value_type)(n_);
            value_type avg_l_ = (*this)().first;
            avg_l_ = (n_val-1) * avg_l_ + l_;
            avg_l_ /= n_val;
            value_type avg_h_ = (*this)().second;
            avg_h_ = (n_val-1) * avg_h_ + h_;
            avg_h_ /= n_val;
            result_.first  = avg_l_;
            result_.second = avg_h_;
            ++n_;
        }
        private:
        pair_quantile_averaged_over_iterations();
        value_type l_;
        value_type h_;
        interval_t result_;
        size_type  n_;
        value_type low_cdf_;
        value_type high_cdf_;
    };

    template<typename RealType,typename Id = mpl::void_>
    class pair_quantile{
        typedef std::size_t                         size_type;
        typedef RealType                            value_type;
        typedef kwd<Id>                             kwd;
        typedef std::pair<value_type,value_type>    interval_t;
        static interval_t default_interval(){
            static interval_t result
                = interval_t((value_type)(-1),(value_type)(1));
            return result;
        }
        static interval_t default_interval_cdf(){
            static interval_t result
                = interval_t((value_type)(0.2),(value_type)(0.8));
            return result;
        }

        public:
        typedef const interval_t& result_type;

        template<typename Args>
        pair_quantile(const Args& args) :
        result_(
            (args[kwd::ars_init_interval|default_interval()].first),
            (args[kwd::ars_init_interval|default_interval()].second)
        ),
        low_cdf_(args[
            kwd::ars_interval_cdf|default_interval_cdf()].first),
        high_cdf_(args[
            kwd::ars_interval_cdf|default_interval_cdf()].second)
        {
            BOOST_ASSERT(high_cdf_>low_cdf_);
        }

        result_type
        operator()()const{
            BOOST_ASSERT(result_.second>result_.first);
            return result_;
        }

        template<typename Impl>
        void update(
            const Impl& impl //impl.inverse_cdf() needed
        ){

            value_type l_ = impl.inverse_cdf(low_cdf_);
            value_type h_ = impl.inverse_cdf(high_cdf_);
            result_.first  = l_;
            result_.second = h_;
        }
        private:
        pair_quantile();
        value_type l_;
        value_type h_;
        interval_t result_;
        value_type low_cdf_;
        value_type high_cdf_;
    };


}//sampler
}//distribution
}//boost




#endif
