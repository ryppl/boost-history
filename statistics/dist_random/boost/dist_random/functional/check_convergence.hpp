//////////////////////////////////////////////////////////////////////////////
// dist_random::functional::check_convergence.hpp                           //
//                                                                          //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DIST_RANDOM_FUNCTIONAL_CHECK_CONVERGENCE_HPP_ER_2009
#define BOOST_DIST_RANDOM_FUNCTIONAL_CHECK_CONVERGENCE_HPP_ER_2009
#include <sstream>
#include <ostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <string>
#include <iterator>
#include <boost/range.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/random/ref_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/statistics/empirical_cdf/algorithm/sequential_kolmogorov_smirnov_distance.hpp>

namespace boost{
namespace random{

    // Verifies that the distribution and the random generator agree
    template<typename T>
    class check_convergence{
        typedef std::string str_;

        public:
        typedef T value_type;
        typedef std::vector<T> vals_;
        typedef typename range_size<vals_>::type size_type;
        // Construction
        check_convergence();
    
        // Update
        template<typename R,typename D,typename U>
        void operator()(
            const D&  mdist,
            const R&  rdist,  
            size_type n_draw,
            size_type n_ks,
            U&        urng
        );

        // Access
        const vals_& draws()const;
        const vals_& kolmogorov_smirnovs()const;
        str_ distribution()const;
        size_type increment()const;
        
        private:
        str_ dist_;
        vals_ draws_;
        vals_ kss_;
    };

    template<typename T>
    std::ostream& operator<<(
        std::ostream& out, const check_convergence<T> that);

    // Implementation 

    template<typename T>
    std::ostream& operator<<(
        std::ostream& out, const check_convergence<T> that){
        static const char* msg 
            = "%1% : ks distances by increments of n = %2% : ";
        format f(msg); 
        f % that.distribution() % that.increment();
        out << f.str(); 
        out.flush();
        std::copy(
            boost::begin(that.kolmogorov_smirnovs()),
            boost::end(that.kolmogorov_smirnovs()),
            std::ostream_iterator<T>(out," ")
        );
        return out;
    }

    template<typename T>
    check_convergence<T>::check_convergence(){}

    template<typename T>
    template<typename R,typename D,typename U>
    void check_convergence<T>::operator()(
        const D&                    mdist,
        const R&                    rdist,
        size_type                   n_draw,
        size_type                   n_ks,
        U&                          urng
    ){
        BOOST_ASSERT(n_draw % n_ks == 0);
        typedef boost::variate_generator<U&,R>        vg_;

        dist_.clear();
        std::ostringstream outs;  
        outs << mdist;   
        dist_ = outs.str();      
        
        draws_.resize(n_draw);
        kss_.resize(n_ks);
        vg_ vg(urng,rdist);

        std::generate_n(boost::begin(draws_),n_draw,vg);
        statistics::empirical_cdf::sequential_kolmogorov_smirnov_distance(
            mdist,
            boost::begin(draws()),
            boost::end(draws()),
            n_ks,
            boost::begin(kss_)
        );
    }

    template<typename T>
    const typename check_convergence<T>::vals_&
    check_convergence<T>::draws()const{
        return draws_;
    }

    template<typename T>
    const typename check_convergence<T>::vals_&
    check_convergence<T>::kolmogorov_smirnovs()const{
        return kss_;
    }

    template<typename T>
    typename check_convergence<T>::str_ 
    check_convergence<T>::distribution()const{
        return dist_;
    }

    template<typename T>
    typename check_convergence<T>::size_type
    check_convergence<T>::increment()const{
        return size(this->draws()) / size(this->kolmogorov_smirnovs());
    }

}// random
}// boost

#endif