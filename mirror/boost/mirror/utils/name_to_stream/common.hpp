/**
 * \file boost/mirror/utils/name_to_stream/common.hpp
 * Common things required by the name_to_stream utility
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_UTILS_NAME_TO_STREAM_COMMON_HPP
#define BOOST_MIRROR_UTILS_NAME_TO_STREAM_COMMON_HPP

namespace boost {
namespace mirror {

/** name_to_stream_helper function object
 */
template <class MetaObject> 
struct name_to_stream_helper;

/** name_to_stream function object
 *  Puts the whole name of the meta object into the given stream.
 */
template <typename MetaObject> 
class name_to_stream : public name_to_stream_helper<MetaObject>
{
private:
	typedef name_to_stream_helper<MetaObject> _helper;
	bool put_leading_dbl_cln;
public:
	// default constructor 
	name_to_stream(bool ldng_dbl_cln = false)
	 : put_leading_dbl_cln(ldng_dbl_cln)
	{ }
	//
	// use the inherited put function too
	using _helper::put;
	//
	//! This version of put uses the stored flag put_leading_dbl_cln
	template <class OutStream>
	OutStream& put(OutStream& s) const 
	{
		return _helper::put(s, put_leading_dbl_cln);
	}
};

/** << operator overload for meta-namespaces
 */
template <class OutStream, class MetaObject>
OutStream& operator << (OutStream& dst, const name_to_stream< MetaObject >& mns)
{
	return mns.put(dst);
}


} // namespace mirror
} // namespace boost

#endif //include guard

