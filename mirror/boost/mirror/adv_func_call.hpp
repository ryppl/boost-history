/**
 * \file boost/mirror/adv_func_call.hpp
 *
 *  Advanced meta-function-based functor callers
 *  
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ADV_FUNC_CALL_HPP
#define BOOST_MIRROR_ADV_FUNC_CALL_HPP

#include <boost/mirror/functor_call.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

#include <boost/mpl/max_element.hpp>

namespace boost {
namespace mirror {
namespace detail {

#define BOOST_MIRROR_DETAIL_IMPLEMENT_SELECT_NTH_N(R, N, PARAM_COUNT) \
template < BOOST_PP_ENUM_PARAMS(PARAM_COUNT, typename T) > \
inline const BOOST_PP_CAT(T, BOOST_PP_DEC(N)) & select_nth( \
	mpl::int_< N > n, \
	BOOST_PP_ENUM_BINARY_PARAMS( \
		PARAM_COUNT, \
		const T, \
		& p \
	) \
){return BOOST_PP_CAT(p, BOOST_PP_DEC(N));}


#define BOOST_MIRROR_DETAIL_IMPLEMENT_SELECT_NTH(R, PARAM_COUNT, _) \
BOOST_PP_REPEAT_FROM_TO( \
	1, \
	BOOST_PP_INC(PARAM_COUNT), \
	BOOST_MIRROR_DETAIL_IMPLEMENT_SELECT_NTH_N, \
	PARAM_COUNT\
)

BOOST_PP_REPEAT(
	BOOST_MIRROR_MAX_FUNC_PARAMS(),
	BOOST_MIRROR_DETAIL_IMPLEMENT_SELECT_NTH,
	_
)

#undef BOOST_MIRROR_DETAIL_IMPLEMENT_SELECT_NTH
#undef BOOST_MIRROR_DETAIL_IMPLEMENT_SELECT_NTH_N

template <
	template <class> class Manufacturer,
	class MetaFunctions,
	class FunctionIndex,
	class ParamIndex,
	class ParamMapping,
	class Arity,
	class ConstructedParam
> struct param_source_switch;


#define BOOST_MIRROR_IMPLEMENT_PARAM_SOURCE_SWITCH_BASE( \
	Z, ARITY, DATA \
) \
template < \
	template <class> class Manufacturer, \
	class MetaFunctions, \
	class FunctionIndex, \
	class ParamIndex, \
	class ParamMapping \
> struct param_source_switch< \
	Manufacturer, \
	MetaFunctions, \
	FunctionIndex, \
	ParamIndex, \
	ParamMapping, \
	mpl::int_< ARITY >, \
	mpl::true_ \
> \
{ \
	typedef typename constructor_utils::template adjust_product< \
	        typename MetaFunctions:: \
		template function<FunctionIndex>::params:: \
	        template param<ParamIndex>:: \
	        type::reflected_type \
	>::type T; \
	typedef Manufacturer< T > manufacturer; \
	manufacturer m; \
	\
	template <typename FactoryParam> \
	inline param_source_switch(FactoryParam factory_param) \
	 : m(factory_param, MetaFunctions(), FunctionIndex(), ParamIndex()) \
	{ } \
	\
	template <typename FactoryParam> \
	inline param_source_switch( \
		FactoryParam factory_param, \
		const T* opt_def \
	): m(factory_param, MetaFunctions(), FunctionIndex(), ParamIndex(), opt_def) \
	{ } \
	\
	template < BOOST_PP_ENUM_PARAMS(ARITY, typename T) > \
	inline T operator()(BOOST_PP_ENUM_PARAMS(ARITY, T)) \
	{ \
		return m(); \
	} \
};

BOOST_PP_REPEAT_FROM_TO(
        1, BOOST_MIRROR_MAX_FUNC_PARAMS(),
        BOOST_MIRROR_IMPLEMENT_PARAM_SOURCE_SWITCH_BASE,
        _
)

#undef BOOST_MIRROR_IMPLEMENT_PARAM_SOURCE_SWITCH_BASE


#define BOOST_MIRROR_IMPLEMENT_PARAM_SOURCE_SWITCH_BASE( \
	Z, ARITY, DATA \
) \
template < \
	template <class> class Manufacturer, \
	class MetaFunctions, \
	class FunctionIndex, \
	class ParamIndex, \
	class ParamMapping \
> struct param_source_switch< \
	Manufacturer, \
	MetaFunctions, \
	FunctionIndex, \
	ParamIndex, \
	ParamMapping, \
	mpl::int_< ARITY >, \
	mpl::false_ \
> \
{ \
	typedef typename constructor_utils::template adjust_product< \
	        typename MetaFunctions:: \
		template function<FunctionIndex>::params:: \
	        template param<ParamIndex>:: \
	        type::reflected_type \
	>::type T; \
	\
	template <typename FactoryParam> \
	inline param_source_switch(FactoryParam) \
	{ } \
	\
	template <typename FactoryParam> \
	inline param_source_switch(FactoryParam, const void*) \
	{ } \
	\
	template < BOOST_PP_ENUM_PARAMS(ARITY, typename T) > \
	inline T operator()( \
		BOOST_PP_ENUM_BINARY_PARAMS(ARITY, const T, & p) \
	) \
	{ \
		return select_nth( \
			mpl::int_< \
				mpl::at<ParamMapping, ParamIndex>::type::value \
			>(), \
			BOOST_PP_ENUM_PARAMS(ARITY, p) \
		); \
	} \
};

BOOST_PP_REPEAT_FROM_TO(
        1, BOOST_MIRROR_MAX_FUNC_PARAMS(),
        BOOST_MIRROR_IMPLEMENT_PARAM_SOURCE_SWITCH_BASE,
        _
)

#undef BOOST_MIRROR_IMPLEMENT_PARAM_SOURCE_SWITCH_BASE


template <
	template <class> class Manufacturer,
	class MetaFunctions,
	class FunctionIndex,
	class ParamMapping,
	class ParamCount,
	class Arity
> struct advanced_functor_caller;

#define BOOST_MIRROR_ADV_FUNC_CALLER_DECLARE_PARAM_SOURCE( \
	Z, PARAM_INDEX, ARITY \
) typedef param_source_switch< \
	Manufacturer, \
	MetaFunctions, \
	FunctionIndex, \
	mpl::int_< PARAM_INDEX >, \
	ParamMapping, \
	mpl::int_< ARITY >, \
	typename mpl::less< \
		typename mpl::at< \
			ParamMapping, \
			mpl::int_< PARAM_INDEX > \
		>::type, \
		mpl::int_< 1 > \
	>::type \
> BOOST_PP_CAT(src_, PARAM_INDEX); \
BOOST_PP_CAT(src_, PARAM_INDEX) BOOST_PP_CAT(_, PARAM_INDEX);

#define BOOST_MIRROR_ADV_FUNC_CALLER_INITIALIZE_PARAM_SOURCE( \
	Z, PARAM_INDEX, _ \
) , BOOST_PP_CAT(_, PARAM_INDEX)(factory_param)

#define BOOST_MIRROR_ADV_FUNC_CALLER_INITIALIZE_PARAM_SOURCE_W_DEF( \
	Z, PARAM_INDEX, _ \
) , BOOST_PP_CAT(_, PARAM_INDEX)( \
	factory_param, \
	& BOOST_PP_CAT(def_value_, PARAM_INDEX) \
)

#define BOOST_MIRROR_ADV_FUNC_CALLER_DECL_DEFAULT_PARAM(Z, INDEX, X) \
        , typename ::boost::call_traits< \
		typename BOOST_PP_CAT(src_, INDEX) ::T \
	>::param_type \
        BOOST_PP_CAT(def_value_, INDEX)


#define BOOST_MIRROR_ADV_FUNC_CALLER_CALL_PARAM_SOURCE( \
	Z, PARAM_INDEX, CALL_PARAMS \
) BOOST_PP_CAT(_, PARAM_INDEX) CALL_PARAMS


#define BOOST_MIRROR_DETAIL_IMPLEMENT_ADVANCED_FUNCTOR_CALLER_1( \
	Z, ARITY, PARAM_COUNT \
) \
template < \
	template <class> class Manufacturer, \
	class MetaFunctions, \
	class FunctionIndex, \
	class ParamMapping \
> struct advanced_functor_caller< \
	Manufacturer, \
	MetaFunctions, \
	FunctionIndex, \
	ParamMapping, \
	mpl::int_<PARAM_COUNT>, \
	mpl::int_< ARITY > \
> \
{ \
protected: \
	struct { } _dummy; \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_ADV_FUNC_CALLER_DECLARE_PARAM_SOURCE, \
		ARITY \
	) \
	\
	template <class FactoryParam> \
	inline advanced_functor_caller(FactoryParam factory_param) \
	 : _dummy() \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_ADV_FUNC_CALLER_INITIALIZE_PARAM_SOURCE, \
		_ \
	){ } \
	\
	template <class FactoryParam> \
	inline advanced_functor_caller( \
		FactoryParam factory_param \
		BOOST_PP_REPEAT( \
			PARAM_COUNT, \
			BOOST_MIRROR_ADV_FUNC_CALLER_DECL_DEFAULT_PARAM, \
			_ \
		) \
	): _dummy() \
	BOOST_PP_REPEAT( \
		PARAM_COUNT, \
		BOOST_MIRROR_ADV_FUNC_CALLER_INITIALIZE_PARAM_SOURCE_W_DEF, \
		_ \
	){ } \
	\
	typedef typename MetaFunctions::template function<FunctionIndex> \
		_meta_function; \
	\
	typedef typename _meta_function::result_type::reflected_type \
		result_type;\
	\
	template <class Functor, BOOST_PP_ENUM_PARAMS(ARITY, typename T) > \
	inline result_type call( \
		Functor func, \
		BOOST_PP_ENUM_BINARY_PARAMS( \
			ARITY, \
			const T, \
			& p \
		) \
	) \
	{ \
		return func( \
			BOOST_PP_ENUM( \
				PARAM_COUNT, \
				BOOST_MIRROR_ADV_FUNC_CALLER_CALL_PARAM_SOURCE,\
				(BOOST_PP_ENUM_PARAMS(ARITY, p))\
			) \
		); \
	} \
public: \
	template <BOOST_PP_ENUM_PARAMS(ARITY, typename T) > \
        inline result_type operator()( \
		BOOST_PP_ENUM_BINARY_PARAMS(ARITY, const T, & p) \
	) \
	{ \
		return call( \
			_meta_function::wrap(), \
			BOOST_PP_ENUM_PARAMS(ARITY, p) \
		); \
	} \
	\
	template <class Class, BOOST_PP_ENUM_PARAMS(ARITY, typename T) > \
        inline result_type operator()( \
		Class& instance, \
		BOOST_PP_ENUM_BINARY_PARAMS(ARITY, const T, & p) \
	) \
	{ \
		return call( \
			_meta_function::wrap(instance), \
			BOOST_PP_ENUM_PARAMS(ARITY, p) \
		); \
	} \
};

#define BOOST_MIRROR_DETAIL_IMPLEMENT_ADVANCED_FUNCTOR_CALLER_2( \
	Z, PARAM_COUNT, _ \
) BOOST_PP_REPEAT_FROM_TO( \
	1, PARAM_COUNT, \
	BOOST_MIRROR_DETAIL_IMPLEMENT_ADVANCED_FUNCTOR_CALLER_1, \
	PARAM_COUNT \
)

BOOST_PP_REPEAT(
	BOOST_MIRROR_MAX_FUNC_PARAMS(),
	BOOST_MIRROR_DETAIL_IMPLEMENT_ADVANCED_FUNCTOR_CALLER_2,
	_
)

#undef BOOST_MIRROR_ADV_FUNC_CALLER_DECL_DEFAULT_PARAM
#undef BOOST_MIRROR_ADV_FUNC_CALLER_INITIALIZE_PARAM_SOURCE_W_DEF
#undef BOOST_MIRROR_ADV_FUNC_CALLER_INITIALIZE_PARAM_SOURCE
#undef BOOST_MIRROR_ADV_FUNC_CALLER_DECLARE_PARAM_SOURCE
#undef BOOST_MIRROR_DETAIL_IMPLEMENT_ADVANCED_FUNCTOR_CALLER_2
#undef BOOST_MIRROR_DETAIL_IMPLEMENT_ADVANCED_FUNCTOR_CALLER_1

} // namespace detail

template <
	template <class> class Manufacturer,
	class MetaFunctions,
	class FunctionIndex,
	class ParamMapping
> struct advanced_functor_caller : public detail::advanced_functor_caller<
	Manufacturer, 
	MetaFunctions,
	FunctionIndex,
	ParamMapping,
	typename MetaFunctions::
	template function<FunctionIndex>::
	params::size,
	mpl::int_< 
		mpl::deref<
			typename mpl::max_element<ParamMapping>::type
		>::type::value 
	>
>
{
private:
	typedef detail::advanced_functor_caller<
	        Manufacturer,
	        MetaFunctions,
	        FunctionIndex,
	        ParamMapping,
	        typename MetaFunctions::
	        template function<FunctionIndex>::
	        params::size,
		mpl::int_< 
			mpl::deref<
				typename mpl::max_element<ParamMapping>::type
			>::type::value 
		>
	> base_class;
public:
	inline advanced_functor_caller(void)
	 : base_class(0)
	{ }

	template <class FactoryParam>
	inline advanced_functor_caller(FactoryParam factory_param)
	 : base_class(factory_param)
	{ }

#ifndef BOOST_NO_VARIADIC_TEMPLATES
        template <class FactoryParam, class ...Defaults>
	advanced_functor_caller(FactoryParam factory_param, Defaults ...defs)
         : base_class(factory_param, defs...)
        { }
#endif

};


/** A more convenient interface for calling constructors
 */ 
template <
        template <class> class Manufacturer,
        class Class,
        int FuncIndex,
	class ParamMapping_c
> struct advanced_constructor_caller : public advanced_functor_caller<
        Manufacturer,
        meta_constructors<Class>,
        mpl::int_<FuncIndex>,
	typename ParamMapping_c::type
>
{
private:
	typedef advanced_functor_caller<
	        Manufacturer,
	        meta_constructors<Class>,
	        mpl::int_<FuncIndex>,
	        typename ParamMapping_c::type
	> base_class;
public:
	inline advanced_constructor_caller(void)
	 : base_class(0)
	{ }

	template <class FactoryParam>
	inline advanced_constructor_caller(FactoryParam factory_param)
	 : base_class(factory_param)
	{ }

#ifndef BOOST_NO_VARIADIC_TEMPLATES
        template <class FactoryParam, class ...Defaults>
        inline advanced_constructor_caller(
                FactoryParam factory_param,
                Defaults ...defs
        ): base_class(factory_param, defs...)
        { }
#endif
};

/** A more convenient interface for calling member functions 
 */ 
template <
        template <class> class Manufacturer,
        class Class,
        int FuncIndex,
	class ParamMapping_c
> struct advanced_mem_func_caller : public advanced_functor_caller<
        Manufacturer,
        meta_member_functions<Class>,
        mpl::int_<FuncIndex>,
	typename ParamMapping_c::type
>
{
private:
	typedef advanced_functor_caller<
	        Manufacturer,
	        meta_member_functions<Class>,
	        mpl::int_<FuncIndex>,
	        typename ParamMapping_c::type
	> base_class;
public:
	inline advanced_mem_func_caller(void)
	 : base_class(0)
	{ }

	template <class FactoryParam>
	inline advanced_mem_func_caller(FactoryParam factory_param)
	 : base_class(factory_param)
	{ }

#ifndef BOOST_NO_VARIADIC_TEMPLATES
        template <class FactoryParam, class ...Defaults>
        inline advanced_mem_func_caller(
                FactoryParam factory_param,
                Defaults ...defs
        ): base_class(factory_param, defs...)
        { }
#endif

};


} // namespace mirror
} // namespace boost

#endif //include guard

