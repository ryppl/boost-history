///////////////////////////////////////////////////////////////////////////////
// ars::example::search_reflection.cpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <boost/format.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/ars/search_reflection.hpp>
#include <boost/ars/constant.hpp>

void example_search_reflection(std::ostream& out){
    std::cout << "-> example_search_reflection" << std::endl;
    using namespace boost;
    namespace st = boost::statistics::detail;

    typedef double value_t;
    typedef st::ars::point<value_t> point_t;
    typedef st::ars::constant<value_t> const_;

    struct local{
        static std::ostream& write(
            std::ostream& o,
            const value_t& x_0,
            const value_t& x_1,
            unsigned n,
            const point_t& p_0,
            const point_t& p_1
        ){
            format f("x_0 = %1%, x_1 = %2%, ");
            f % x_0 % x_1;
            o << f.str();
            o << ", n = " << n << ", p_0 : " << p_0 << ", p_1 : " << p_1;
            return o;
        }
    };

    value_t x_min,x_max, x_0, x_1;
    point_t p_0,p_1;
    unsigned n_max, n = 0;

    {
        typedef math::normal_distribution<value_t> mdist_t;
        typedef const mdist_t& param_t;
        x_min = const_::inf_;
        x_max = const_::inf_;
        n_max = 1e2;
        const value_t mu = 0.0;
        const value_t sigma = 2.0;
        mdist_t mdist(mu,sigma);
        out << (format("N(%1%,%2%)")%mu%sigma);

        {
            x_0 = 100.0;
            x_1 = 100.01;
            n = st::ars::search_reflection_dist(
                x_min,
                x_max,
                mdist,
                x_0,
                x_1,
                p_0,
                p_1,
                n_max
            );
            local::write(out,x_0,x_1,n,p_0,p_1);
            out << std::endl;
        } 
        
        {
            x_0 = -100.01;
            x_1 = -100.00;
            n = st::ars::search_reflection_dist(
                x_min,
                x_max,
                mdist,
                x_0,
                x_1,
                p_0,
                p_1,
                n_max
            );
            local::write(out,x_0,x_1,n,p_0,p_1);
            out << std::endl;
        }

        {
            x_0 = -0.02;
            x_1 = -0.01;
            n = st::ars::search_reflection_dist(
                x_min,
                x_max,
                mdist,
                x_0,
                x_1,
                p_0,
                p_1,
                n_max
            );
            local::write(out,x_0,x_1,n,p_0,p_1);
            out << std::endl;
        }
        
    }
    out << "<-" << std::endl;
}
