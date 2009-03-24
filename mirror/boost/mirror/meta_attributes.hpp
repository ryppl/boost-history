/**
 * \file boost/mirror/meta_attributes.hpp
 * Registering and reflection of class attributes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_ATTRIBUTES_HPP
#define BOOST_MIRROR_META_ATTRIBUTES_HPP

// macros for declaration of simple meta attributes
#include <boost/mirror/detail/meta_attribs_base.hpp>
#include <boost/mirror/detail/meta_all_attribs_base.hpp>

namespace boost {
namespace mirror {

template < class Class, class VariantTag >
struct meta_class_all_attributes
 : public detail::meta_class_attributes_offset_calculator<
	Class,
	detail::meta_class_all_attributes_base<Class, VariantTag>
>
{
private:
	typedef ::boost::mirror::detail::meta_class_attributes_offset_calculator<
		Class,
		detail::meta_class_all_attributes_base<
			Class, 
			VariantTag
		>
	> offs_calc;

public:
	template <int I>
	static inline ptrdiff_t offset_of(mpl::int_<I> pos)
	{
		return offs_calc::get_offset_of(pos, (Class*)0);
	}

};

/** Instances of this template are used to store information 
 *  about single class' member attribute and are used mainly
 *  in the algorithms.
 */
template <
	class ReflectedType, 
	class VariantTag,
	class MetaAttributes,
	class AttribPos
>
struct meta_class_attribute
{
public:
	// the meta-class for the class to which 
	// the attribute belongs
	// this is actually the scope of the meta-attribute
	typedef ::boost::mirror::meta_class<ReflectedType, VariantTag> 
		scope;
private:
	struct result_of_get
	{
		typedef typename mpl::at<
			typename MetaAttributes::type_list,
			AttribPos
		>::type type;
	};
	typedef typename scope::reflected_type
		owner_class;

	inline static const char* get_double_colon(::std::char_traits<char>)
	{
		return "::";
	}

	inline static const wchar_t* get_double_colon(::std::char_traits<wchar_t>)
	{
		return L"::";
	}

	template <typename CharT>
	inline static ::std::basic_string<CharT> init_full_name(
		::std::char_traits<CharT> cht
	)
	{
		typedef ::std::basic_string<CharT> str_type;
		str_type name(scope::get_name(mpl::true_(), cht));
		name.append(str_type(get_double_colon(cht)));
		name.append(base_name());
		return name;
	}

public:

	// the meta-attributes list (own/all)
	// into which the attribute belongs
	// in this instance
	typedef MetaAttributes container;
	typedef MetaAttributes meta_attributes;
	// the position of the meta-attribute 
	typedef AttribPos position;
private:
	// use type::reflected_type to get the type 
	// of the attribute
	typedef typename mpl::at<
		typename MetaAttributes::type_list,
		position
	>::type attribute_type;
public:


	template <typename CharT>
	inline static const ::std::basic_string<CharT>& get_name(
		mpl::false_ full_name,
		::std::char_traits<CharT> cht
	)
	{
		return meta_attributes::get_name(
			position(),
			full_name,
			cht
		);
	}

	// base name getter
	inline static const cts::bstring& base_name(void)
	{
		return get_name(
			mpl::false_(),
			cts::bchar_traits()
		);
	}

	template <typename CharT>
	inline static const ::std::basic_string<CharT>& get_name(
		mpl::true_ full_name,
		::std::char_traits<CharT> cht
	)
	{
		typedef ::std::basic_string<CharT> str_type;
		static str_type name(init_full_name(cht));
		return name;
	}

	// full name getter
	inline static const cts::bstring& full_name(void)
	{
		return get_name(
			mpl::true_(),
			cts::bchar_traits()
		);
	}

	// address getter
	inline static attribute_type* address(owner_class& instance)
	{
		return meta_attributes::address(instance, position());
	}

	// offset getter
	inline static ptrdiff_t offset(owner_class& instance)
	{
		return meta_attributes::offset(instance, position());
	}

	// offset getter
	inline static ptrdiff_t offset_of(void)
	{
		return meta_attributes::offset_of(position());
	}

	// value getter
	inline static typename result_of_get::type get(
		const owner_class& instance
	)
	{
		return meta_attributes::get(instance, position());
	}

	// value query
	template <typename DestType>
	inline static DestType& query(
		const owner_class& instance, 
		DestType& dest
	)
	{
		return meta_attributes::query(instance, position(), dest);
	}

	// value setter
	inline static void set(
		owner_class& instance, 
		typename call_traits<attribute_type>::param_type val
	)
	{
		meta_attributes::set(instance, position(), val);
	}

	// value setter
	inline static void set(
		const owner_class& instance, 
		typename call_traits<attribute_type>::param_type val
	)
	{
		meta_attributes::set(instance, position(), val);
	}

private:
	// attribute traits
	BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
		detail_traits, 
		meta_attributes::get_traits(position())
	)
public:
	typedef typename detail_traits::type traits;
private:
	// mirror type selector 
	typedef typename traits::meta_type_selector
		typedef_or_type;
public:
	/** the meta_class/meta_type reflecting 
	 *  the type of the attribute.
	 */
	// TODO: this need to be fixed to work with the selector
	//typedef BOOST_MIRRORED_CLASS(typedef_or_type)
	typedef BOOST_MIRRORED_CLASS(attribute_type)
		type;

};


} // namespace mirror
} // namespace boost

#endif //include guard

