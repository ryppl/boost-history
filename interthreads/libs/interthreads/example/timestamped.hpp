#ifndef BOOST_INTERTHREADS_TIMESTAMPED_HPP
#define BOOST_INTERTHREADS_TIMESTAMPED_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/thread_time.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/thread/thread.hpp>

namespace boost {
namespace interthreads {
    template <typename T>
    struct timestamped {
#define XTIME
#ifdef XTIME
        boost::xtime date_;
        void reset_date(unsigned seq) {
	        boost::xtime_get(&date_,1);
            seq_ = seq;
        }
        struct ref_comparator {
            typedef timestamped value_type;
            bool operator()(const value_type&lhs, const value_type&rhs) {
                if (lhs.date_.sec > rhs.date_.sec) return true;
                else if (lhs.date_.sec == rhs.date_.sec && lhs.date_.nsec >= rhs.date_.nsec) return true;
                else if (lhs.date_.nsec == rhs->date_.nsec && lhs.seq_ < rhs->seq_) return true;
                return false;
            }
        };
        struct ptr_comparator_gt {
            typedef timestamped* value_type;
            bool operator()(const value_type&lhs, const value_type&rhs) {
                if (lhs->date_.sec > rhs->date_.sec) return true;
                else if (lhs->date_.sec == rhs->date_.sec && lhs->date_.nsec > rhs->date_.nsec) return true;
                else if (lhs->date_.nsec == rhs->date_.nsec && lhs->seq_ > rhs->seq_) return true;
                return false;
            }
        };
        struct ptr_comparator_lt {
            typedef timestamped* value_type;
            bool operator()(const value_type&lhs, const value_type&rhs) {
                if (lhs->date_.sec < rhs->date_.sec) return true;
                else if (lhs->date_.sec == rhs->date_.sec && lhs->date_.nsec < rhs->date_.nsec) return true;
                else if (lhs->date_.nsec == rhs->date_.nsec && lhs->seq_ < rhs->seq_) return true;
                return false;
            }
        };
#else
        system_time date_;
        void reset_date(unsigned seq) {
            date_ = system_time();	
            seq_ = seq;
        }
        struct ref_comparator {
            typedef timestamped value_type;
            bool operator()(const value_type&lhs, const value_type&rhs) {
                return lhs.date_ >= rhs.date_;
            }
        };
        struct ptr_comparator_gt {
            typedef timestamped* value_type;
            bool operator()(const value_type&lhs, const value_type&rhs) {
                return (lhs->date_ > rhs->date_) ? true :
                        (lhs->date_ == rhs->date_) && (lhs->seq_ > rhs->seq_)? true:false;
            }
        };
#endif
        unsigned seq_;
        T value_; 
    };   
}
}


#endif
