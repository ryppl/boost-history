/**
 * \file boost/mirror/algorithm/detail/for_each.hpp
 * Implementation of the for_each algorithm 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_FOR_EACH_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_FOR_EACH_HPP

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// mirror::at
#include <boost/mirror/algorithm/at.hpp>

namespace boost {
namespace mirror {

namespace detail {

/** Implementation of the for_each function
 */
template <class meta_object_sequence, class sequence_size>
struct for_each_meta_object
{
protected:
	template <class meta_object_op, class transform_op, int I>
	static inline void do_apply_to(meta_object_op op, transform_op transf, mpl::int_<I> pos)
	{
		typedef typename boost::mirror::at<
			meta_object_sequence, mpl::int_<I> 
		>:: type meta_object;
		op(transf(meta_object()));
	}

	typedef mpl::int_< -1 > begin;

	template <class meta_object_op, class transform_op>
	static inline void apply_to(meta_object_op op, transform_op transf, begin){ }

	template <class meta_object_op, class transform_op, int I>
	static inline void apply_to(meta_object_op op, transform_op transf, mpl::int_<I> pos)
	{
		apply_to(op, transf, mpl::int_<I - 1>()), 
		do_apply_to(op, transf, pos);
	}
public:
	template <class meta_object_op, class transform_op>
	static inline meta_object_op perform(meta_object_op op, transform_op transf)
	{
		typedef mpl::int_<sequence_size::value - 1> last;
		apply_to(op, transf, last());
		return op;
	}
};

} // namespace detail

} // namespace mirror
} // namespace boost

#endif //include guard

