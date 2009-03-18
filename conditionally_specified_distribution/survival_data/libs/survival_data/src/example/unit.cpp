/////////////////////////////////////////////////////////////////////////////
//  unit.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/generator/shared_container.hpp>
#include <boost/survival_data/unit.hpp>
#include <boost/survival_data/unit_at_time.hpp>
#include <libs/survival_data/src/example/unit.h>

void example_survival_data_unit(){
    std::cout << "->example_surival_data_unit" << std::endl;
    using namespace boost;
    namespace surv = boost::survival_data;
    typedef double                                    value_type;
    typedef std::vector<value_type>                   cont_t;
    typedef surv::unit<cont_t>                      unit_t;
    typedef surv::unit_at_time<unit_t>              unit_at_time_t;

    cont_t container;
    {
    	using namespace boost::assign;
        container+= -1.0, 0.0, 1.0;
    }

    const value_type entry_time0 = 1.0;
    const value_type event_time = 1.0;
    value_type t = 1.5; //analysis time
    unit_t unit0(entry_time0,container);
    unit0.set_event(true,event_time);

    std::cout << " covariate=";
    copy(
        begin(container),
        end(container),
        std::ostream_iterator<value_type>(std::cout, " ")
    ); std::cout << std::endl;

	std::cout << "unit : " << t <<  std::endl;
	std::cout
        << " entry_time = "
        << unit0.entry_time()
        << std::endl;
	std::cout
        << " time_on_study(" << t << ")="
        << unit0.time_on_study(t)
        << std::endl;
	std::cout
        << " event_time(" << t << ")="
        << unit0.event_time(t) << std::endl;

	std::cout
        << " is_at_risk(0.5," << t << ")="
        << unit0.is_at_risk(0.5,t) << std::endl;
	std::cout
        << " is_at_risk(1.0," << t << ")="
        << unit0.is_at_risk(1.0,t) << std::endl;

    t = 2.5;
	std::cout
        << " event_time(" << t << ")="
        << unit0.event_time(t) << std::endl;
	std::cout
        << " is_failure(" << t << ")="
        << unit0.is_failure(t) << std::endl;

    t = 1.5;
    unit_at_time_t unit_at_time(
        unit0,
        t
    );

	std::cout << "unit_at_time at : " << t <<  std::endl;

	std::cout
        << " entry_time = "
        << unit_at_time.entry_time()
        << std::endl;
	std::cout
        << " time_on_study()="
        << unit_at_time.time_on_study()
        << std::endl;
	std::cout
        << " event_time()="
        << unit_at_time.event_time() << std::endl;

	std::cout
        << " is_at_risk(0.5)="
        << unit_at_time.is_at_risk(0.5) << std::endl;
	std::cout
        << " is_at_risk(1.0)="
        << unit_at_time.is_at_risk(1.0) << std::endl;


    std::cout << "<-" << std::endl;
}

