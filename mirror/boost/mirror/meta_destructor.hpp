/**
 * \file boost/mirror/meta_destructor.hpp
 * Meta data concerning class' destructors
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DESTRUCTOR_HPP
#define BOOST_MIRROR_META_DESTRUCTOR_HPP


namespace boost {
namespace mirror {

/** Meta class - specializes the meta_type for classes
 */
template <
	class Class, 
	class VariantTag
>
struct meta_class_destructor
{
	inline static void delete_instance(Class* i){delete i;}
	inline static void destroy_instance(Class& i){i.~Class();}
};


} // namespace mirror
} // namespace boost

#endif //include guard

