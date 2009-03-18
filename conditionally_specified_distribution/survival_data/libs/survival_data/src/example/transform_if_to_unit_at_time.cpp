/////////////////////////////////////////////////////////////////////////////
//  transform_if_to_unit_at_time.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/assert.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/survival_data/unit.hpp>
#include <boost/generator/shared_container.hpp>
#include <boost/survival_data/transform_if_to_unit_at_time.hpp>
#include <libs/survival_data/src/example/transform_if_to_unit_at_time.h>

void example_survival_data_transform_if_to_unit_at_time(){
  std::cout << "-> example_survival_data_transform_if_to_unit_at_time"
    << std::endl;

    using namespace boost;
    using namespace boost::survival_data;
    typedef double                          value_type;
    typedef std::vector<value_type>         covariate_t;
    typedef unit<covariate_t>               unit_t;
    typedef unit_at_time<unit_t>            unit_at_time_t;

    covariate_t cov;
    {
    	using namespace boost::assign;
        cov+= -1.0, 0.0, 1.0;
    }

    const value_type entry_time0 = 2.0;
    const value_type entry_time1 = 1.0;
    const value_type event_time0 = 1.0;
    const value_type event_time1 = 1.0;
    const bool is_fail0 = true;
    const bool is_fail1 = true;
    //value_type max_entry_t
    //    = (entry_time0<entry_time1)? entry_time1 : entry_time0;

    unit_t unit0(entry_time0,cov);
    unit0.set_event(is_fail0,event_time0);
    unit_t unit1(entry_time1,cov);
    unit1.set_event(is_fail1,event_time1);

    typedef std::vector<unit_t>  dataset_type;
    dataset_type dataset;
    dataset.push_back(unit0);
    dataset.push_back(unit1);

    typedef std::vector<unit_at_time_t> dataset_at_time_type;
    value_type time_analysis = (entry_time0+entry_time1)/2.0;
    dataset_at_time_type dataset_at_time;

    transform_if_to_unit_at_time(
        begin(dataset),
        end(dataset),
        back_inserter(dataset_at_time),
        time_analysis
    );

    std::size_t size_diff = size(dataset_at_time) - 1;
    BOOST_ASSERT(size_diff == 0);

    value_type desired_entry_time =
     (entry_time0<entry_time1)? entry_time0 : entry_time1;
    value_type entry_time_diff = desired_entry_time;
    entry_time_diff -= dataset_at_time[0].entry_time();

    BOOST_ASSERT(
        (entry_time_diff > -1e-10) && (entry_time_diff<1e-10)
    );

  std::cout << "OK <-" << std::endl;

}
