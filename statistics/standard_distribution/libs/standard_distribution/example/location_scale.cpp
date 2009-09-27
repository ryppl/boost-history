//////////////////////////////////////////////////////////////////////////////
// standard_distribution::example::location_scale.cpp                       //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cmath>
#include <string>
#include <boost/mpl/apply.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/scalar_dist/map_pdf/product_pdf.hpp>
#include <boost/scalar_dist/map_pdf/inverse_pdf.hpp>
#include <boost/scalar_dist/map_pdf/ratio_pdf.hpp>

#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/standard_distribution/distributions/students_t.hpp>
#include <boost/standard_distribution/transformation/location_scale.hpp>
#include <boost/scalar_dist/algorithm/transform.hpp>
#include <boost/scalar_dist/iterator/distribution_function.hpp>
#include <boost/scalar_dist/meta/include.hpp>
#include <boost/scalar_dist/fun_wrap/include.hpp>

void example_location_scale(std::ostream& out){
        out << "-> example_math_location_scale->" << std::endl;

        // Examples of location_scale + fun_wrap + transformation + algorithms

        using namespace boost;
        using namespace math;

        // Types
        typedef double                                          val_;
        typedef std::vector<val_>                               vals_;
        typedef std::string                                     str_;
        typedef boost::mt19937                                  urng_;
        typedef boost::normal_distribution<val_>                rnd_;
        typedef math::students_t_distribution<val_>             mstud_;
        typedef math::location_scale_distribution<mstud_>       mls_stud_;
        typedef boost::variate_generator<urng_&,rnd_>           vg_;

        // Constants
        const unsigned df   = 10;
        const val_ mu       = 10;
        const val_ sigma    = 2;
        const unsigned n    = 1e1;
        const val_ x        = 2.132;

        format fl("log_unnormalized_pdf(%2%,%1%) = %3%"); fl%x;
        format fd("deleg(%2%,%1%) = %3%"); fd%x;

        // Initialization
        urng_ urng;
        vals_ range_x;

        // Arbitrary random sample
        std::generate_n(
            back_inserter(range_x),
            n,
            vg_(urng,rnd_(0,1))
        );
        
        mstud_ mstud(df);
        mls_stud_ mls_stud(mstud, mu, sigma);

        {   // fun_wrap
            typedef math::delegate<mstud_>              meta_deleg_;
            typedef meta_deleg_::type                   deleg_;
            deleg_ deleg 
                = meta_deleg_::make<math::fun_wrap::log_unnormalized_pdf_>();
            format ffl = fl; ffl%"mstud"%log_unnormalized_pdf(mstud,x);
            format ffd = fd; ffd%"mstud"%deleg(mstud,x);
            out << ffl.str() << ' ' << ffd.str() << std::endl;
        }
        {   // fun_wrap + location_scale_distribution

            typedef math::delegate<mls_stud_>           meta_deleg_;
            typedef meta_deleg_::type                   deleg_;
            deleg_ deleg 
                = meta_deleg_::make<math::fun_wrap::log_unnormalized_pdf_>();

            format ffl = fl; ffl%"mls_stud"%log_unnormalized_pdf(mls_stud,x);
            format ffd = fd; ffd%"mls_stud"%deleg(mls_stud,x);
            out << ffl.str() << ' ' << ffd.str() << std::endl;
        }
        {   // make_distribution_function_iterator + fun_wrap
            vals_ range_log_pdf;
            
            std::copy(
                math::make_distribution_function_iterator<
                    math::fun_wrap::log_unnormalized_pdf_
                >(mstud,boost::begin(range_x)),
                math::make_distribution_function_iterator<
                    math::fun_wrap::log_unnormalized_pdf_
                >(mstud,boost::end(range_x)),
                std::back_inserter(range_log_pdf)
            );
            
            
        }
        {   // product_pdf + fun_wrap
            typedef math::product_pdf<mstud_,mls_stud_>    prod_dist_;
            typedef math::product_pdf<mstud_&,mls_stud_&>  ref_prod_dist_;

            typedef math::delegate<prod_dist_>          meta_deleg_;
            typedef meta_deleg_::type                   deleg_;
            deleg_ deleg 
                = meta_deleg_::make<math::fun_wrap::log_unnormalized_pdf_>();

            prod_dist_ prod_dist(mstud,mls_stud);
            prod_dist_ prod_dist2(mstud,mls_stud);
            prod_dist = prod_dist2;

            format ffl = fl; ffl%"prod_dist"%log_unnormalized_pdf(prod_dist,x);
            format ffd = fd; ffd%"prod_dist"%deleg(prod_dist,x);
            out << ffl.str() << ' ' << ffd.str() << std::endl;
            ref_prod_dist_ ref_prod_dist(mstud,mls_stud);
            ref_prod_dist_ ref_prod_dist2(mstud,mls_stud);
            // Would cause compile error (as expected):
            // no matching for call to mpl::void_::operator=
            // ref_prod_dist = ref_prod_dist2;
        }
        {   // inverse_pdf + fun_wrap
            typedef math::inverse_pdf<mstud_>               inv_dist_;
            typedef math::delegate<inv_dist_>               meta_deleg_;
            typedef meta_deleg_::type                       deleg_;
            deleg_ deleg 
                = meta_deleg_::make<math::fun_wrap::log_unnormalized_pdf_>();
            inv_dist_ inv_dist(mstud);
            format ffl = fl; ffl%"inv_dist"%log_unnormalized_pdf(inv_dist,x);
            format ffd = fd; ffd%"inv_dist"%deleg(inv_dist,x);
            out << ffl.str() << ' ' << ffd.str() << std::endl;
        }
        {   // ratio_pdf + fun_wrap
            typedef math::meta_ratio_pdf<mstud_,mls_stud_>      mf_;
            typedef mf_::type                                   ratio_dist_;

            ratio_dist_ ratio_dist = mf_::make(mstud,mls_stud);
            typedef math::delegate<ratio_dist_>                 meta_deleg_;
            typedef meta_deleg_::type                           deleg_;
            deleg_ deleg 
                = meta_deleg_::make<math::fun_wrap::log_unnormalized_pdf_>();
            format ffl = fl; 
                ffl%"ratio_dist"%log_unnormalized_pdf(ratio_dist,x);
            format ffd = fd; 
                ffd%"ratio_dist"%deleg(ratio_dist,x);
            out << ffl.str() << ' ' << ffd.str() << std::endl;
        }
    out << "<-" << std::endl;
}
