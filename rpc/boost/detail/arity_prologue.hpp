// Macros defined:
// All macros which are intended to be used with 0-based counting have _INC appended

// Number of arguments
#define BOOST_ARITY_NUM_ARGS BOOST_PP_ITERATION()
#define BOOST_ARITY_NUM_ARGS_INC BOOST_PP_INC(BOOST_ARITY_NUM_ARGS)

// Concatenate Prefix, Insertion, and Suffix
#define BOOST_ARITY_INSERT(P,I,S) BOOST_PP_CAT(P,BOOST_PP_CAT(I,S))

// argI_type definitions
#define BOOST_ARITY_ARG_TYPE_NAME(I) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_type)
// argI_storable_type definition
#define BOOST_ARITY_ARG_STORABLE_NAME(I) BOOST_ARITY_INSERT(arg,BOOST_PP_INC(I),_storable_type)

#define BOOST_ARITY_TYPE(z,I,signature) \
   typedef typename boost::function_traits<signature>::BOOST_ARITY_ARG_TYPE_NAME(I) \
  BOOST_ARITY_ARG_TYPE_NAME(I);
#define BOOST_ARITY_TYPES(signature) \
 BOOST_PP_REPEAT(BOOST_ARITY_NUM_ARGS,BOOST_ARITY_TYPE,signature)

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

#define BOOST_ARITY_COMMA BOOST_PP_IF(BOOST_ARITY_NUM_ARGS,BOOST_PP_COMMA,BOOST_PP_EMPTY)()