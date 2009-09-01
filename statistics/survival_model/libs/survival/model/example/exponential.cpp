///////////////////////////////////////////////////////////////////////////////
// survival::model::example::model::exponential.cpp                          //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <stdexcept>
#include <string> //needed?
#include <vector>
#include <limits>
#include <ostream>
#include <fstream>
#include <algorithm>
#include <iterator>
// #include <boost/archive/binary_oarchive.hpp>
// #include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/arithmetic/equal.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/range_cycle.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp> 
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/range_cycle.hpp>

#include <boost/standard_distribution/distributions/normal.hpp>

#include <boost/survival/data/include.hpp>
#include <boost/survival/model/models/exponential/include.hpp>
#include <libs/survival/model/example/exponential.h>

// Must come after the model to be used
#include <boost/statistics/model/include.hpp>

#include <libs/survival/model/example/exponential.h>

void example_exponential(std::ostream& out){

    out << "-> example_model_exponential : ";
    
    // Steps shown in this example:
    //
    // Loads the first batch of a set of records
    // Creates events at given time
    // Evaluates the likelihoods and posteriors
    
    using namespace boost;
    using namespace statistics;
    namespace surv = survival;

    // [ Types ]
    // Value
    typedef double                                  val_;
    typedef std::vector<val_>                       vals_;
    typedef surv::constant<val_>                    const_;

    // I/O
    typedef boost::archive::text_oarchive oa_;
    typedef boost::archive::text_iarchive ia_;
    
   // Records
    typedef surv::data::record<val_>                record_;
    typedef std::vector<record_>                    records_;
    typedef range_iterator<records_>::type          it_record_;
    
    // Events
    typedef surv::data::event<val_>                 event_;
    typedef std::vector<event_>                     events_;
    typedef range_iterator<events_>::type           it_event_;

    // Covariates
    typedef val_                                    x_;
    typedef vals_                                   r_x_;
    typedef range_cycle<>                           range_cycle_;
    typedef range_cycle_::apply<r_x_>::type         x_cycle_;

    // Model
    typedef surv::model::exponential::model<val_>   model_;
    typedef val_                                    par_;
    typedef vals_                                   pars_;

    // [ Constants ]
    const val_ entry_bound = const_::inf_;
    const val_ par = 2.0;
    const char* batches_path 
        = "/Users/erwann/projets/2009/Xcode/survival/build/Release/batches";

    // [ Variables ]
    long n_record;

    // [ Upload first batch of records ]
    records_ records;
    {
        std::ifstream ifs(batches_path);
        if(ifs.good()){
            ia_ ia(ifs);
            ia >> records;
        }else{
            std::string str = "error opening : ";
            str.append( batches_path );
            throw std::runtime_error(str);
        }
        ifs.close();
    }
    n_record = boost::size( records );
    
    // [ Events ]
    events_ events; 
    events.reserve( size(records) );
    surv::data::events(
        begin(records),
        end(records),
        entry_bound,
        std::back_inserter(events)
    );
    
    // [ Covariates ] 
    r_x_ r_x;
    {
        using namespace boost::assign;
        r_x += -0.5, 0.5;
    }
    x_cycle_ x_cycle = range_cycle_::make(r_x,0,n_record);
    out << "size(x_cycle) = " << size(x_cycle) << std::endl;
    BOOST_ASSERT( size(x_cycle)>=size(events) );
    // Resize x_cycle to a size that matches that of events
    x_cycle.advance_end( - (size(x_cycle) - size(events)) );
    BOOST_ASSERT( size(x_cycle) == size(events) );

    // Model
    model_ model;

    // Pars
    pars_ pars;
    {
        using namespace assign;
        pars += -2.0, -1.0, 0.0, 1.0, 2.0; 
    }

    // [ Likelihood ]

    typedef math::normal_distribution<val_> mprior_;
    mprior_ mprior;

    out << '(';
    out << model::log_likelihood<val_>(
        model::make_model_data(
            model,
            r_x[0],
            events[0]
        ),
        par
    );
    out << ',';
    out << model::log_likelihood<val_>(
        model::make_model_data(
            model,
            r_x[1],
            events[1]
        ),
        par
    ) << ')';


    // [ Likelihoods ]
    vals_ lls;
    model::log_likelihoods<val_>(
        model::make_model_dataset(model,r_x,events),
        boost::begin(pars),
        boost::end(pars),
        std::back_inserter(lls)
    );

    // [ Prior ]
    vals_ lprs;
    math::transform<math::fun_wrap::log_unnormalized_pdf_>(
        mprior,
        boost::begin(pars),
        boost::end(pars),
        std::back_inserter(lprs)
    );

    // [ Posteriors ]

    vals_ lpos;
    model::log_posteriors<val_>(
        model::make_prior_model_dataset(mprior,model,r_x,events),
        boost::begin(pars),
        boost::end(pars),
        std::back_inserter(lpos)
    );

    // Consistency check
    typedef range_iterator<vals_>::type it_val_;
    {

        it_val_ i_lpr = boost::begin(lprs);
        it_val_ i_lpo = boost::begin(lpos);
        out << std::endl;
        out << "log(prior,likelihood,posterior)" << std::endl;
        for(
            it_val_ i_ll = boost::begin(lls); 
            i_ll< boost::end(lls); 
            i_ll++,i_lpr++,i_lpo++
        ){
            out << '(';
            val_ lpr = *i_lpr;  out << lpr << ',';
            val_ ll = *i_ll;    out << ll << ',';
            val_ lpo = *i_lpo;  out << lpo << ')' << std::endl;
            val_ lpo2 = lpr + ll;
            BOOST_ASSERT(
                arithmetic_tools::equal(
                    lpo,
                    lpo2
                )
            );
        }
    }
    
    // Consistency check2

    {
        vals_ lpr2s ( size(pars) );
        model::log_posteriors<val_>(
            model::make_prior_model_dataset(mprior,model,r_x,events),
            boost::begin(pars),
            boost::end(pars),
            boost::begin(lls), //subtracted
            boost::begin(lpr2s)
        );

        it_val_ i_lpr = boost::begin(lprs);
        out << std::endl;
        out << "log(prior,prior2)" << std::endl;
        for(
            it_val_ i_lpr2 = boost::begin(lpr2s); 
            i_lpr2< boost::end(lpr2s); 
            i_lpr2++,i_lpr++
        ){
            out << '(';
            val_ lpr  = *i_lpr;     out << lpr << ',';
            val_ lpr2 = *i_lpr2;    out << lpr2 << ')' << std::endl;
            BOOST_ASSERT(
                arithmetic_tools::equal(
                    lpr,
                    lpr2
                )
            );
        }
    }

    out << "<-" << std::endl;
}
