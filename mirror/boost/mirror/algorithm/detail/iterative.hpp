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
	template <class MetaObjectSequence, template <class, class> class AlgoImpl >
	struct iterative_algorithm { };

	/** Specialization of iterative_algorithm<MetaObjectSequence,AlgoImpl>
	 *  for meta_class_attributes<>
	 */
	template <class Class, class VariantTag, template <class, class> class AlgoImpl>
	struct iterative_algorithm<meta_class_attributes<Class, VariantTag>, AlgoImpl >
	: AlgoImpl<
		meta_class_attributes<Class, VariantTag>,
		size<meta_class_attributes<Class, VariantTag> >
	>{ };

	/** Specialization of iterative_algorithm<MetaObjectSequence,AlgoImpl>
	 *  for meta_class_all_attributes<>
	 */
	template <class Class, class VariantTag, template <class, class> class AlgoImpl>
	struct iterative_algorithm<meta_class_all_attributes<Class, VariantTag>, AlgoImpl >
	: AlgoImpl<
		meta_class_all_attributes<Class, VariantTag>,
		size<meta_class_all_attributes<Class, VariantTag> >
	>{ };


	/** Specialization of iterative_algorithm<MetaObjectSequence,AlgoImpl>
	 *  for meta_base_classes<>
	 */
	template <class Class, class VariantTag, template <class, class> class AlgoImpl>
	struct iterative_algorithm<meta_base_classes<Class, VariantTag>, AlgoImpl >
	: AlgoImpl<
		meta_base_classes<Class, VariantTag>,
		size<meta_base_classes<Class, VariantTag> >
	>{ };

} // namespace detail

} // namespace mirror
} // namespace boost

#endif //include guard

