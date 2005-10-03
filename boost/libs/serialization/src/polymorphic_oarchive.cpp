/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// polymorphic_text_oarchive.cpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// fast array serialization (C) Copyright 2005 Matthias Troyer
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#if (defined _MSC_VER) && (_MSC_VER < 1300)
#  pragma warning (disable : 4786) // too long name, harmless warning
#endif

#include <ostream>

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/polymorphic_oarchive.hpp>

// explicitly instantiate for this type of stream
#include <boost/archive/impl/archive_pointer_oserializer.ipp>

namespace boost {
namespace archive {

template class detail::archive_pointer_oserializer<polymorphic_oarchive> ;

// default implementations for arrays by loops

#define BOOST_ARCHIVE_IMPLEMENT_POLYMPORPHIC_FUNCTION(T)  \
void polymorphic_oarchive::save_array(T const * p, std::size_t length)  \
{                                                                       \
  while (length--)                                                      \
    save(*p--);                                                         \
}

#include <boost/archive/detail/implement_polymorphic_function.hpp>
	
#undef BOOST_ARCHIVE_IMPLEMENT_POLYMPORPHIC_FUNCTION

} // namespace archive
} // namespace boost
