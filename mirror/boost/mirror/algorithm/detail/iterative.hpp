/**
 * \file boost/mirror/algorithm/detail/iterative.hpp
 * Iterative algorithms on mirror meta-object sequences
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_ITERATIVE_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_ITERATIVE_HPP

// mirror common definitions 
#include <boost/mirror/common_defs.hpp>
// forward declarations
#include <boost/mirror/meta_data_fwd.hpp>
// mirror::size
#include <boost/mirror/intrinsic/size.hpp>
#include <boost/mirror/iterator/iterator_equal.hpp>
#include <boost/mirror/iterator/next.hpp>
//
#include <boost/ref.hpp>


namespace boost {
namespace mirror {
namespace detail {

	template <
		class IteratorBegin, 
		class IteratorEnd,
		template <class, class> class AlgoImpl 
	>
	struct iterative_algorithm 
	: AlgoImpl<IteratorBegin, IteratorEnd>{ };

	/** Implementation of the for-each-like templates
	 */
	template <
		class IteratorBegin, 
		class IteratorEnd,
		class Direction
	>
	struct perform_on_range
	{
	private:
		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void do_apply_to(
			reference_wrapper<MetaObjectOp> op_ref, 
			reference_wrapper<TransformOp> transf_ref, 
			Iterator
		)
		{
			typedef typename boost::mirror::deref<
				Iterator
			>:: type meta_object;
			MetaObjectOp& op(op_ref);
			TransformOp& transf(transf_ref);
			op(transf(meta_object()));
		}
	
		typedef IteratorBegin begin;
		typedef IteratorEnd end;
		typedef typename mpl::int_<1> forward;
		typedef typename mpl::int_<-1> reverse;


		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void pre_apply_to(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf, 
			Iterator, 
			forward
		)
		{
			do_apply_to(op, transf, Iterator());
		}

		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void pre_apply_to(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf, 
			Iterator, 
			reverse
		){	}

		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void post_apply_to(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf, 
			Iterator, 
			forward
		){	}

		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void post_apply_to(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf, 
			Iterator, 
			reverse
		)
		{
			do_apply_to(op, transf, Iterator());
		}

	
		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void apply_to(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf, 
			Iterator, 
			mpl::true_
		){ }
	
		template <class MetaObjectOp, class TransformOp, class Iterator>
		static inline void apply_to(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf, 
			Iterator i, 
			mpl::false_
		)
		{
			Direction dir;
			pre_apply_to(op, transf, i, dir);
			typedef typename boost::mirror::next<Iterator>::type J;
			typename iterator_equal<J, end>::type done;
			apply_to(op, transf, J(), done); 
			post_apply_to(op, transf, i, dir);
		}
	public:
		template <class MetaObjectOp, class TransformOp>
		static inline reference_wrapper<MetaObjectOp> perform(
			reference_wrapper<MetaObjectOp> op, 
			reference_wrapper<TransformOp> transf
		)
		{
			typename iterator_equal<begin, end>::type done;
			apply_to(op, transf, begin(), done);
			return op;
		}
	};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

