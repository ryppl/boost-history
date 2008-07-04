/**
 * \file boost/mirror/detail/nontrivial_type_name.hpp
 * Helpers for composing a nontrivial typenames
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_NONTRIVIAL_TYPE_NAME_HPP
#define BOOST_MIRROR_META_DETAIL_NONTRIVIAL_TYPE_NAME_HPP

#ifndef BOOST_MIRROR_USE_STATIC_NAME_STRINGS
#	include <memory> // ::std::auto ptr
#endif
namespace boost {
namespace mirror {
namespace detail {

template <class MetaType, bool BaseOrFull>
struct nontrivial_type_base_or_full_name;

/** Base name 
 */
template <class MetaType>
struct nontrivial_type_base_or_full_name<MetaType, true>
{
	BOOST_STATIC_CONSTANT(
		int,
		name_length =
		MetaType::base_name_length
	);
	inline static const bchar* name(void)
	{
		return MetaType::base_name();
	}
};

/** Full name 
 */
template <class MetaType>
struct nontrivial_type_base_or_full_name<MetaType, false>
{
	BOOST_STATIC_CONSTANT(
		int,
		name_length =
		MetaType::full_name_length
	);
	inline static const bchar* name(void)
	{
		return MetaType::full_name();
	}
};

template <class MetaType, typename MetaData, template <class, typename, bool> class Implementation>
struct static_nontrivial_type_name
: Implementation<MetaType, MetaData, true>
, Implementation<MetaType, MetaData, false>
{
private:
	typedef Implementation<MetaType, MetaData, true>  implementation_base_name;
	typedef Implementation<MetaType, MetaData, false> implementation_full_name;

	inline static bchar* new_string(const size_t size)
	{
		assert(size != 0);
		bchar* result = new bchar[size];
		result[0] = 0;
		return result;
	}

	inline static bool name_not_initialized(const bchar* str)
	{
		return !str[0];
	}

	template <bool FormatBaseName>
	static const bchar* get_name(mpl::bool_<FormatBaseName>)
	{
		typedef Implementation<MetaType, MetaData, FormatBaseName>
			impl;
		const int name_len(impl::name_length);
#ifndef BOOST_MIRROR_USE_DYNAMIC_NAME_STRINGS
		//static bchar the_name[name_len + 1] = {BOOST_STR_LIT("")};
		// TODO: the previews line won't compile since
		// name_len is not an integral constant.
		// Thus we need to find some better workaround 
		// because this one will cause memory leaks.
		static bchar* the_name = new_string(name_len+1);
#else
		static ::std::auto_ptr<bchar> the_name_holder(new_string(name_len+1));
		bchar* the_name = the_name_holder.get();
#endif
		if(name_not_initialized(the_name)) 
			impl::init_name(the_name);
		return the_name;
	}
public:
	BOOST_STATIC_CONSTANT(
		int,
		base_name_length =
		implementation_base_name::name_length
	);
	BOOST_STATIC_CONSTANT(
		int,
		full_name_length =
		implementation_full_name::name_length
	);
	static const bchar* base_name(void)
	{
		return get_name(mpl::bool_<true>());
	}
	static const bchar* full_name(void)
	{
		return get_name(mpl::bool_<false>());
	}
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

