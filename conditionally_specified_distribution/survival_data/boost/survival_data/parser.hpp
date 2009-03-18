///////////////////////////////////////////////////////////////////////////////
// parser.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_MAKE_UNIT_OF_OBSERVATION_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_MAKE_UNIT_OF_OBSERVATION_HPP_ER_2009
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <functional>
#include <vector>
#include <istream>
#include <string>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/type_traits.hpp>
#include <boost/call_traits.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/survival_data/unit.hpp>
#include <boost/type_traits.hpp>

namespace boost{
namespace survival_data{

	template<typename Observation,typename StringType>
    Observation
    parser(typename call_traits<StringType>::param_type line){
        typedef Observation                             unit_obs_type;
        typedef typename
            unit_obs_type::result_of_covariate tmp;
        typedef typename tmp::type   covariate_result_type;
        typedef typename remove_const<
            covariate_result_type>::type                covariate_type;
        typedef typename unit_obs_type::value_type      value_type;
        typedef std::vector<StringType>                 buffer_type;
        typedef std::vector<value_type>                 local_covariate_type;

        buffer_type buffer;
        local_covariate_type covariate;

		std::istringstream ss(line);
   	  	StringType field; std::getline(ss,field,',');
		value_type entry_time = boost::lexical_cast<value_type>(field);

        buffer.clear();
        unit_obs_type unit_obs(0.0,covariate);
        while(std::getline(ss,field,',')){buffer.push_back(field);}
        if(buffer.size()>2){
            unsigned int K = buffer.size()-2;
            transform(
                buffer.begin(),
                buffer.end(),
                back_inserter(covariate),
                boost::bind(
                    &boost::lexical_cast<
                        value_type,typename buffer_type::value_type>,
                    _1
                )
            );

            bool is_failure = boost::lexical_cast<bool>(buffer[K]);//defective

            value_type event_time =
                boost::lexical_cast<value_type>(buffer[K+1]);
            unit_obs = unit_obs_type(entry_time,covariate);//assignement
            unit_obs.set_event(is_failure,event_time);
        }else{
            std::runtime_error("boost::stat::survival::make_data_unit");
        }
        return unit_obs;
    };

}//survival_data
}//boost
#endif
