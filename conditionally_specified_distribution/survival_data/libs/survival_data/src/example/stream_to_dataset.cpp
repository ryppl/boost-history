///////////////////////////////////////////////////////////////////////////////
//  stream_to_dataset.cpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <stdexcept>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/survival_data/include.hpp>
#include <libs/survival_data/src/example/stream_to_dataset.h>

	void example_survival_data_stream_to_dataset(){
		std::cout
            << "-> example_survival_data_stream_to_dataset()"
            << std::endl;
		using namespace boost::assign;
		using namespace boost::survival_data;
		using namespace std;
		// Warning : change the path as necessary
		const char* filepath =
        //"mice_data_shortened";
  "/Users/erwann/sandbox/conditionally_specified_distribution/survival_data/libs/survival_data/src/example/mice_data.txt";
        //"./mice_data";
        typedef double                                  real_type;
        typedef std::vector<real_type>                  container_type;
        typedef unit<container_type>        unit_obs_type;
		typedef vector<unit_obs_type>				      dataset_type;
		typedef dataset_type::size_type                 dataset_size_type;
        typedef vector<real_type>					    coefficients_type;
        coefficients_type coefficients; coefficients += -1.0, -0.1, 0.1, 1.0 ;
        const dataset_size_type dataset_size = 80;
        const unsigned modulo = 20;
        const real_type max_censoring_time = 40.0;

		dataset_type dataset; dataset.reserve(dataset_size);
		std::ifstream mice_data(filepath);
		try{
            if(!mice_data.good()){
                std::string str = "mice data not open check the filepath:";
                str.append(filepath);
                throw std::runtime_error(str);
            }
            stream_to_dataset::fill(mice_data,dataset);

            for(unsigned int i = 0; i<dataset.size(); i++){//TODO algorithm
                if(i%modulo==0){
                    std::cout << "unit # =" << i << std::endl;
                    unit_obs_type uo = dataset[i];
                    std::cout << "covariate=";
                    copy(
                        boost::begin(uo.covariate()),
                        boost::end(uo.covariate()),
                        std::ostream_iterator<real_type>(std::cout, " ")
                    );
                    std::cout
                        << "is failure="
                        << uo.is_failure(max_censoring_time)
                        << " ";
                    std::cout
                        << "event time="
                        << uo.event_time(max_censoring_time)
                        << std::endl;
                }
            }

        }catch(const std::exception& e){
                std::cout<< e.what() << std::endl;
        }
		std::cout << "<-" << std::endl;
	}

