///////////////////////////////////////////////////////////////////////////////
//  call_event_time.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_CALL_EVENT_TIME_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_CALL_EVENT_TIME_HPP_ER_2009
namespace boost{namespace survival_data{
    template<typename UnitOfObservation>
	class call_event_time{
		public:
			typedef typename UnitOfObservation::value_type     result_type;
            typedef const UnitOfObservation&                   argument_type;
			call_event_time(result_type t_):t(t_){};
			result_type operator()(argument_type d)const{
				return d.event_time(t);
			};
		private:
			result_type t;
	};
}}


#endif
