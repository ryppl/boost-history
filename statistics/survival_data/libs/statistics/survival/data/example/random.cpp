//#include <cmath>
#include <stdexcept>
#include <fstream>
#include <ostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <algorithm>
#include <ext/algorithm> // is_sorted
#include <iterator>
#include <functional>

#include <boost/typeof/typeof.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/timer.hpp>
#include <boost/range.hpp>
#include <boost/utility.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/math/tools/precision.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>         //prior
#include <boost/standard_distribution/distributions/exponential.hpp>    //data

#include <boost/statistics/empirical_cdf/algorithm/sequential_kolmogorov_smirnov_distance.hpp>

#include <boost/dist_random/include.hpp>

#include <boost/statistics/model/include.hpp>
#include <boost/statistics/survival/data/include.hpp>
#include <boost/statistics/survival/model/models/exponential/include.hpp>

#include <libs/statistics/survival/data/example/random.h>

void example_random(
    const unsigned&      n_records,          // = 1e2;
    const unsigned&      n_batches,           // = 5e4;
    const double&        mu,                // = 0.0;
    const double&        sigma,             // = 5.0;
    const double&        t,                 // = 0.0
    const double&        delta_t,           // = 0.0
    const std::string&   out_path,
    std::ostream& out
){
    out << "-> example_random : "; 
    out.flush();
    
    using namespace boost;
    using namespace statistics;
    namespace surv = survival;

    // Generates batches of iid random records as follows:
    // 1) The parameter is generated, for each batch, given a prior.
    // 2) The covariates are generated in cyclic fashion within each batch
    // 3) Data is generated given model, parameter, covariate, entry time.
    // KS distances are computed for each batch of data
    // Each batch is saved using serialization

    // Types 
    typedef std::string                                     str_;
    typedef double                                          val_;
    typedef std::vector<val_>                               vals_;
    typedef boost::mt19937                                  urng_;

    // Covariates values
    typedef val_                                            x_;

    // Model
    typedef math::normal_distribution<val_>                 mprior_;
    typedef val_                                            par_;
    typedef surv::model::exponential::model<val_>           model_;
    typedef model::model_covariate_parameter_<model_,x_,par_> mcp_;

    // Output
    typedef boost::archive::text_oarchive                   oa_;
    typedef std::ofstream                                   ofs_;

    // [Constants]
    const unsigned      k                   = 2;        // # number x values
    const unsigned      n_ks_data           = n_records/k;

    BOOST_ASSERT( n_records % k == 0 );
    BOOST_ASSERT( n_ks_data % (n_records/k) == 0 );
    
    // [ os ]
    const str_ prior_path      = out_path + "prior";
    const str_ ks_path         = out_path + "ks_data";
    const str_ xpm_mngr_path   = out_path + "covariates_prior_model_mngr";
    const str_ pr_mngrs_path   = out_path + "par_records_mngrs_path";
    
    // [ covariates - model ]
     typedef surv::data::default_covariates_model_mngr<x_,model_>  
        xm_mngr_;
     typedef surv::data::default_covariates_prior_model_mngr<x_,mprior_,model_>  
        xpm_mngr_;
    typedef surv::data::default_parameter_records_mngr<val_,par_> 
        pr_mngr_;

    mprior_     mprior( mu, sigma ); 
        
    xm_mngr_ xm_mngr;
    xpm_mngr_ xpm_mngr;
    {
        vals_ x_vals;
        {
            using namespace boost::assign;
            x_vals += -0.5, 0.5;
        }
        BOOST_ASSERT(size(x_vals) == k);
        typedef xpm_mngr_::prior_model_wrapper_type pm_;
        xpm_mngr =  xpm_mngr_(
            boost::begin( x_vals ),
            boost::end( x_vals ),
            pm_(mprior,model_())
        );
    }
    {
        ofs_    ofs(xpm_mngr_path.c_str());
        oa_     oa(ofs);
        oa      << xpm_mngr;
        ofs.flush();
        ofs.close();
    }

    // [ par_records_mngr ]
    pr_mngr_ pr_mngr;
    
    // [ Initialization ]
    urng_       urng;

    // Buffers
    vals_ kss; 
    kss.reserve(n_ks_data);                             // kolmogorov-smirnov 
    vals_ fts; fts.reserve(n_records / k);              // failure times

    {
        ofs_    ofs_pr_mngrs(pr_mngrs_path.c_str());
        surv::data::simulate_batches(
            ofs_pr_mngrs,  
            xpm_mngr, 
            n_batches,
            n_records,
            t,
            delta_t,
            urng
        );
    }

/*
    {
        // Simulate batches of records
        ofs_    ofs_ks(ks_path.c_str());
        ofs_    ofs_pr_mngrs(pr_mngrs_path.c_str());
        oa_     oa_pr_mngrs(ofs_pr_mngrs);
        ofs_ks << "first and last ks of the failure times : " << std::endl;
        for(unsigned i = 0; i<n_batches; i++){
            //records.clear();
            par_ par = boost::sample(mprior,urng);

            pr_mngr.clear_records();
            pr_mngr.set_parameter( par );
            pr_mngr.back_generate(
                n_records,
                xm_mngr,
                t,
                delta_t,
                urng
            );

            {
                oa_pr_mngrs << pr_mngr; 
            }
            
            // Verify that the empirical distribution of failure times agrees
            // with their assumed distribution
            typedef surv::data::meta_failure_distribution<model_>   meta_fd_;
            typedef meta_fd_::type                                  fd_;
            if(n_ks_data>0){   
                ofs_ks << (format("batch %1%, ")%i).str() << std::endl;
                for(unsigned i = 0; i<k; i++){
                    ofs_ks << (format("x[%1%] : ")%i).str();
                    fts.clear();
                    
                    surv::data::failure_times<k>(
                        boost::begin(pr_mngr.records()),
                        boost::end(pr_mngr.records()),
                        i,
                        std::back_inserter(fts)
                    );
                    mcp_ mcp(
                        xm_mngr.model_wrapper(),
                        xm_mngr.x_values()[i],
                        par
                    );
                    fd_ fail_dist = surv::data::make_failure_distribution(mcp);

                    kss.clear();
                    statistics::empirical_cdf
                     ::sequential_kolmogorov_smirnov_distance(
                        fail_dist,
                        boost::begin( fts ),
                        boost::end( fts ),
                        n_ks_data,
                        std::back_inserter( kss )
                    );
 
                    if(n_ks_data>1){
                        // Desired result: kss[0] > kss.back();
                        ofs_ks << kss[0] << ',' << kss.back();
                    }
                    ofs_ks << std::endl;
                    ofs_ks.flush();
                }
            }
        } // batch loop
        ofs_ks.close();
        ofs_pr_mngrs.close();
    } // records generation
*/
    
}