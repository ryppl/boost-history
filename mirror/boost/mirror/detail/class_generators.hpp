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

#include <boost/mirror/intrinsic/size.hpp>
#include <boost/typeof/typeof.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <
        class MetaClassAttributes,
	template <class, class, class, class> class MetaFunction,
        class Position,
	class UnitKindSelector
>
struct class_generator_unit
{
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
                generator_plugin,
                (MetaClassAttributes::template get_generator_plugin<
			MetaClassAttributes,
			MetaFunction
		>(Position(), UnitKindSelector()))
        );
        typedef typename generator_plugin::type type;
};


template <
	class MetaClassAttributes,
	template <class, class, class, class> class MetaFunction,
	class Position, 
	class Size,
	class UnitKindSelector
> struct class_generator_base
: public class_generator_unit<
	MetaClassAttributes,
	MetaFunction, 
	Position,
	UnitKindSelector
>::type
, public class_generator_base<
	MetaClassAttributes,
	MetaFunction, 
	mpl::int_<Position::value + 1>, 
	Size,
	UnitKindSelector
>
{
	typedef typename class_generator_unit<
		MetaClassAttributes,
		MetaFunction, 
		Position,
		UnitKindSelector
	>::type head;

	typedef class_generator_base<
		MetaClassAttributes,
		MetaFunction, 
		mpl::int_<Position::value + 1>, 
		Size,
		UnitKindSelector
	> tail;

	inline class_generator_base(void)
	{ }

	inline class_generator_base(const class_generator_base& other)
	 : head(static_cast<const head&>(other))
	 , tail(static_cast<const tail&>(other))
	{ }

	template <class Param>
	inline class_generator_base(Param init)
	 : head(init)
	 , tail(init)
	{ }
};

template <
	class MetaClassAttributes,
	template <class, class, class, class> class MetaFunction,
	class Size,
	class UnitKindSelector
>
struct class_generator_base<
	MetaClassAttributes, 
	MetaFunction, 
	Size, 
	Size,
	UnitKindSelector
>
{
	inline class_generator_base(void){ }

	template <class Param>
	inline class_generator_base(Param){ }
};

/** Creates a class hierarchy based on the results
 *  of the GeneratorUnit meta-function, with the 
 *  metaObjectSequence, individual meta-object 
 *  Positions and the Data as parameters.
 */
template <
	class MetaClassAttributes, 
	template <class, class, class, class> class MetaFunction,
	class UnitKindSelector
> struct class_generator
 : detail::class_generator_base<
	MetaClassAttributes,
	MetaFunction,
	mpl::int_<0>,
	mpl::int_<mirror::size<MetaClassAttributes>::value>,
	UnitKindSelector
>
{
private:
	typedef detail::class_generator_base<
		MetaClassAttributes,
		MetaFunction,
		mpl::int_<0>,
		mpl::int_<mirror::size<MetaClassAttributes>::value>,
		UnitKindSelector
	> base_generator;
public:
	inline class_generator(void) { }

	inline class_generator(const class_generator& other)
	 : base_generator(static_cast<const base_generator&>(other))
	{ }

	template <class Param>
	inline class_generator(Param init)
	 : base_generator(init)
	{ }
};

/** A class generator unit which "returns" the result type of the
 *  get_meta_attrib_holder function on the MetaClassAttributes.
 */
template <
        class Class,
        class VariantTag,
        class MetaClassAttributes,
        class Position
>
struct get_meta_attrib_generator_plugin
{
        typedef meta_class_attribute<
                Class,
                VariantTag,
                MetaClassAttributes,
                Position
        > type;
};


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

