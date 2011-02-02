//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_less_equal_HPP_INCLUDED
#define BOOST_TT_less_equal_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_operator_less_equal
#define BOOST_TT_TRAIT_OP <=
#define BOOST_TT_DEFAULT_RET void
#define BOOST_TT_FORBIDDEN_IF\
	(\
		/* one pointer and one non pointer built-in */\
		(\
			::boost::is_pointer< typename ::boost::remove_reference<LHS>::type >::value\
			and\
			not (\
				::boost::is_class  < typename ::boost::remove_reference<RHS>::type >::value or\
				::boost::is_union  < typename ::boost::remove_reference<RHS>::type >::value or\
				::boost::is_pointer< typename ::boost::remove_reference<RHS>::type >::value\
			)\
		) or (\
			::boost::is_pointer< typename ::boost::remove_reference<RHS>::type >::value\
			and\
			not (\
				::boost::is_class  < typename ::boost::remove_reference<LHS>::type >::value or\
				::boost::is_union  < typename ::boost::remove_reference<LHS>::type >::value or\
				::boost::is_pointer< typename ::boost::remove_reference<LHS>::type >::value\
			)\
		)\
		or\
		/* two pointers but no inheritance */\
		(\
			(\
				::boost::is_pointer< typename ::boost::remove_reference<LHS>::type >::value and\
				::boost::is_pointer< typename ::boost::remove_reference<RHS>::type >::value\
			) and not (\
				::boost::is_base_of< typename ::boost::remove_reference< typename ::boost::remove_pointer<LHS>::type >::type, typename ::boost::remove_reference< typename ::boost::remove_pointer<RHS>::type >::type >::value or\
				::boost::is_base_of< typename ::boost::remove_reference< typename ::boost::remove_pointer<RHS>::type >::type, typename ::boost::remove_reference< typename ::boost::remove_pointer<LHS>::type >::type >::value or\
				::boost::is_void< typename ::boost::remove_pointer<LHS>::type >::value or\
				::boost::is_void< typename ::boost::remove_pointer<RHS>::type >::value\
			)\
		)\
	)


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_DEFAULT_RET
#undef BOOST_TT_FORBIDDEN_IF

#endif
