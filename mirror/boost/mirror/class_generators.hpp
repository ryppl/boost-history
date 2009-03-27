/**
 * \file boost/mirror/class_generators.hpp
 * 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_INTRINSIC_BY_NAME_HPP
#define BOOST_MIRROR_INTRINSIC_BY_NAME_HPP

#include <boost/mirror/meta_attributes.hpp>
#include <boost/mirror/detail/class_generators.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <template <class> class Model>
struct apply_meta_function_generator_plugin_wrapper
{
	template <
	        class Class,
	        class VariantTag,
	        class MetaClassAttributes,
	        class Position
	>
	struct generator_plugin
	{
	        typedef typename Model<
			meta_class_attribute<
		                Class,
		                VariantTag,
		                MetaClassAttributes,
		                Position
			>
	        >::type type;
	};
};


} // namespace detail

template <
	class MetaClassAttributes,
	template <class> class Model,
	class UnitKindSelector
> struct class_generator
 : detail::class_generator<
	MetaClassAttributes,
	detail::
	apply_meta_function_generator_plugin_wrapper<Model>::
	template generator_plugin,
	UnitKindSelector
>
{
private:
	typedef detail::class_generator<
	        MetaClassAttributes,
		detail::
		apply_meta_function_generator_plugin_wrapper<Model>::
		template generator_plugin,
		UnitKindSelector
	> base_class;
public:
	inline class_generator(void){ }

	inline class_generator(const class_generator& other)
	 : base_class(static_cast<const base_class&>(other))
	{ }

	/*	
	inline class_generator(class_generator& other)
	 : base_class(static_cast<const base_class&>(other))
	{ }
	 */

	template <class Param>
	inline class_generator(Param param)
	 : base_class(param)
	{ }
};

} // namespace mirror
} // namespace boost

#endif //include guard

