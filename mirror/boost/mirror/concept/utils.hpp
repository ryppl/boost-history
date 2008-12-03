/**
 * \file boost/mirror/concept/utils.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_CONCEPT_UTILS_HPP
#define BOOST_MIRROR_CONCEPT_UTILS_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

namespace boost {
namespace mirror {

#define BOOST_MIRROR_CONCEPT_TESTER_BEGIN \
private: \
        typedef char mismatch;\
        struct match { char x,y,z; };\
        static mismatch test(...);

#define BOOST_MIRROR_CONCEPT_TESTER_END \
public: \
        typedef typename ::boost::mpl::and_< \
		other_constraints, \
		::boost::mpl::bool_< \
	                sizeof(match) == \
	                sizeof(test((T*)0)) \
	        > \
	>::type type;


} // namespace mirror
} // namespace boost

#endif //include guard

