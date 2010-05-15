// 
// Boost.Process 
// ~~~~~~~~~~~~~ 
// 
// Copyright (c) 2006, 2007 Julio M. Merino Vidal 
// Copyright (c) 2008, 2009 Boris Schaeling 
// 
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) 
// 

/** 
 * \file boost/process/stream_behavior.hpp 
 * 
 * Includes the declaration of the stream_behavior
 *
 */ 

#ifndef BOOST_PROCESS_STREAM_BEHAVIOR_HPP 
#define BOOST_PROCESS_STREAM_BEHAVIOR_HPP 

#include <boost/process/config.hpp> 

namespace boost {
namespace process {

        /* 
         *
         * stream_behavior defines how a stream should behavior 
         * from a brand-new child.
         *
         * inherit: The stream are inherit from it's father
         * capture: The stream is connected with an anonymous pipe
         * closed: The stream is closed as soon as the process is created
         * dummy: Dummy data will be writed or read from this stream
         *
         */
        enum stream_behavior { inherit, capture, closed, dummy};

}
}

#endif 
