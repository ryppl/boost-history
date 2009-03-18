///////////////////////////////////////////////////////////////////////////////
//  stream_to_dataset.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_SURVIVAL_DATA_FILL_DATASET_FROM_STREAM_HPP_ER_2009
#define BOOST_SURVIVAL_DATA_FILL_DATASET_FROM_STREAM_HPP_ER_2009
#include <istream>
#include <string>
#include <sstream>
#include <boost/range.hpp>
#include <boost/utility/remove_qualifier.hpp>
#include <boost/survival_data/unit.hpp>
#include <boost/survival_data/parser.hpp>
namespace boost{
namespace survival_data{

    /// \class read_from_stream
    /// \brief Reads a survival dataset from a stream
    class stream_to_dataset{
     public:
    /// \param Dataset Back Insertion Sequence
    /// \param is and input stream with each satisfying
    /// the format shown between the arrows below: \n
    /// ->entry_time, x_0, . . . x_{J-1}, is_failure, event_time<-\n
    /// where
    ///  - entry_time is in \c Real_type or <tt> unsigned int </tt> format
    ///  - x_j is in \c Real_type or <tt> int </tt> format
    ///  - is_failure takes values is in \f$\{0,1\}\f$
    ///  - event_time is in \c Real_type or <tt> unsigned int </tt> format
    ///
    /// Here's how the data should be formatted for the
    /// <a href="http://www.mathstat.helsinki.fi/openbugs/Examples/Mice.html">
    /// mice data </a>
    /// (showing observations 28 and 29):\n
    /// ...\n
    /// 0,0,1,0,0,0,40\n
    /// 0,0,1,0,0,1,25\n
    /// ...\n
        template<typename Dataset>
        static void fill(
            std::istream& is,
            Dataset& dataset
        ){
            typedef typename utility::remove_qualifier<
                typename range_value<Dataset>::type
            >::type unit_t;

            std::string line;
            //TODO exception handling
            while(std::getline(is,line) ){
                dataset.push_back(parser<unit_t,std::string>(line));
            }
        }
	};

}//survival_data
}//boost


#endif
