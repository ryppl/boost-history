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
        class Position
>
struct class_generator_unit
{
        BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
                generator_plugin,
                (MetaClassAttributes::template get_generator_plugin<
			MetaClassAttributes,
			MetaFunction
		>(Position()))
        );
        typedef typename generator_plugin::type type;
};


template <
	class MetaClassAttributes,
	template <class, class, class, class> class MetaFunction,
	class Position, 
	class Size
> struct class_generator_base
: public class_generator_unit<
	MetaClassAttributes,
	MetaFunction, 
	Position 
>::type
, public class_generator_base<
	MetaClassAttributes,
	MetaFunction, 
	mpl::int_<Position::value + 1>, 
	Size
>
{
	typedef typename class_generator_unit<
		MetaClassAttributes,
		MetaFunction, 
		Position
	>::type head;

	typedef class_generator_base<
		MetaClassAttributes,
		MetaFunction, 
		mpl::int_<Position::value + 1>, 
		Size 
	> tail;

	class_generator_base(void)
	{ }

	template <class Param>
	class_generator_base(Param& init)
	 : head(init)
	 , tail(init)
	{ }
};

template <
	class MetaClassAttributes,
	template <class, class, class, class> class MetaFunction,
	class Size
>
struct class_generator_base<
	MetaClassAttributes, 
	MetaFunction, 
	Size, 
	Size 
>
{
	class_generator_base(void){ }

	template <class Param>
	class_generator_base(Param&){ }
};

} // namespace detail

/** Creates a class hierarchy based on the results
 *  of the GeneratorUnit meta-function, with the 
 *  metaObjectSequence, individual meta-object 
 *  Positions and the Data as parameters.
 */
template <
	class MetaClassAttributes, 
	template <class, class, class, class> class MetaFunction
> struct class_generator
 : detail::class_generator_base<
	MetaClassAttributes,
	MetaFunction,
	mpl::int_<0>,
	mpl::int_<mirror::size<MetaClassAttributes>::value>
>
{
private:
	typedef detail::class_generator_base<
		MetaClassAttributes,
		MetaFunction,
		mpl::int_<0>,
		mpl::int_<mirror::size<MetaClassAttributes>::value>
	> base_generator;
public:
	class_generator(void) { }

	template <class Param>
	class_generator(Param& init)
	 : base_generator(init)
	{ }
};


} // namespace mirror
} // namespace boost

#endif //include guard

