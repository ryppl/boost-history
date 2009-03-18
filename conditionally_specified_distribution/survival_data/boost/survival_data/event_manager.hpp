///////////////////////////////////////////////////////////////////////////////
// event_manager.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_EVENT_MANAGER_ER_2009_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_EVENT_MANAGER_ER_2009_HPP_ER_2009
#include <stdexcept>
#include <boost/call_traits.hpp>

namespace boost{namespace survival_data{
    /// \class event_manager
    /// \brief Manages event creation/retrieval for a data unit
    ///
    /// \details Right censoring
    template<typename RealType>
	class event_manager{
    public:
        typedef RealType                                    value_type;
		explicit event_manager(value_type entry_time_arg);
		event_manager(const event_manager& that);
		event_manager& operator=(const event_manager& that);
		/// \details Once an event is set it cannot be changed
		void set_event(bool is_failure_arg,value_type time_since_entry);
		value_type entry_time()const;
		value_type time_on_study(value_type t)const;
		value_type event_time(value_type t)const;
		bool is_failure(value_type t)const;
		bool is_at_risk(value_type s,value_type t)const;
	private:
		value_type   entry_time_; //on the calendar scaled
		bool 	    is_failure_;
		bool 	    is_event_;
		value_type 	event_time_; //measured from time of entry
	};

    template<typename value_type>
	event_manager<value_type>::event_manager(value_type entry_time_arg)
	:entry_time_(entry_time_arg),is_failure_(false),is_event_(false){}

    template<typename value_type>
	event_manager<value_type>::event_manager(const event_manager& that)
	:entry_time_(that.entry_time_),is_failure_(that.is_failure_),
	is_event_(that.is_event_),event_time_(that.event_time_){}

    template<typename value_type>
	event_manager<value_type>&
	event_manager<value_type>::operator=(const event_manager& that){
		if(&that!=this){
			entry_time_ = that.entry_time_;
			is_failure_ = that.is_failure_;
			is_event_ = that.is_event_;
			event_time_ = that.event_time_;}
		return *this;
	}

    template<typename value_type>
	value_type event_manager<value_type>::entry_time()const{
            return entry_time_;}

    template<typename value_type>
	void event_manager<value_type>::set_event(
        bool is_failure,value_type time_since_entry){
		if(is_event_){
			throw std::runtime_error(
                "Surv::event_manager<value_type>::set_event");
		}else{
			is_failure_ = is_failure;
			event_time_ = time_since_entry;
			is_event_ = true;
		};
	}

    template<typename value_type>
	value_type event_manager<value_type>::time_on_study(value_type t)const{
		value_type x = t-entry_time();
		return (x<0.0)? 0.0 : x;
	}

    template<typename value_type>
	value_type event_manager<value_type>::event_time(value_type t)const{
		value_type tos = time_on_study(t);
		if(is_event_){
			tos = (tos<event_time_)? tos : event_time_;
		}
		return tos;
	}

    template<typename value_type>
	bool event_manager<value_type>::is_failure(value_type t)const{
	    bool is_failure_at_time_t = false;
		if(is_failure_){
		    if(event_time_<time_on_study(t)){
		        is_failure_at_time_t = true;
		    }
		}
		return is_failure_at_time_t;
	}

    template<typename value_type>
	bool event_manager<value_type>::is_at_risk(
        value_type s,value_type t)const{
	    return (s<=event_time(t));}

}}
#endif
