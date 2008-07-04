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

// template meta programming
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
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
		inline static const bchar* base_name(void){return BOOST_STR_LIT("");}

		// length of the basename:number of characters w/o the terminating '\0'
		typedef ::boost::mpl::int_<0>::type base_name_length;

		// the base name of the namespace
		inline static const bchar* full_name(void){return base_name();}

		// the length of the full name without the terminating null
		typedef base_name_length full_name_length;
	};

	// this is necessary for MIRORRED_NAMESPACE() to work with
	// the global scope
	namespace __ {
		typedef struct namespace_::_ _;
	} // namespace __ 
} // namespace namespace_

// meta_namespace specialization for the global scope
template < >
struct meta_namespace< namespace_ :: _ > : namespace_ :: _ 
{ 
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
 */
#define BOOST_MIRROR_REG_NAMESPACE(NAME_SEQUENCE) \
	namespace namespace_ { \
	BOOST_PP_SEQ_FOR_EACH(BOOST_MIRROR_REG_NAMESPACE_PROLOGUE_HELPER, _, NAME_SEQUENCE) \
		struct _ \
		{ \
			typedef namespace_ \
			BOOST_PP_SEQ_FOR_EACH( \
				BOOST_MIRROR_REG_NAMESPACE_ENUM_HELPER, \
				_, \
				BOOST_PP_SEQ_POP_BACK(NAME_SEQUENCE) \
			) :: _ parent_placeholder; \
			static const bchar* base_name(void) \
			{ \
				return BOOST_CTS_STRINGIZE( \
					BOOST_PP_SEQ_HEAD( \
						BOOST_PP_SEQ_REVERSE(NAME_SEQUENCE) \
					) \
				); \
			} \
			typedef ::boost::mpl::int_< \
				BOOST_STR_LIT_LENGTH( \
					BOOST_CTS_STRINGIZE( \
						BOOST_PP_SEQ_HEAD( \
							BOOST_PP_SEQ_REVERSE(NAME_SEQUENCE) \
						) \
					) \
				) \
			>::type base_name_length; \
		}; \
	BOOST_PP_SEQ_FOR_EACH(BOOST_MIRROR_REG_NAMESPACE_EPILOGUE_HELPER, _, NAME_SEQUENCE) \
	}


/** Register some of the common namespaces
 */
BOOST_MIRROR_REG_NAMESPACE((std))
BOOST_MIRROR_REG_NAMESPACE((boost))
BOOST_MIRROR_REG_NAMESPACE((boost)(mirror))

} // namespace mirror
} // namespace boost

#endif //include guard

