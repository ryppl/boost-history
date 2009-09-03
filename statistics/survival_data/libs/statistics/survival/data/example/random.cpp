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

#include <boost/matrix_view/algorithm/transform_column.hpp> 
#include <boost/dist_random/include.hpp>

#include <boost/statistics/model/include.hpp>
#include <boost/statistics/survival/data/include.hpp>
#include <boost/statistics/survival/model/models/exponential/include.hpp>


void example_random(std::ostream& out){
    out << "-> example_random : ";
    
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
    typedef double                                          val_;
    typedef std::vector<val_>                               vals_;
    typedef range_iterator<vals_>::type                     iter_val_;
    typedef boost::mt19937                                  urng_;

    // Covariates values
    typedef val_                                            x_;
    typedef vals_                                           x_vals_;
    typedef range_cycle<>                                   range_cycle_;
    typedef range_cycle_::apply<x_vals_>::type              x_cycle_;
    typedef range_size<x_vals_>::type                       size_;

    // Survival data
    typedef surv::data::record<val_>                        record_;
    typedef std::vector<record_>                            records_;
    typedef std::vector<records_>                           records_batches_;

    // Model
    typedef math::normal_distribution<val_>                 mprior_;
    typedef val_                                            par_;
    typedef std::vector<par_>                               pars_;
    typedef surv::model::exponential::model<val_>           model_;
    
    // Batch
    typedef surv::data::random::meta_default_batch<
        val_, model_, par_, x_vals_>                        meta_batch_;

    typedef meta_batch_::clock_type                         clock_;
    typedef meta_batch_::type                               batch_;
    typedef batch_::model_covariate_parameter_              mcp_;
    typedef mcp_::model_parameter_w_                        mp_;
    typedef variate_generator<urng_&,batch_>                vg_r; //record

    // I/O
    typedef boost::archive::text_oarchive                   oa_;
    typedef boost::archive::text_iarchive                   ia_;
    typedef std::ofstream                                   ofs_;

    // Failure time distribution
    typedef surv::data::meta_failure_distribution<model_>   meta_fd_;
    typedef meta_fd_::type                                  fd_;

    // [Constants]
    const unsigned      k                   = 2;        // # number x values
    const unsigned      n_record            = 1e2;
    const unsigned      n_batch             = 5e4;
    const unsigned      n_ks_data           = n_record/k;
    const val_          mu                  = 0.0;
    const val_          sigma               = 5.0;
    const clock_        clock(0.0,0.0);

    BOOST_ASSERT( n_record % k == 0 );
    BOOST_ASSERT( n_ks_data % (n_record/k) == 0 );
    
    // [ os ]
    const char* prior_path      = "./prior";
    const char* x_vals_path     = "./x_vals";
    const char* ks_path         = "./ks_data";
    const char* batches_path    = "./batches";
    const char* pars_path       = "./pars";
    
    // [ Covariate values ]
    vals_ x_vals;
    {
        using namespace boost::assign;
        x_vals += -0.5, 0.5;
    }
    BOOST_ASSERT(size(x_vals) == k);

    {
        ofs_    ofs(x_vals_path);
        oa_     oa(ofs);
        oa << x_vals;
        ofs.flush();
        ofs.close();
    }

    // [ Initialization ]
    model_      model; 
    mprior_     mprior( mu, sigma ); 
    urng_       urng;
    {
        ofs_    ofs(prior_path);
        oa_     oa(ofs);
        BOOST_AUTO(prim,make_distribution_primitives(mprior));
        oa << prim;
        ofs.flush();
        ofs.close();
    }

    // Buffers
    records_ records; records.reserve(n_record);
    pars_ true_pars; true_pars.reserve(n_batch);
    vals_ kss; 
    kss.reserve(n_ks_data);                             // kolmogorov-smirnov 
    vals_ fts; fts.reserve(n_record / k);               // failure times
    vals_ pars; pars.reserve(n_record);
    {
        // Simulate batches of records
        ofs_    ofs_ks(ks_path);
        ofs_    ofs_batches(batches_path);
        ofs_    ofs_pars(pars_path);
        oa_     oa_batches(ofs_batches);
        oa_     oa_pars(ofs_pars);
        ofs_ks << "first and last ks of the failure times : " << std::endl;
        for(unsigned i = 0; i<n_batch; i++){
            records.clear();
            par_ par = boost::sample(mprior,urng);
            oa_pars << par; 
            ofs_pars.flush();
            meta_batch_::rcov_ r = meta_batch_::rcov(
                x_vals,
                0,
                n_record
            );
            batch_ batch = meta_batch_::make(
                model,
                par,
                clock, 
                r
            );
            true_pars.push_back( par );
            vg_r vg_r( urng, batch );

            std::generate_n(
                std::back_inserter( records ),
                n_record,
                vg_r
            );

            BOOST_ASSERT(
                is_sorted(
                    boost::begin( records ),
                    boost::end( records )
                )
            );// clock is supposed to tick forward
            
            oa_batches << records;
            
            if(n_ks_data>0){   
                ofs_ks << (format("batch %1%, ")%i).str() << std::endl;
                for(unsigned i = 0; i<k; i++){
                    ofs_ks << (format("x[%1%] : ")%i).str();
                    fts.clear();
                    matrix_view::transform_column<k>(
                        boost::begin(records),
                        boost::end(records),
                        i,
                        lambda::bind(&record_::failure_time,lambda::_1),
                        std::back_inserter(fts)
                    );
                    mcp_ mcp(
                        batch.model(),
                        x_vals[i],
                        batch.parameter()
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
                        // Desired result: kss[0] < kss.back();
                        ofs_ks << kss[0] << ',' << kss.back();
                    }
                    ofs_ks << std::endl;
                    ofs_ks.flush();
                }
            }
        } // batch loop
        ofs_ks.close();
        ofs_pars.close();
        ofs_batches.close();
    } // records generation
}