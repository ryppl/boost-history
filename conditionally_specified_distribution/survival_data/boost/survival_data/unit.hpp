///////////////////////////////////////////////////////////////////////////////
// unit.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_UNIT_OF_OBSERVATION_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_UNIT_OF_OBSERVATION_HPP_ER_2009
#include <vector>
#include <string>
#include <iterator>
#include <boost/mpl/identity.hpp>
#include <boost/call_traits.hpp>
#include <boost/format.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/container_to_string.hpp>
#include <boost/generator/shared_container.hpp>
#include <boost/survival_data/event_manager.hpp>
#include <boost/survival_data/keyword.hpp>
namespace boost{
namespace survival_data{

    // see home/er/cpp/libs/stat/doc/covariate

    template<typename Cont,typename Id>
    struct unit_base{
        typedef Cont                                  cont_t;
        typedef shared_container<Cont>                shared_t;
        typedef typename shared_t::size_type          size_type;
        typedef typename shared_t::value_type         value_type;

        struct identifier : mpl::identity<Id>{};
        struct result_of_covariate
            : mpl::identity<typename shared_t::result_type>
        {};
        // TODO see range_covariate() in derived
        struct result_of_range_covariate
            : result_of_covariate
        {};
    };

	/// \class unit
	/// \brief Staggered entry, right censored, fixed covariate
	///
    /// Event state (before/after event) is shared across copies.
    /// \param Cont models Back Insertion Sequence
	template<typename Cont = std::vector<double,std::allocator<double> >,
        typename Id = mpl::void_>
	class unit : public unit_base<Cont,Id>{
	    typedef unit_base<Cont,Id>                      super_t;
		typedef event_manager<typename super_t::value_type>
                                                      event_manager_type;
		typedef boost::shared_ptr<event_manager_type>
                                                    ptr_event_manager_type;
     protected:
        typedef kwd<Id>                                 kwd;
     public:
            typedef typename super_t::value_type        value_type;
            template<typename ArgPack>
            unit(const ArgPack& args);
            template<class R>
			unit(value_type entry_time,const R& covariate);
			unit(const unit&);
			unit& operator=(const unit&);

			/// \details Once set event cannot be changed
			/// \warning If multiple clients have a copy of this
			/// data unit, make sure the event has already been set.
			void set_event(bool is_failure_,value_type time_since_entry);
			value_type entry_time()const;
			value_type time_on_study(value_type t)const;
			value_type event_time(value_type t)const;
			bool is_failure(value_type t)const;
			bool is_at_risk(value_type s,value_type t)const;

			typename super_t::result_of_covariate::type
			covariate()const{
                return shared_covariate_();
			}
            //TODO as needed by conditionally_specified_distribution
            // redundant with covariate(). Maybe remove covariate()
			typename super_t::result_of_range_covariate::type
			range_covariate()const{
                return this->covariate();
			}
            std::string as_string(value_type t)const;
		 private:
            /// Pointers because a data unit may be part of a large dataset,
            /// that maybe embeded in multiple clients
			ptr_event_manager_type	ptr_event_manager;
			typename super_t::shared_t shared_covariate_;
	};

	template<typename Cont,typename Id>
    template<typename ArgPack>
    unit<Cont,Id>::unit(const ArgPack& args)
    :ptr_event_manager(new event_manager_type(kwd::entry_time)),
	shared_covariate_(args[kwd::covariate])
	{}

	template<typename Cont,typename Id>
        template<class R>
	unit<Cont,Id>::unit(
        value_type entry_time_,
        const R& r
    )
	:ptr_event_manager(new event_manager_type(entry_time_)),
	shared_covariate_(r)
	{}

	template<typename Cont,typename Id>
	unit<Cont,Id>::unit(const unit& that)
	:ptr_event_manager(that.ptr_event_manager),
	shared_covariate_(that.shared_covariate_)
	{}

	template<typename Cont,typename Id>
	unit<Cont,Id>&
	unit<Cont,Id>::operator=(const unit& that){
		if(&that!=this){
            ptr_event_manager = that.ptr_event_manager;
            shared_covariate_ = that.shared_covariate_;
		}
		return *this;
	}

	template<typename Cont,typename Id>
	void unit<Cont,Id>::set_event(
        bool is_failure_,value_type time_since_entry){
		ptr_event_manager->set_event(is_failure_,time_since_entry);
    }

	template<typename Cont,typename Id>
	typename unit<Cont,Id>::value_type
	unit<Cont,Id>::entry_time()const{
	    return ptr_event_manager->entry_time();
    }

	template<typename Cont,typename Id>
	typename unit<Cont,Id>::value_type
	unit<Cont,Id>::time_on_study(value_type t)const{
        return ptr_event_manager->time_on_study(t);
    }

	template<typename Cont,typename Id>
	typename unit<Cont,Id>::value_type
	unit<Cont,Id>::event_time(value_type t)const{
	    return ptr_event_manager->event_time(t);
    }

	template<typename Cont,typename Id>
	bool unit<Cont,Id>::is_failure(value_type t)const{
	    return ptr_event_manager->is_failure(t);
    }

	template<typename Cont,typename Id>
	bool unit<Cont,Id>::is_at_risk(
        value_type s,value_type t
    )const{
	    return ptr_event_manager->is_at_risk(s,t);
    }

	template<typename Cont,typename Id>
    std::string
    unit<Cont,Id>::as_string(value_type t)const{
        typedef utility::container_to_string cont_to_str_t;
        std::string str = "[at t = %1%\n";
        str+= "entry_time : %2%\n";
        str+= "range_covariate : %3%\n";
        str+= "time_on_study(t) : %4%\n";
        str+= "event_time(t) : %5%\n";
        str+= "is_failure(t) : %6%]";
        format f(str);
        f%t;
        f%entry_time();
        f%cont_to_str_t::get_indexed(range_covariate());
        f%time_on_study(t);
        f%event_time(t);
        std::string str_b= "false";
        if(this->is_failure(t)){ str_b = "true"; }
        f%str_b;
        return f.str();
    }


}//survival_data
}//boost
#endif
