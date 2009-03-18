///////////////////////////////////////////////////////////////////////////////
// unit_at_time.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_UNIT_OBSERVED_AT_TIME_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_UNIT_OBSERVED_AT_TIME_HPP_ER_2009
#include <vector>
#include <iterator>
#include <string>
#include <boost/call_traits.hpp>
#include <boost/mpl/void.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/survival_data/keyword.hpp>
#include <boost/survival_data/unit.hpp>
namespace boost{namespace survival_data{
    // see home/er/cpp/libs/stat/doc/covariate

	/// \class unit_at_time
	/// \brief Staggered entry, right censored, fixed covariate
	///
    /// Event state (before/after event) is shared across copies.
    /// \param Cont models Back Insertion Sequence
	template<typename Unit>
	class unit_at_time : public Unit
	//TODO ideally only the base class is public, rest is private
    {
        typedef Unit                                            super_t;
        typedef typename super_t::kwd                           kwd;
     public:

        template<typename ArgPack>
		unit_at_time(const ArgPack& args)
		:super_t(args),
		time_analysis_(args[kwd::time_analysis]){}

		unit_at_time(
            const super_t& impl,
            typename super_t::value_type time_analysis
        ):super_t(impl),
        time_analysis_(time_analysis){}

		unit_at_time&
		operator=(const unit_at_time& that){
		    if(&that!=this){
		        super_t::operator=(that);
		        time_analysis_ = that.time_analysis_;
		    }
		    return *this;
        }

        //Beware that the base interface is still here
        //Below overlaods but does not override
		typename super_t::value_type
		time_on_study()const{ return super_t::time_on_study(t()); }
		typename super_t::value_type
		event_time()const{ return super_t::event_time(t()); }
		bool is_failure()const{ return super_t::is_failure(t()); }
		bool is_at_risk(typename super_t::value_type s)const{
		    return super_t::is_at_risk(s,t());
        }
		typename super_t::value_type
        time_analysis()const{ return t(); }

        std::string
        as_string()const{
            return super_t::as_string(t());
        }

		private:
		 typename super_t::value_type t()const{ return time_analysis_; }
         typename super_t::value_type time_analysis_;
	};

}}

#endif
