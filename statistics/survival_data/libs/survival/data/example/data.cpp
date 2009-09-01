///////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::example::data.cpp                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <ostream>
#include <fstream>
#include <stdexcept>
#include <string> //needed?
#include <algorithm>
#include <iterator>
#include <boost/tuple/tuple.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp> 
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/iterator/range_cycle.hpp>
#include <boost/survival/data/include.hpp>
#include <libs/survival/data/example/data.h>

void example_data(std::ostream& out){
    out << "-> example_data : ";
    
    // Steps shown in this example:
    //
    // Records creation
    // Events creation
    // I/O
    // Statistics
    
    using namespace boost;
<<<<<<< .mine
    using namespace statistics;
    namespace surv = statistics::survival;
=======
    namespace surv = survival;
>>>>>>> .r55939

    // [ Types ]
    typedef unsigned val_; // do not modify
    typedef std::vector<val_> vals_;

    // Records
    typedef surv::data::record<val_> record_;
    typedef std::vector<record_> records_;
    typedef range_iterator<records_>::type it_record_;

    // Events
    typedef surv::data::event<val_> event_;
    typedef std::vector<event_> events_;
    typedef range_iterator<events_>::type it_event_;

    // I/O
    typedef boost::archive::text_oarchive oa_;
    typedef boost::archive::text_iarchive ia_;

    // Covariates
    typedef val_                                    x_;
    typedef vals_                                   r_x_;
    typedef range_cycle<>                           range_cycle_;
    typedef range_cycle_::apply<r_x_>::type         x_cycle_;

    // Statistics
    typedef surv::data::mean_event<val_>            me_;
    typedef std::vector<me_>                        mes_;
    typedef surv::data::mean_events_by_covariate<val_,x_> mes_by_x_;

    // [ Constants ]
    const unsigned n_record = 4; 
    const val_ entry_bound = n_record - 1;
    const val_ fail_time = 2;

    // [ Records ]
    records_ records;
    events_ events;
    for(val_ i = 0; i<n_record; i++){
        val_ entry_t = i;
        record_ record(entry_t,fail_time);
        records.push_back(record);
    }

    out << " records : ";
    for(val_ i = 0; i<n_record; i++){
        out << records[i] << ' ';
    }

    // [ Events ]
    
    surv::data::events(
        begin(records),
        end(records),
        entry_bound,
        std::back_inserter(events)
    );
    
    // Analysis for n_record = 4 and entry_bound = 3;
    //      e       ft      dt     ft<=dt  min(ft,dt)
    //      0       2       3       1       2
    //      1       2       2       1       2
    //      2       2       1       0       1
    //      3       2       0       NA      NA

    BOOST_ASSERT(size(events) == entry_bound);
    out << " events : ";
    BOOST_FOREACH(const event_& e,events){
        out << e << ' ';
    }

    // [ I/O ]
    const char* path = "./serialized_events";
    
    {
        // Save events
        std::ofstream ofs(path);
        oa_ oa(ofs);
        BOOST_FOREACH(const event_& e,events){
            oa << e;
            ofs.flush();
        }
    }
    val_ n = size(events);
    events_ events2;
    {
        // Recover events
        event_ tmp;
        std::ifstream ifs(path);
        if(ifs.good()){
            ia_ ia(ifs);
            val_ j = 0;
            //while(true){ //TODO
            while(j<n){
                ia >> tmp;
                events2.push_back(tmp);
                // TODO 
                //if (ifs.eof()) break; 
                //if (ifs.fail()){
                //    throw std::runtime_error("error reading");
                //}
                ++j;
            }
        }else{
            std::string str = "error opening : ";
            str.append( path );
            throw std::runtime_error(str);
        }
    }
    BOOST_ASSERT(n == size(events2));
    for(val_ i = 0; i<n; i++){
        BOOST_ASSERT(events[i] == events2[i]);
    }

    // [ Statistics ]
    // Assume the records were generated conditional on the following
    // covariates:
    r_x_ r_x;
    {
        using namespace boost::assign;
        r_x += 1, 2;
    }
    x_cycle_ x_cycle = range_cycle_::make(r_x,0,n_record);
    out << "size(x_cycle) = " << size(x_cycle) << std::endl;
    BOOST_ASSERT( size(x_cycle)>=size(events) );
    // Resize x_cycle to a size that matches that of events
    x_cycle.advance_end(
        - (size(x_cycle) - size(events))
    );
    BOOST_ASSERT( size(x_cycle)==size(events) );

    mes_by_x_ mes_by_x(r_x);
    mes_by_x(
        boost::begin(x_cycle),
        boost::end(x_cycle),
        boost::begin(events)
    );

    mes_ mes;
    mes_by_x.tails(std::back_inserter(mes));
    
    // Analysis for n_record = 4 and entry_bound = 3;
    //      x       ft<=dt  min(ft,dt)  
    //      1       1       2
    //      2       1       2
    //      1       0       1
    // -------------------------------
    //      1       1/2=0   3/2 = 1         // Remember, val_ = unsigned,
    //      2       1       2               // not a float
    
    out << "mean_events : ";
    std::copy(
        boost::begin( mes ),
        boost::end( mes ),
        std::ostream_iterator<me_>(out," ")
    );

    vals_ flat_mes;
    surv::data::vectorize_events(
        boost::begin( mes ),
        boost::end( mes ),
        std::back_inserter( flat_mes )
    );

    out << "flattened mean_events : ";
    std::copy(
        boost::begin( flat_mes ),
        boost::end( flat_mes ),
        std::ostream_iterator<val_>(out," ")
    );

//    // Dont try this here because val_ = unsigned, but you get the idea
//    surv::data::logit_log(
//        boost::begin(flat_mes),
//        boost::end(flat_mes),
//        std::back_inserter(flat_mes),
//        0.01,
//        0.01
//    );
    out << "<-" << std::endl;
}


