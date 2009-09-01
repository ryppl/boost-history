///////////////////////////////////////////////////////////////////////////////
// survival::model::example::model::posterior_analysis.cpp                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <ostream>
#include <fstream>
#include <stdexcept>
#include <string> //needed?
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
#include <boost/math/distributions/uniform.hpp>
#include <boost/statistics/empirical_cdf/algorithm/sequential_kolmogorov_smirnov_distance.hpp>
#include <boost/survival/data/include.hpp>
#include <boost/survival/model/models/exponential/include.hpp>

// Must come after the model to be used
#include <boost/statistics/model/include.hpp>

#include <boost/importance_weights/algorithm/prepare_weights.hpp>
#include <boost/importance_sampling/include.hpp>

#include <libs/survival/model/example/posterior_analysis.h>

void example_posterior_analysis(std::ostream& out){

    out << "-> example_posterior_analysis : ";
    out.flush();
    
    // Steps shown in this example:
    //
    // Samples proposal draws
    // Successively loads batches of records, and for each
    // Creates events at a given time
    // Importance samples from the posterior as the target density
    // Saves the Cook-Gelman (cg) statistics for each.
    // Checks the cg data against U[0,1] using Kolmogorov Smirnov
    
    using namespace boost;
    using namespace statistics;
    namespace surv = survival;
    namespace iw = importance_weights;

    // Types
    typedef std::string                             str_;
    typedef double                                  val_;
    typedef std::vector<val_>                       vals_;
    typedef surv::constant<val_>                    const_;

    // [ Input ]
    typedef boost::archive::text_iarchive           ia_;
    typedef std::ifstream                           ifs_;

    const str_ input_path 
        = "/Users/erwann/projets/2009/Xcode/survival/build/Release/";
    const str_ prior_path   = input_path + "/prior";
    const str_ x_vals_path  = input_path + "/x_vals";
    const str_ pars_path    = input_path + "/pars";
    const str_ batches_path = input_path + "/batches";

    // Covariates
    typedef val_                                    x_;
    typedef vals_                                   x_vals_;
    typedef range_cycle<>                           range_cycle_;
    typedef range_cycle_::apply<x_vals_>::type      x_cycle_;
    x_vals_ x_vals;
    {
        ifs_ ifs(x_vals_path.c_str());
        ia_ ia(ifs);
        ia >> x_vals;
    }
    x_cycle_ x_cycle;

    // Prior
    typedef math::normal_distribution<val_>         mprior_;
    mprior_ mprior;
    {
        typedef math::meta_distribution_primitives<mprior_>::type  prim_;
        ifs_ ifs(prior_path.c_str());
        ia_ ia(ifs);
        prim_ prim;
        ia >> prim;
        mprior = prim;
    }

    // Records
    // TODO n_bath should be deduced from input (getline)
    const long n_batch                              = 1e4; 

    typedef surv::data::record<val_>                record_;
    typedef std::vector<record_>                    records_;
    typedef range_size<records_>::type              size_type;
    typedef range_iterator<records_>::type          it_record_;

    records_ records;
    ifs_ ifs_batches(batches_path.c_str());
    if(!ifs_batches.good()){
        str_ str = "error opening : ";
        str.append( batches_path );
        throw std::runtime_error(str);
    }
    ia_ ia_batches(ifs_batches);

    // True pars
    typedef val_                                    par_;
    typedef vals_                                   pars_;
    const long n_true_pars_kss = -1;//n_batch; 
    pars_ true_pars;
    true_pars.reserve(n_batch);
    {
        ifs_ ifs_pars(pars_path.c_str());
        if(!ifs_pars.good()){
            str_ str = "error opening : ";
            str.append( pars_path );
            throw std::runtime_error(str);
        }
        ia_ ia_pars(ifs_pars);

        for(long i = 0; i< n_batch; i++){
            val_ par;
            ia_pars >> par;
            true_pars.push_back(par);
        }
    }
    if(n_true_pars_kss>0){   // Check that F_n(true_pars) agrees with mprior
        BOOST_ASSERT( n_batch % n_true_pars_kss == 0);
        vals_ true_pars_kss;
        true_pars_kss.reserve(n_true_pars_kss);
        statistics::empirical_cdf::sequential_kolmogorov_smirnov_distance(
            mprior,
            boost::begin(true_pars),
            boost::end(true_pars),
            n_true_pars_kss,
            std::back_inserter(true_pars_kss)
        );
        out << "true pars kss : ";
        std::copy(
            boost::begin( true_pars_kss ),
            boost::end( true_pars_kss ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl;
    }

    // [ Output ]
    typedef boost::archive::text_oarchive           oa_;
    typedef std::ofstream                           ofs_;

    const char* t_pars_path = "./t_pars";
    const char* cg_path     = "./cg";
    const char* ks_path     = "./kss";

    // Cg
    ofs_ ofs_cg( cg_path ); 
    vals_ cgs; cgs.reserve(n_batch);

    // Targets
    const long n_t_pars = 1e3;
    pars_ t_pars; 
    t_pars.reserve( n_t_pars );
    ofs_ ofs_t_pars(t_pars_path);
    oa_ oa_t_pars(ofs_t_pars);
    
    // [ Local ]

    // Monitoring
    const long n_batch_mod                          = 1e3;

    // Events
    const val_ entry_bound                          = const_::inf_;
    typedef surv::data::event<val_>                 event_;
    typedef std::vector<event_>                     events_;
    typedef range_iterator<events_>::type           it_event_;

    // Urng
    typedef boost::mt19937                          urng_;
    urng_                                           urng;

    // Unif
    typedef math::uniform_distribution<val_>        munif_;
    
    // Proposal
    typedef mprior_                                 mproposal_;
    pars_ p_pars; 
    const long n_proposal = 1e3; //1e4 recommended but takes longer
    p_pars.reserve(n_proposal);

    // This one size fits all proposal is likely to result in a small effective
    // sample size. Check out<<pws below to determine n_proposal.
    mproposal_  mproposal = mprior; 
    vals_ p_lpdfs; 
    p_lpdfs.reserve(n_proposal); 

    // Weights
    const val_ max_log = 100.0;
    typedef iw::prepare_weights<val_> pws_;
    pws_ pws( max_log );
    vals_ iws; 
    iws.reserve( n_proposal );
        
    // Model
    typedef surv::model::exponential::model<val_> model_;
    model_ model;
    typedef model::prior_model_dataset_<mprior_,model_,x_cycle_,events_> pmd_; 

    {   // [ Proposal sample ]
        const long n_p_pars_kss = -1;//n_batch; 
        boost::timer t;
        p_pars.clear();
        generate_n(
            std::back_inserter(p_pars),
            n_proposal,
            mproposal,
            urng
        );

        if(n_p_pars_kss>0){   // Check that F_n(p_pars) agrees with mproposal
            BOOST_ASSERT( n_batch % n_p_pars_kss == 0);
            vals_ p_pars_kss;
            p_pars_kss.reserve(n_p_pars_kss);
            statistics::empirical_cdf::sequential_kolmogorov_smirnov_distance(
                mproposal,
                boost::begin(p_pars),
                boost::end(p_pars),
                n_p_pars_kss,
                std::back_inserter(p_pars_kss)
            );
            out << "p_pars kss : ";
            std::copy(
                boost::begin( p_pars_kss ),
                boost::end( p_pars_kss ),
                std::ostream_iterator<val_>(out," ")
            );
            out << std::endl;
        }

        p_lpdfs.clear();
        math::transform<math::fun_wrap::log_unnormalized_pdf_>(
            mproposal,
            boost::begin( p_pars ),
            boost::end( p_pars ),
            std::back_inserter( p_lpdfs )
        );
        BOOST_ASSERT( size(p_pars) == size(p_lpdfs) );
    }

    // Loops over batches
    for(unsigned i = 0; i<n_batch; i++){
        records.clear();
        ia_batches >> records;
        events_ events; 
        events.reserve( size(records) );
        surv::data::events(
            begin(records),
            end(records),
            entry_bound,
            std::back_inserter(events)
        );
        size_type n_event = size( events );
        
        x_cycle = range_cycle_::make( x_vals, 0, n_event );
        BOOST_ASSERT( size( x_cycle ) >= n_event );
        x_cycle.advance_end( - ( size( x_cycle ) - n_event ) );
        BOOST_ASSERT( size( x_cycle ) == n_event );

        {   // [ Posterior sample ]
            boost::timer t;
            pmd_ pmd( mprior, model, x_cycle, events);
            iws.resize( size( p_pars ) );

            // [Warning: Bug]
            // The Cook-Gelman statistics come out higher at the ends of the
            // [0,1] range than in the middle
            //
            // model::log_posteriors<val_>(
            //    pmd,
            //    boost::begin( p_pars ),
            //    boost::end( p_pars ),
            //    boost::begin( p_lpdfs ),   
            //    boost::begin( iws )  
            // ); 
            // out << std::endl << "iws:";
            // copy(
            //    boost::begin( iws ),
            //    boost::next( boost::begin( iws ), 10),
            //    std::ostream_iterator<val_>(std::cout, " ")
            // );           
            // For testing purposes only:
            // We expect iws2 == iws. But small differences (precision error?)
            // vals_ iws2 = iws;
            // model::log_likelihoods<val_>(
            //    pmd,
            //    boost::begin( p_pars ),
            //    boost::end( p_pars ),
            //    boost::begin( iws2 )  
            // );            
            // out << std::endl << "iws2:";
            // copy(
            //    boost::begin( iws2 ),
            //    boost::next( boost::begin( iws2 ), 10),
            //    std::ostream_iterator<val_>(std::cout, " ")
            // );           

            // Temporary fix, until bug above is resolved.
            BOOST_ASSERT(
                make_distribution_primitives(mprior) ==
                make_distribution_primitives(mproposal) 
            );
            model::log_likelihoods<val_>(
                pmd,
                boost::begin( p_pars ),
                boost::end( p_pars ),
                boost::begin( iws )  
            );            

            pws(
                boost::begin( iws ),
                boost::end( iws ),
                boost::begin( p_pars )
            );
            t_pars.clear();
            t_pars.resize( n_t_pars );
            is::generate(
                urng,
                boost::begin( iws ),
                boost::end( iws ),
                boost::begin( p_pars ),
                boost::begin( t_pars ),
                n_t_pars
            );
            oa_t_pars << t_pars;
            val_ plt =  statistics::empirical_cdf::proportion_less_than(
                boost::begin( t_pars ),
                boost::end( t_pars ),
               true_pars[i]
            );
            cgs.push_back( plt );
            ofs_cg << plt << ' ';
            ofs_cg.flush();
            const char* str = "i = %1%, t = %2%, pws = %3%";
            if(i%n_batch_mod == 0){
                format f(str);
                f % i  % t.elapsed() % pws; 
                out << std::endl << f.str();  
            }
        }// Proposal sample
    }// loop over batches

    {   // Kolmorov Smirnov of the cgs
        long n_ks = n_batch;
        BOOST_ASSERT( n_batch % n_ks == 0 );
        vals_ kss;
        kss.reserve( n_batch );
        statistics::empirical_cdf::sequential_kolmogorov_smirnov_distance(
            munif_(),
            boost::begin( cgs ),
            boost::end( cgs ),
            n_ks,
            std::back_inserter(kss)
        );
        ofs_ ofs_ks(ks_path);
        std::copy(
            boost::begin( kss ),
            boost::end( kss ),
            std::ostream_iterator<val_>(ofs_ks," ")
        );
    }

    out << "<-" << std::endl;
}

