// Macros defined:
// everitying intended to work with _SHIFTED has _SHIFTED as suffix

// Number of arguments
#define BOOST_ARITY_NUM_ARGS BOOST_PP_ITERATION()
#define BOOST_ARITY_NUM_ARGS_SHIFTED BOOST_PP_INC(BOOST_ARITY_NUM_ARGS)
// deprecated form
#define BOOST_ARITY_NUM_ARGS_INC BOOST_PP_INC(BOOST_ARITY_NUM_ARGS)

// helper macros
// Concatenate Prefix, Insertion, and Suffix
#define BOOST_ARITY_INSERT(P,I,S) BOOST_PP_CAT(P,BOOST_PP_CAT(I,S))
#define BOOST_ARITY_COMMA BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,BOOST_PP_COMMA,BOOST_PP_EMPTY)()
#define BOOST_ARITY_COLON BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,:,BOOST_PP_EMPTY())

// for a variable/argument
// BOOST_ARITY_VAR_[name]_NAME(n) - name of the n^th variable
#define BOOST_ARITY_VAR_aN_NAME(n) BOOST_PP_CAT(a,BOOST_PP_INC(n))

// for a type
// BOOST_ARITY_TYPE_[name]_NAME(n) - name of the n^th type
// BOOST_ARITY_TYPE_[name]_TYPEDEF(z,n,newtype)
//  - expands to typedef BOOST_ARITY_TYPE_[name]_NAME(n) newtype(n);
// BOOST_ARITY_TYPE_[name]_VARIABLE(z,n,variable)
//  - expands to BOOST_ARITY_TYPE_[name]_NAME(n) variable(n);
// BOOST_ARITY_TYPE_[name]_ARGUMENT(z,n,variable)
#define BOOST_ARITY_TYPE_argN_type_NAME(z,n,data) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_type)
#define BOOST_ARITY_TYPE_argN_type_TYPEDEF(z,n,newtype) \
    typedef BOOST_ARITY_TYPE_argN_type_TYPEDEF(n) newtype(n);
#define BOOST_ARITY_TYPE_argN_type_VARIABLE(z,n,variable) \
    BOOST_ARITY_TYPE_argN_type_NAME(n) variable(n);
#define BOOST_ARITY_TYPE_argN_type_ARGUMENT(z,n,argument) \
    BOOST_ARITY_TYPE_argN_type_NAME(n) variable(n)

// repetition mechanisms
#define BOOST_ARITY_REPEAT(macro,data) BOOST_PP_REPEAT(BOOST_ARITY_NUM_ARGS,macro,data)
#define BOOST_ARITY_ENUM(macro,data) BOOST_PP_ENUM(BOOST_ARITY_NUM_ARGS,macro,data)
#define BOOST_ARITY_ENUM_TRAILING(macro,data) BOOST_PP_ENUM_TRAILING(BOOST_ARITY_NUM_ARGS,macro,data)

// argI_type definitions
#define BOOST_ARITY_ARG_TYPE_NAME(I) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_type)
// argI_storable_type definition
#define BOOST_ARITY_ARG_STORABLE_NAME(I) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_storable_type)

#define BOOST_ARITY_TYPE(z,I,signature) \
    typedef typename boost::function_traits<signature>::BOOST_ARITY_ARG_TYPE_NAME(I) \
    BOOST_ARITY_ARG_TYPE_NAME(I);
#define BOOST_ARITY_TYPES(signature) \
    BOOST_PP_REPEAT(BOOST_ARITY_NUM_ARGS,BOOST_ARITY_TYPE,signature)

#define BOOST_ARITY_TYPEDEF(oldtype,newtype) \
    BOOST_PP_REPEAT(BOOST_ARITY_NUM_ARGS,

#define BOOST_ARITY_STORABLE_TYPE(z,I,signature) \
    typedef typename boost::detail::storable<typename boost::function_traits<signature>::BOOST_ARITY_ARG_TYPE_NAME(I)>::type \
    BOOST_ARITY_ARG_STORABLE_NAME(I);
#define BOOST_ARITY_STORABLE_TYPES(signature) \
    BOOST_PP_REPEAT(BOOST_ARITY_NUM_ARGS,BOOST_ARITY_STORABLE_TYPE,signature)

#define BOOST_ARITY_STORABLE(z,n,text) arg##n##_storable_type arg_storable##n;
#define BOOST_ARITY_STORABLES(n) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_INC(n),BOOST_ARITY_STORABLE,_)

#define BOOST_ARITY_STORABLE_RESULT_TYPE typename boost::detail::storable< \
    typename boost::function_traits<Signature>::result_type>::type
#define BOOST_ARITY_ON_STORABLE(z,n,prefix) prefix arg_storable##n;
#define BOOST_ARITY_ON_STORABLES(n,prefix) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_INC(n),BOOST_ARITY_ON_STORABLE,prefix)

#define BOOST_ARITY_TYPE_PARM(J,I,D) BOOST_ARITY_ARG_TYPE_NAME(I) BOOST_PP_CAT(a,BOOST_PP_INC(I))
#define BOOST_ARITY_TYPE_PARMS BOOST_PP_ENUM(BOOST_ARITY_NUM_ARGS,BOOST_ARITY_TYPE_PARM,BOOST_PP_EMPTY)

#define BOOST_ARITY_TYPE_PARM_LONG(J,I,signature) \
    typename boost::function_traits<signature>::BOOST_ARITY_ARG_TYPE_NAME(I) BOOST_PP_CAT(a,BOOST_PP_INC(I))
#define BOOST_ARITY_TYPE_PARMS_LONG(signature) BOOST_PP_ENUM(BOOST_ARITY_NUM_ARGS,BOOST_ARITY_TYPE_PARM_LONG,signature)

#define BOOST_ARITY_ON_PARM(z,n,prefix) prefix a##n;
#define BOOST_ARITY_ON_PARMS(n,prefix) BOOST_PP_REPEAT_FROM_TO(1,BOOST_PP_INC(n),BOOST_ARITY_ON_PARM,prefix)
