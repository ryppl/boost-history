//////////////////////////////////////////////////////////////////////////////
// distribution_toolkit::example::location_scale.cpp                        //
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
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/range.hpp>

#include <boost/math/tools/precision.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/statistics/detail/distribution_toolkit/meta/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/students_t/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/location_scale/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/wrapper/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/iterator/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/include.hpp>

#include <libs/statistics/detail/distribution_toolkit/example/distribution_function.h>

void example_distribution_function(std::ostream& out){
        out << "-> example_math_location_scale->" << std::endl;

        // Examples of location_scale + fun_wrap + map_pdf + iterator

        using namespace boost;
        using namespace math;
        using namespace statistics::detail;
        namespace tk = distribution_toolkit;
        namespace tk_it = tk::iterator;

        // Types
        typedef double                                          val_;
        typedef std::vector<val_>                               vals_;
        typedef std::string                                     str_;
        typedef boost::mt19937                                  urng_;
        typedef boost::normal_distribution<val_>                rnd_;
        typedef math::students_t_distribution<val_>             stud_;
        typedef tk::location_scale_distribution<stud_>          ls_stud_;
        typedef boost::variate_generator<urng_&,rnd_>           vg_;

        // Constants
        const unsigned df   = 10;
        const val_ mu       = 10;
        const val_ sigma    = 2;
        const unsigned n    = 1e1;
        const val_ x        = 2.132;


        // Initialization
        urng_ urng;
        vals_ range_x;
        
        stud_ stud(df);
        ls_stud_ ls_stud(mu, sigma, stud);

        struct float_{
            
            static bool equal(const val_& a, const val_& b){
                static val_ e = boost::math::tools::epsilon<val_>();;
                return fabs(a-b)< e;
            }
        
        };


        {   // fun_wrap
            typedef tk::meta::delegate<stud_>               meta_deleg_;
            typedef meta_deleg_::type                       deleg_;
            deleg_ deleg 
                = meta_deleg_::make<tk::fun_wrap::log_unnormalized_pdf_>();
            BOOST_ASSERT(
                float_::equal(
                    deleg(stud,x),
                    tk::log_unnormalized_pdf(stud,x)
                )
            );

            // test wrapper
            // BUG
            {
                typedef tk::wrapper<const stud_&>           cref_stud_;
                cref_stud_ cref_stud(stud);
            
                typedef tk::meta::delegate<cref_stud_>      meta_deleg2_;
                typedef meta_deleg2_::type                  deleg2_;
                
                // TODO compile error here:
                //deleg2_ deleg2 
                //    = meta_deleg2_::make<tk::fun_wrap::log_unnormalized_pdf_>();

                BOOST_ASSERT(
                    float_::equal(
                        tk::log_unnormalized_pdf(cref_stud,x),
                        tk::log_unnormalized_pdf(stud,x)
                    )
                );
            }
        }
        {   // fun_wrap + location_scale_distribution

            typedef tk::meta::delegate<ls_stud_>               meta_deleg_;
            typedef meta_deleg_::type                           deleg_;
            deleg_ deleg 
                = meta_deleg_::make<tk::fun_wrap::log_unnormalized_pdf_>();
            BOOST_ASSERT(
                float_::equal(
                    deleg(ls_stud,x),
                    tk::log_unnormalized_pdf(ls_stud,x)
                )
            );
        }
        {   // make_distribution_function_iterator + fun_wrap
        // Arbitrary random sample
            std::generate_n(
                back_inserter(range_x),
                n,
                vg_(urng,rnd_(0,1))
            );
            vals_ range_log_pdf;
            std::copy(
                tk_it::make_distribution_function<
                    tk::fun_wrap::log_unnormalized_pdf_
                >(stud,boost::begin(range_x)),
                tk_it::make_distribution_function<
                    tk::fun_wrap::log_unnormalized_pdf_
                >(stud,boost::end(range_x)),
                std::back_inserter(range_log_pdf)
            );
        }
        {   // product_pdf + fun_wrap
            typedef tk::product_pdf<stud_,ls_stud_>     prod_dist_;

            typedef tk::meta::delegate<prod_dist_>      meta_deleg_;
            typedef meta_deleg_::type                   deleg_;
            deleg_ deleg 
                = meta_deleg_::make<tk::fun_wrap::log_unnormalized_pdf_>();

            prod_dist_ prod_dist(stud,ls_stud);
            prod_dist_ prod_dist2(stud,ls_stud);
            prod_dist = prod_dist2; // check assignment

            BOOST_ASSERT(
                float_::equal(
                    deleg(prod_dist,x),
                    tk::log_unnormalized_pdf(prod_dist,x)
                )
            );

        }
        {   // inverse_pdf + fun_wrap
            typedef tk::inverse_pdf<stud_>                   inv_dist_;
            typedef tk::meta::delegate<inv_dist_>            meta_deleg_;
            typedef meta_deleg_::type                        deleg_;
            deleg_ deleg 
                = meta_deleg_::make<tk::fun_wrap::log_unnormalized_pdf_>();
            inv_dist_ inv_dist(stud);

            BOOST_ASSERT(
                float_::equal(
                    deleg(inv_dist,x),
                    tk::log_unnormalized_pdf(inv_dist,x)
                )
            );

        }
        {   // ratio_pdf + fun_wrap
            typedef tk::meta_ratio_pdf<stud_,ls_stud_>        mf_;
            typedef mf_::type                                   ratio_dist_;

            ratio_dist_ ratio_dist = mf_::make(stud,ls_stud);
            typedef tk::meta::delegate<ratio_dist_>                 meta_deleg_;
            typedef meta_deleg_::type                           deleg_;
            deleg_ deleg 
                = meta_deleg_::make<tk::fun_wrap::log_unnormalized_pdf_>();
                
            BOOST_ASSERT(
                float_::equal(
                    deleg(ratio_dist,x),
                    tk::log_unnormalized_pdf(ratio_dist,x)
                )
            );
        }
    out << "<-" << std::endl;
}
