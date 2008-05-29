/**
 * \file boost/mirror/detail/class_generator.hpp
 *  
 * Custom class generators
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_CLASS_GENERATOR_HPP
#define BOOST_MIRROR_META_DETAIL_CLASS_GENERATOR_HPP

#include <boost/mirror/detail/meta_attribs_outline.hpp>
#include <boost/mirror/algorithm/size.hpp>

namespace boost {
namespace mirror {
namespace detail {


template <
	class MetaAttributes,
	template <class> class Model, 
	class Position
>
struct class_generator_unit
{
	BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
		outline_holder,
		(MetaAttributes::get_outline_holder((Model<void>*)0, Position()))
	);
};

template <
	class Class, 
	template <class> class Model, 
	class Position, 
	class Size
>
struct class_generator_base
: public class_generator_unit<
	meta_class_attributes<Class>, Model, Position
>::outline_holder::type
, public class_generator_base<
	Class, Model, mpl::int_<Position::value+1>, Size
>
{
	typedef typename class_generator_unit<
		meta_class_attributes<Class>, Model, Position
	>::outline_holder::type head;

	typedef class_generator_base<
		Class, Model, mpl::int_<Position::value+1>, Size
	> tail;

	class_generator_base(Model<void>& model)
	: head(model)
	, tail(model)
	{ }

	class_generator_base(const Model<void>& model)
	: head(model)
	, tail(model)
	{ }
};

template <
	class Class, 
	template <class> class Model, 
	class Size
>
struct class_generator_base<Class, Model, Size, Size>
{
	class_generator_base(Model<void>& model){ }
	class_generator_base(const Model<void>& model){ }
};

} // namespace detail

template <
	class Class, 
	template <class> class Model
>
struct class_generator
: detail::class_generator_base<
	Class,
	Model,
	mpl::int_<0>,
	mpl::int_<mirror::size<meta_class_attributes<Class> >::value>
>
{
private:
	Model<void> model;
	typedef detail::class_generator_base<
		Class,
		Model,
		mpl::int_<0>,
		mpl::int_<mirror::size<meta_class_attributes<Class> >::value>
	> base_generator;
public:
	class_generator(void)
	: base_generator(model)
	{ }
};


} // namespace mirror
} // namespace boost

#endif //include guard

