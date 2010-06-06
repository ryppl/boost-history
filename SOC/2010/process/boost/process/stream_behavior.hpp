//
// Boost.Process
// ~~~~~~~~~~~~~
//
// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008, 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

/**
 * \file boost/process/stream_behavior.hpp
 *
 * Includes the declaration of the stream_behavior enumeration. 
 *
 */

#ifndef BOOST_PROCESS_STREAM_BEHAVIOR_HPP
#define BOOST_PROCESS_STREAM_BEHAVIOR_HPP

#include <boost/process/config.hpp>

namespace boost {
namespace process {

/**
 *
 * Stream behaviors to define how standard streams are passed to a child process.
 *
 * inherit: A stream is inherited from its parent process. 
 * capture: A stream is redirected to its parent process (anonymous pipe). 
 * mute: A stream is redirected to /dev/null, /dev/zero or NUL. 
 * close: A stream is closed. 
 *
 */
enum stream_behavior { inherit, capture, mute, close }; 

}
}

#endif
