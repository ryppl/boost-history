/**
 * \file boost/mirror/algorithm/detail/reverse_for_each.hpp
 * Implementation of the reverse_for_each algorithm 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_REVERSE_FOR_EACH_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_REVERSE_FOR_EACH_HPP

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
struct reverse_for_each_meta_object
{
protected:
	template <class meta_object_op, class transform_op, int I>
	static void do_apply_to(meta_object_op op, transform_op transf, mpl::int_<I> pos)
	{
		typedef typename boost::mirror::at<
			meta_object_sequence, mpl::int_<I> 
		>:: type meta_object;
		op(transf(meta_object()));
	}

	typedef typename mpl::int_<sequence_size::value> end;

	template <class meta_object_op, class transform_op>
	static void apply_to(meta_object_op op, transform_op transf, end){ }

	template <class meta_object_op, class transform_op, int I>
	static void apply_to(meta_object_op op, transform_op transf, mpl::int_<I> pos)
	{
		apply_to(op, transf, mpl::int_<I + 1>()), 
		do_apply_to(op, transf, pos);
	}
public:
	template <class meta_object_op, class transform_op>
	static meta_object_op perform(meta_object_op op, transform_op transf)
	{
		typedef mpl::int_<0> first;
		apply_to(op, transf, first());
		return op;
	}
};

} // namespace detail

} // namespace mirror
} // namespace boost

#endif //include guard

