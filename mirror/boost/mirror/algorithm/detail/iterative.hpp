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
#include <boost/mirror/algorithm/size.hpp>

namespace boost {
namespace mirror {
namespace detail {

	/** Declaration of the default iterative_algorithm
	 *  helper template.
	 */
	template <class meta_object_sequence, template <class, class> class algo_impl >
	struct iterative_algorithm { };

	/** Specialization of iterative_algorithm<meta_object_sequence,algo_impl>
	 *  for meta_class_attributes<>
	 */
	template <class reflected_class, class variant_tag, template <class, class> class algo_impl>
	struct iterative_algorithm<meta_class_attributes<reflected_class, variant_tag>, algo_impl >
	: algo_impl<
		meta_class_attributes<reflected_class, variant_tag>,
		size<meta_class_attributes<reflected_class, variant_tag> >
	>{ };

	/** Specialization of iterative_algorithm<meta_object_sequence,algo_impl>
	 *  for meta_class_all_attributes<>
	 */
	template <class reflected_class, class variant_tag, template <class, class> class algo_impl>
	struct iterative_algorithm<meta_class_all_attributes<reflected_class, variant_tag>, algo_impl >
	: algo_impl<
		meta_class_all_attributes<reflected_class, variant_tag>,
		size<meta_class_all_attributes<reflected_class, variant_tag> >
	>{ };


	/** Specialization of iterative_algorithm<meta_object_sequence,algo_impl>
	 *  for meta_base_classes<>
	 */
	template <class reflected_class, class variant_tag, template <class, class> class algo_impl>
	struct iterative_algorithm<meta_base_classes<reflected_class, variant_tag>, algo_impl >
	: algo_impl<
		meta_base_classes<reflected_class, variant_tag>,
		size<meta_base_classes<reflected_class, variant_tag> >
	>{ };

} // namespace detail

} // namespace mirror
} // namespace boost

#endif //include guard

