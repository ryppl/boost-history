//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::data::sample_cdf.hpp                               //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DATA_SAMPLE_CDF_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_DATA_SAMPLE_CDF_HPP_ER_2009
#include <ext/algorithm>
#include <ostream>
#include <vector>
#include <limits>
#include <boost/format.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution_toolkit{

    // This data structure is useful, for example, to compute the kolmogorov
    // smirnov statistic at different iterations during the sampling genera-
    // tion process. 
    template<typename T>
    class sample_cdf{
        typedef mpl::int_<1> key1_;
        typedef mpl::int_<2> key2_;
        
        typedef fusion::pair<key1_,T> pair1_;
        typedef fusion::pair<key2_,T> pair2_;
        
        typedef fusion::map<
            pair1_,
            pair2_
        > map_;

        public:
        typedef T value_type;
        typedef key1_ key_observation;
        typedef key1_ key_cdf;
        
        typedef std::vector<map_> sample_type;
        typedef typename range_size<sample_type>::type size_type;

        sample_cdf(){}

        // Update
        template<typename N>
        void reserve(N n){ this->sample_.reserve(n); }
        void clear(){ this->sample_.clear(); }
        template<typename X,typename T1>
        void operator()(const X& x,const T1& cdf){
            map_ map(
                fusion::make_pair<key1_>(x),
                fusion::make_pair<key2_>(cdf)
            );
            this->sample_.push_back(map);
        }

        //template<typename N,typename D,typename U>
        //void generate(N n, U& urng,const D& dist){
        //    BOOST_AUTO(vg,boost::make_random_generator(urng,dist));
        //    size_type i = 0;
        //    while(i<n){
        //        (*this)(vg(),dist);
        //
        //        cdf(dist,x)
        //
        //        ++i;
        //    };
        //    
        //}

        // Access
        size_type size()const{ return boost::size(this->sample()); }
        const sample_type& sample()const{ return this->sample_; }
        
        // Warning : 
        // 1) not correct if combos in the sample (discrete distribution)
        // 2) sorts the sample by observation value
        value_type kolmogorov_smirnov_statistic()const{
            typedef typename range_iterator<const sample_type>::type it_;
            std::sort(
                boost::begin(this->sample_),
                boost::end(this->sample_),
                less_than()
            );
            
            value_type m1 = static_cast<T>(0);
            size_type i = 1;
            size_type n = boost::size(this->sample());

            it_ b = boost::begin(this->sample());
            it_ e = boost::end(this->sample());
            
            while(b<e){
                T ecdf = static_cast<T>(i)/static_cast<T>(n);
                T cdf = boost::fusion::at_key<key2_>(*b);
                T m2 = (cdf > ecdf)?(cdf - ecdf) : (ecdf - cdf);
                if(m2 > m1){ m1 = m2; } 
                ++b;
                ++i;
            }
          return m1;
        }

        private:
        mutable sample_type sample_; 
        struct less_than{
            less_than(){}
            typedef bool result_type;
            template<typename M>
            bool operator()(const M& m1,const M& m2)const{
                return ( boost::fusion::at_key<key1_>(m1) 
                    < boost::fusion::at_key<key1_>(m2)
                );
            }
        
        };
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os,const sample_cdf<T>& sc){
    
        os << (
            format("(%1%,%2%)")%sc.size()%sc.kolmogorov_smirnov_statistic()
        ).str();
        return os;
    }


}// distribution_toolkit
}// detail
}// statistics
}// boost

#endif