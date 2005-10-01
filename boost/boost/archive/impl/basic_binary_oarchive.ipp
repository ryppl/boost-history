#ifndef BOOST_ARCHIVE_DETAIL_BASIC_BINARY_IARCHIVE_IPP
#define BOOST_ARCHIVE_DETAIL_BASIC_BINARY_IARCHIVE_IPP

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_binary_oarchive.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.
#include <string>
#include <cassert>
#include <algorithm>
#include <cstring>

#include <boost/config.hpp> // for BOOST_DEDUCED_TYPENAME
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::memcpy; 
}
#endif

#include <boost/archive/basic_binary_oarchive.hpp>

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implementation of binary_binary_oarchive

template<class Archive>
#if !defined(__BORLANDC__)
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
#else
void
#endif
basic_binary_oarchive<Archive>::init(){
    // write signature in an archive version independent manner
    const std::string file_signature(ARCHIVE_SIGNATURE());
    * this->This() << file_signature;
    // write library version
    const version_type v(ARCHIVE_VERSION());
    * this->This() << v;
}

} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_DETAIL_BASIC_BINARY_IARCHIVE_IPP

