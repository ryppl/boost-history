/**
 * \file boost/mirror/meta_namespace.hpp
 * Registering and reflection of namespaces
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_NAMESPACE
#define BOOST_MIRROR_META_NAMESPACE

// preprocessor related things
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/reverse.hpp>
#include <boost/preprocessor/seq/pop_back.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/wstringize.hpp>

// template meta programming
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>

// forward declarations and common mirror defs
#include <boost/mirror/common_defs.hpp>
#include <boost/mirror/meta_data_fwd.hpp>

// char-type related 
#include <boost/char_type_switch/string.hpp>
// full name builder
#include <boost/mirror/detail/full_name_builder.hpp>


namespace boost {
namespace mirror {

/** Meta data about namespaces
 */
template <class NamespacePlaceholder>
struct meta_namespace
: detail::full_name_builder< 
	meta_namespace<typename NamespacePlaceholder::parent_placeholder>,
	NamespacePlaceholder
>
{
	typedef  detail::full_name_builder<
        	meta_namespace<typename NamespacePlaceholder::parent_placeholder>,
        	NamespacePlaceholder
	> base_class;

	inline static const cts::bstring& base_name(void)
	{
		return base_class::get_name(
			mpl::false_(),
			cts::bchar_traits()
		);
	}

	inline static const cts::bstring& full_name(void)
	{
		return base_class::get_name(
			mpl::true_(),
			cts::bchar_traits()
		);
	}


	// the parent meta_namespace
	typedef meta_namespace<
		typename NamespacePlaceholder::parent_placeholder
	> scope;

	//
	typedef typename ::boost::mpl::push_back<
		typename scope::ancestors,
		scope
	>::type ancestors;
};

/** The declaration of the namespace placeholder for the 
 *  global scope
 */
namespace namespace_ {
	struct _ 
	{
		// the base name of the namespace
		template <typename CharT>
		inline static const ::std::basic_string<CharT>& get_name(
			mpl::false_,
			::std::char_traits<CharT>
		)
		{
			// the base name of the global scope is an empty string
			static ::std::basic_string<CharT> s_name;
			return s_name;
		}

		// the full name of the namespace
		template <typename CharT>
		inline static const ::std::basic_string<CharT>& get_name(
			mpl::true_,
			::std::char_traits<CharT> _cht
		)
		{
			// the same as the base name
			return get_name(mpl::false_(), _cht);
		}

		inline static const cts::bstring& base_name(void)
		{
			return get_name(
				mpl::false_(),
				cts::bchar_traits()
			);
		}
	
		inline static const cts::bstring& full_name(void)
		{
			return get_name(
				mpl::true_(),
				cts::bchar_traits()
			);
		}

	};

} // namespace namespace_

// meta_namespace specialization for the global scope
template < >
struct meta_namespace< namespace_ :: _ > : namespace_ :: _ 
{ 
	typedef meta_namespace< namespace_ :: _ > scope;
	typedef mpl::vector0<> ancestors;
};


// helper macro expanded multiple times during the namespace registration
#define BOOST_MIRROR_REG_NAMESPACE_PROLOGUE_HELPER(R, DATA, NAMESPACE_NAME) \
	namespace NAMESPACE_NAME {

// helper macro expanded multiple times during the namespace registration
#define BOOST_MIRROR_REG_NAMESPACE_EPILOGUE_HELPER(R, DATA, NAMESPACE_NAME) \
	} // namespace NAMESPACE_NAME

// helper macro expanded multiple times during the namespace registration
#define BOOST_MIRROR_REG_NAMESPACE_ENUM_HELPER(R, DATA, NAMESPACE_NAME) \
	:: NAMESPACE_NAME 


/** Macro that does the registering of a namespace.
 *  The argument must be a BOOST_PP sequence containing the
 *  names of the parent namspaces and the registered namespace.
 *  
 *  The sequence needed to register namespace ::test is (test)
 *  To register ::test::foo::bar::baz use (test)(foo)(bar)(baz)
 * 
 *  this macro expands for the sequence (test)(foo)(bar)(baz)
 *  this way:
 *  namespace namespace_ { // -1-
 *  namespace test {
 *  namespace foo {
 *  namespace bar {
 *  namespace baz { // -2-
 *  struct _  // -3-
 *  {
 *    typedef ::boost::mirror::namespace_::test::foo::bar::_
 *      parent_placeholder; // -4-
 *    static const bchar* get_name(mpl::false_)(return "baz";} // -5-
 *  };
 *  } // namespace baz
 *  } // namespace bar
 *  } // namespace foo
 *  } // namespace test
 *  } // -9-
 */
#define BOOST_MIRROR_REG_NAMESPACE(NAME_SEQUENCE) \
	namespace namespace_ { /* -1- */ \
	BOOST_PP_SEQ_FOR_EACH( \
		BOOST_MIRROR_REG_NAMESPACE_PROLOGUE_HELPER, \
		_, \
		NAME_SEQUENCE \
	) /* -2- */ \
		struct _ /* -3- */ \
		{ \
			typedef ::boost::mirror::namespace_ \
			BOOST_PP_SEQ_FOR_EACH( \
				BOOST_MIRROR_REG_NAMESPACE_ENUM_HELPER, \
				_, \
				BOOST_PP_SEQ_POP_BACK(NAME_SEQUENCE) \
			) :: _ parent_placeholder; /* -4- */ \
			static inline const ::std::string& get_name( \
				mpl::false_, \
				::std::char_traits<char> \
			) \
			{ \
				static ::std::string s_name(BOOST_PP_STRINGIZE( \
					BOOST_PP_SEQ_HEAD( \
						BOOST_PP_SEQ_REVERSE(NAME_SEQUENCE) \
					) \
				)); \
				return s_name; \
			} \
			static inline const ::std::wstring& get_name( \
				mpl::false_, \
				::std::char_traits<wchar_t> \
			) \
			{ \
				static ::std::wstring s_name(BOOST_PP_WSTRINGIZE( \
					BOOST_PP_SEQ_HEAD( \
						BOOST_PP_SEQ_REVERSE(NAME_SEQUENCE) \
					) \
				)); \
				return s_name; \
			} /* -5- */ \
		}; \
		} \
		BOOST_PP_SEQ_FOR_EACH( \
			BOOST_MIRROR_REG_NAMESPACE_EPILOGUE_HELPER, \
			_, \
			NAME_SEQUENCE \
		) /* -9- */ 


/** Register some of the common namespaces
 */
BOOST_MIRROR_REG_NAMESPACE((std))
BOOST_MIRROR_REG_NAMESPACE((boost))
BOOST_MIRROR_REG_NAMESPACE((boost)(mirror))

} // namespace mirror
} // namespace boost

#endif //include guard

