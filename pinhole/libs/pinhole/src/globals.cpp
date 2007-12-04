// Pinhole pinhole.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/pinhole.hpp>

boost::shared_ptr<boost::pinhole::property_manager>
    boost::pinhole::property_manager::m_instance(new boost::pinhole::property_manager);

boost::pinhole::event_source
    *boost::pinhole::event_source::m_instance = 0;