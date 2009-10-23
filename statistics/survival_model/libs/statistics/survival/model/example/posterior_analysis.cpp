///////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::example::model::posterior_analysis.cpp       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <limits>
#include <algorithm>
#include <iterator>
// #include <boost/archive/binary_oarchive.hpp>
// #include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/timer.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/range_cycle.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp> 

#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/standard_distribution/distributions/uniform.hpp>
#include <boost/dist_random/include.hpp>
#include <boost/statistics/empirical_cdf/algorithm/sequential_kolmogorov_smirnov_distance.hpp>
#include <boost/statistics/survival/data/include.hpp>
#include <boost/statistics/survival/model/models/exponential/include.hpp>

// Must come after survival/model/models/exponential/include
#include <boost/statistics/model/include.hpp>

#include <libs/statistics/survival/model/example/posterior_analysis.h>

void example_posterior_analysis(
    // TODO n_bath should be deduced from input (getline)
    const long& n_batch,            // = 5e4; 
    const long& n_batch_mod,        // = 1e3;
    const long& n_proposal,         // = 1e4; 
    const long& n_t_pars,           // = 1e4;
    const double& entry_bound,      // = const_::inf_;
    const std::string& in_path,     // = "/Users/erwann/projets/2009/Xcode/survival/build/Release/    
    const std::string& out_path,    // = "/Users/erwann/projets/2009/Xcode/survival/build/Release/
    std::ostream& out
){

    out << "-> example_posterior_analysis : ";
    out.flush();
    
    // Steps shown in this example:
    //
    // Samples proposal draws
    // Successively loads batches of records, and for each
    // Creates events at a given time
    // Importance samples from the posterior density
    // Saves the Cook-Gelman (cg) statistics for each.
    // Checks the cg data against U[0,1] using the Kolmogorov Smirnov distance
    
    using namespace boost;
    using namespace statistics;
    namespace surv = survival;
    namespace e_cdf = empirical_cdf;
    namespace iw = importance_weights;

    // Types
    typedef std::string                             str_;
    typedef double                                  val_;
    typedef std::vector<val_>                       vals_;

    // [ Input ]
    typedef boost::archive::text_iarchive           ia_;
    typedef std::ifstream                           ifs_;

    const str_ prior_path       = in_path + "prior";
    const str_ xm_mngr_path     = in_path + "covariates_model_mngr";
    const str_ pr_mngrs_path    = in_path + "par_records_mngrs_path";

    // [ Model ]
    typedef val_ x_;
    typedef val_ par_;
    typedef surv::model::exponential::model<val_>               model_;
    typedef model::model_covariate_parameter_<model_,x_,par_>   mcp_;
    typedef surv::data::default_covariates_model_mngr<
        x_,
        model_
    > xm_mngr_;
    xm_mngr_ xm_mngr;
    {
        ifs_ ifs(xm_mngr_path.c_str());
        ia_ ia(ifs);
        ia >> xm_mngr;
    }
    typedef val_ par_;
    typedef surv::data::default_parameter_records_mngr<
        val_,
        par_
    > pr_mngr_;
    pr_mngr_ pr_mngr;
    ifs_ ifs_pr_mngrs( pr_mngrs_path.c_str() );
    if(!ifs_pr_mngrs.good()){
        str_ str = "error opening : ";
        str.append( pr_mngrs_path );
        throw std::runtime_error(str);
    }
    ia_ ia_pr_mngrs( ifs_pr_mngrs );

    typedef math::normal_distribution<val_> mprior_;
    mprior_ mprior;
    {
        typedef math::meta_distribution_primitives<mprior_>::type  prim_;
        ifs_ ifs(prior_path.c_str());
        ia_ ia(ifs);
        prim_ prim;
        ia >> prim;
        mprior = prim;
    }

    // [ Output ]
    typedef boost::archive::text_oarchive           oa_;
    typedef std::ofstream                           ofs_;
    const str_ t_pars_path = out_path + "t_pars";
    const str_ cg_path     = out_path + "cg";
    const str_ ks_path     = out_path + "kss";

    // Cg
    ofs_ ofs_cg( cg_path.c_str() ); 
    vals_ cgs; cgs.reserve(n_batch);

    // Targets
    typedef std::vector<par_> pars_;
    pars_ t_pars( n_t_pars );
    ofs_ ofs_t_pars(t_pars_path.c_str());
    oa_ oa_t_pars(ofs_t_pars);
    
    // Events
    typedef surv::data::responses_mngr<val_>        y_mngr_;
    y_mngr_ y_mngr;

    // Urng
    typedef boost::mt19937                          urng_;
    urng_                                           urng;

    // Proposal
    typedef mprior_                                 mproposal_;
    // This one size fits all proposal is likely to result in a small ESS. You
    // can monitor the ESS by calling out << pws. Ultimately, uniformity of
    // the Cook-Gelman will determine if either the computations are wrong
    // or if n_proposal is too small. n_proposal = 1e4 works fine for N(0,5).
    mproposal_ mproposal = mprior; 
    pars_ p_pars(n_proposal); 
    vals_ p_lpdfs(n_proposal);

    // Weights
    const val_ max_log = 100.0;
    typedef model::importance_sampler<val_> model_is_;
    model_is_ model_is( max_log );

    vals_ iws; 
    iws.reserve( n_proposal );
        
    // Model
    typedef surv::model::exponential::model<val_> model_;
    typedef model::prior_model_dataset_<
        mprior_,
        xm_mngr_::model_wrapper_type::model_type,
        xm_mngr_::covariates_type,
        y_mngr_::responses_type
    > pmd_; 

    {   // [ Proposal sample ]
        const long n_p_pars_kss = -1;   //n_batch; 
        boost::timer t;

        generate_function_n<math::fun_wrap::log_unnormalized_pdf_>(
            boost::begin( p_pars ), 
            boost::begin( p_lpdfs ), 
            n_proposal,
            mproposal, 
            urng
        );

        BOOST_ASSERT( size(p_pars) == size(p_lpdfs) );
    }

    // Loops over batches
    for(unsigned i = 0; i<n_batch; i++){
        ia_pr_mngrs >> pr_mngr;
        y_mngr.clear_responses();
        y_mngr.update_responses(
            boost::begin( pr_mngr.records() ),
            boost::end( pr_mngr.records() ),
            entry_bound
        );
        xm_mngr.update_covariates(
            boost::size(
                y_mngr.responses()
            )
        );
        
        {   // [ Posterior sample ]
            boost::timer t;
            pmd_ pmd( 
                mprior, 
                xm_mngr.model_wrapper().model(), 
                xm_mngr.covariates(), 
                y_mngr.responses()
            );
            iws.resize( boost::size( p_pars ) );

            t_pars.clear();
            t_pars.resize( n_t_pars );
            model_is.prepare_weights(
                pmd,
                boost::begin( p_pars ),       
                boost::end( p_pars ),    
                boost::begin( p_lpdfs )
            );            
            model_is.generate_n(
                boost::begin(t_pars),
                n_t_pars,
                boost::begin( p_pars ),
                urng
            );

            oa_t_pars << t_pars;
            val_ plt = e_cdf::proportion_less_than(
                boost::begin( t_pars ),
                boost::end( t_pars ),
                pr_mngr.parameter_wrapper().parameter()
            );
            cgs.push_back( plt );
            ofs_cg << plt << ' ';
            ofs_cg.flush();
            const char* str = "i = %1%, t = %2%, pws = %3%";
            if(i%n_batch_mod == 0){
                format f(str);
                f 
                % i  
                % t.elapsed() 
                % model_is.pws(); 
                out << std::endl <<  f.str();  
            }
        }// Proposal sample
    }// loop over batches

    {   // Kolmorov Smirnov of the cgs
        typedef math::uniform_distribution<val_> munif_;
        long n_ks = n_batch;
        BOOST_ASSERT( n_batch % n_ks == 0 );
        vals_ kss;
        kss.reserve( n_batch );
        e_cdf::sequential_kolmogorov_smirnov_distance(
            munif_(),
            boost::begin( cgs ),
            boost::end( cgs ),
            n_ks,
            std::back_inserter(kss)
        );
        ofs_ ofs_ks(ks_path.c_str());
        std::copy(
            boost::begin( kss ),
            boost::end( kss ),
            std::ostream_iterator<val_>(ofs_ks," ")
        );
    }

    out << "<-" << std::endl;
}

