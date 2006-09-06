//
// Boost.Process
//
// Copyright (c) 2006 Julio M. Merino Vidal.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt.)
//

//!
//! \file boost/process/pipeline.hpp
//!
//! Includes the declaration of the pipeline class.
//!

#if !defined(BOOST_PROCESS_PIPELINE_HPP)
/** \cond */
#define BOOST_PROCESS_PIPELINE_HPP
/** \endcond */

#include <boost/process/basic_pipeline.hpp>

namespace boost {
namespace process {

// ------------------------------------------------------------------------

//!
//! \brief Generic instantiation of the basic_pipeline template.
//!
//! Generic instantiation of the basic_pipeline template.  This uses common
//! classes as the parameters.
//!
//! XXX This class should most probably go away.
//!
typedef basic_pipeline< std::string, std::vector< std::string > > pipeline;

// ------------------------------------------------------------------------

} // namespace process
} // namespace boost

#endif // !defined(BOOST_PROCESS_PIPELINE_HPP)
