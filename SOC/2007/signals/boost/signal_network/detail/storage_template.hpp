/*
 *  signal_storage_template.h
 *  Utility
 *
 *  Created by Stjepan Rajko on 3/9/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIGNAL_NETWORK_TEMPLATE_BODY

SIGNAL_NETWORK_ARGVAR_TYPEDEFS(SIGNAL_NETWORK_TEMPLATE_ARITY)
BOOST_PP_IF(SIGNAL_NETWORK_TEMPLATE_ARITY,SIGNAL_NETWORK_STORAGE_SIGNAL,BOOST_PP_EMPTY)()
typename SIGNAL_NETWORK_TEMPLATE_BASE::result_type operator()()
{
	SIGNAL_NETWORK_RETURN(_return) SIGNAL_NETWORK_TEMPLATE_BASE::out(SIGNAL_NETWORK_ARGVAR_NAMES(SIGNAL_NETWORK_TEMPLATE_ARITY));
}
BOOST_PP_REPEAT_FROM_TO(1,SIGNAL_NETWORK_TEMPLATE_ARITY_INC,SIGNAL_NETWORK_RETURN_STORED,_)

protected:
SIGNAL_NETWORK_ARGVARS(SIGNAL_NETWORK_TEMPLATE_ARITY)

#else
#define SIGNAL_NETWORK_ARGVAR_REPEAT_TYPEDEF(z,n,text) typedef typename SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE::arg##n##_value_type arg##n##_value_type;
#define SIGNAL_NETWORK_ARGVAR_REPEAT_TYPEDEFS(n) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_ADD(n,1),SIGNAL_NETWORK_ARGVAR_REPEAT_TYPEDEF,_)

public:
	typedef SIGNAL_NETWORK_TEMPLATE_CLASS_IMPL_BASE base_class;
	storage() {}
	BOOST_PP_IF(SIGNAL_NETWORK_TEMPLATE_ARITY,
				storage(SIGNAL_NETWORK_ARGVAR_TYPENAMES(SIGNAL_NETWORK_TEMPLATE_ARITY,typename base_class:: ))
			{operator()(SIGNAL_NETWORK_ARGVAR_NAMES(SIGNAL_NETWORK_TEMPLATE_ARITY));},
		BOOST_PP_EMPTY())

#undef SIGNAL_NETWORK_ARGVAR_REPEAT_TYPEDEF
#undef SIGNAL_NETWORK_ARGVAR_REPEAT_TYPEDEFS
#endif
