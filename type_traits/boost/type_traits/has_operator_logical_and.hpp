//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_logical_and_HPP_INCLUDED
#define BOOST_TT_logical_and_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_operator_logical_and
#define BOOST_TT_TRAIT_OP &&
#define BOOST_TT_DEFAULT_RET void
#define BOOST_TT_FORBIDDEN_IF\
	(\
		/* pointer with built-in non convertible to bool */\
		(\
			boost::is_pointer< typename boost::remove_reference<LHS>::type >::value and\
			not (\
				boost::is_class  < typename boost::remove_reference<RHS>::type >::value or\
				boost::is_union  < typename boost::remove_reference<RHS>::type >::value or\
				boost::is_pointer< typename boost::remove_reference<RHS>::type >::value or\
				boost::is_convertible < typename boost::remove_reference<RHS>::type, bool >::value\
			)\
		) or (\
			boost::is_pointer< typename boost::remove_reference<RHS>::type >::value and\
			not (\
				boost::is_class  < typename boost::remove_reference<LHS>::type >::value or\
				boost::is_union  < typename boost::remove_reference<LHS>::type >::value or\
				boost::is_pointer< typename boost::remove_reference<LHS>::type >::value or\
				boost::is_convertible < typename boost::remove_reference<LHS>::type, bool >::value\
			)\
		)\
	)


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_DEFAULT_RET
#undef BOOST_TT_FORBIDDEN_IF

#endif
