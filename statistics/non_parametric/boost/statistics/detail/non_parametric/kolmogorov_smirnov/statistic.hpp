//////////////////////////////////////////////////////////////////////////////
// non_parametric::kolmogorov_smirnov::statistics.hpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_STATISTIC_HPP_ER_2009
#include <string>
#include <boost/format.hpp>
#ifdef BOOST_MSVC
 #include <algorithm>
#endif
#ifndef BOOST_MSVC
 #include <ext/algorithm>
#endif
#include <boost/range.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace non_parametric{
namespace kolmogorov_smirnov{

    // This statistics has to be a applied to a range each elements of which is
    // of type fusion::map and whose (x,cdf(x)) components are identified by 
    // key Kx and Kcdf, respectively
    //
    // Warning: ignores combos (don't use it for discrete distributions)
    template<typename T,typename Kx,typename Kcdf>
    class statistic{
        typedef Kx      key1_;
        typedef Kcdf    key2_;
        typedef std::string str_;
        public:
    
        typedef std::size_t size_type;

        typedef T result_type;

        // Side effect: sorts the range by x
        template<typename It>
        result_type
        operator()(It b,It e)const{
            typedef T val_;
            std::sort(
                b,
                e,
                less_than()
            );
            
            val_ m1 = static_cast<val_>(0);
            size_type i = 1;
            this->n_ = static_cast<size_type>(
                std::distance(b,e)
            );

            while(b<e){
                val_ ecdf = static_cast<val_>(i);
                ecdf /= static_cast<val_>(this->count());
                val_ cdf = boost::fusion::at_key<key2_>(*b);
                val_ m2 = (cdf > ecdf)?(cdf - ecdf) : (ecdf - cdf);
                if(m2 > m1){ m1 = m2; } 
                ++b;
                ++i;
            }
            this->s_ = m1;
            return m1;
        }

        const size_type& count()const{ return this->n_; }
        const result_type value()const{ return this->s_; }
        
        static const str_ description_header;
        str_ description()const
        {
             return (
                boost::format(
                    "(%1%,%2%)"
                )%this->count()%this->value()
            ).str();
        }

        private:
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
        mutable result_type s_;
        mutable size_type n_;
    };
    template<typename T,typename Kx,typename Kcdf>
    const std::string 
    statistic<T,Kx,Kcdf>::description_header = "kolmogorov_smirnov(n,s)";

}// kolmogorov_smirnov
}// non_parametric
}// detail
}// statistics
}// boost


#endif