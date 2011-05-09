
#if !BOOST_PP_IS_ITERATING
#   ifndef BOOST_LOCAL_FUNCTION_OVERLOAD_HPP_
#       define BOOST_LOCAL_FUNCTION_OVERLOAD_HPP_

#       include "../config.hpp"
#       include "../aux_/file.hpp"
#       include "../aux_/overload_base.hpp"
#       include <boost/preprocessor/iteration/iterate.hpp>
#       include <boost/preprocessor/repetition/enum.hpp>
#       include <boost/preprocessor/repetition/repeat.hpp>
#       include <boost/preprocessor/control/expr_iif.hpp>
#       include <boost/preprocessor/control/expr_if.hpp>
#       include <boost/preprocessor/comparison/greater.hpp>
#       include <boost/preprocessor/comparison/less.hpp>
#       include <boost/preprocessor/cat.hpp>
#       include <boost/preprocessor/arithmetic/add.hpp>
#       include <boost/preprocessor/tuple/eat.hpp>
#       include <boost/preprocessor/logical/and.hpp>
#       include <boost/preprocessor/logical/not.hpp>
#       include <boost/preprocessor/facilities/expand.hpp>

#define BOOST_LOCAL_f_type(z, f, unused) \
    BOOST_PP_CAT(F, f)

#define BOOST_LOCAL_f_tparam(z, f, unused) \
    typename BOOST_LOCAL_f_type(z, f, unused) \

#define BOOST_LOCAL_f_tparam_dflt(z, f, is_tspec) \
    BOOST_LOCAL_f_tparam(z, f, unused) \
    /* overload requires at least 2 functors so F0 and F1 not optional */ \
    BOOST_PP_EXPR_IIF(BOOST_PP_AND(BOOST_PP_NOT(is_tspec), \
            BOOST_PP_GREATER(f, 1)), \
        = void \
    )

#define BOOST_LOCAL_g_arg_type(z, f, unused) \
    BOOST_PP_CAT(G, f)

#define BOOST_LOCAL_g_arg_name(z, f, unused) \
    BOOST_PP_CAT(g, f)

#define BOOST_LOCAL_g_arg_tparam(z, f, unused) \
    typename BOOST_LOCAL_g_arg_type(z, f, unused)

#define BOOST_LOCAL_g_arg(z, f, unused) \
    /* unfortunately, cannot add const and/or & (not even using */ \
    /* Boost.TypeTraits or Boost.CallTraits) to this function argument */ \
    /* type which needs to remain generic as in its template declaration */ \
    /* (otherwise MSVC cannot deduce the types */ \
    BOOST_LOCAL_g_arg_type(z, f, unused) \
    BOOST_LOCAL_g_arg_name(z, f, unsed)

#define BOOST_LOCAL_overload_base(z, f, unused) \
    ::boost::local::aux::overload_base<BOOST_LOCAL_f_type(z, f, unused)>

#define BOOST_LOCAL_overload_inherit(z, f, unused) \
    public BOOST_LOCAL_overload_base(z, f, unused)

#define BOOST_LOCAL_overload_base_init(z, f, unused) \
    BOOST_LOCAL_overload_base(z, f, unused)( /* base init paren `()` */ \
            BOOST_LOCAL_g_arg_name(z, f, unused))

#define BOOST_LOCAL_using_operator_call(z, f, unused) \
    using BOOST_LOCAL_overload_base(z, f, unused)::operator();

namespace boost { namespace local { namespace function {

// Iterate within namespace.
#       define BOOST_PP_ITERATION_PARAMS_1 \
                /* need at least 2 functors to overload so iter 2, 3, ... */ \
                (3, (0, BOOST_PP_SUB(BOOST_LOCAL_CONFIG_OVERLOADS, 2), \
                BOOST_LOCAL_AUX_FILE_FUNCTION_OVERLOAD_HPP))
#       include BOOST_PP_ITERATE() // Iterate over function arity.

}}} // namespace boost::local::function

#   endif // #include guard

#elif BOOST_PP_ITERATION_DEPTH() == 1
#   define BOOST_LOCAL_overloads \
        /* iterate as OVERLOADS, OVERLOADS-1, OVERLOADS-2, ... */ \
        BOOST_PP_SUB(BOOST_LOCAL_CONFIG_OVERLOADS, BOOST_PP_FRAME_ITERATION(1))
#   define BOOST_LOCAL_is_tspec \
        /* if template specialization */ \
        BOOST_PP_LESS(BOOST_LOCAL_overloads, BOOST_LOCAL_CONFIG_OVERLOADS)

// Iterating within namespace boost::local::function.
template<BOOST_PP_ENUM(BOOST_LOCAL_overloads, BOOST_LOCAL_f_tparam_dflt,
        BOOST_LOCAL_is_tspec)>
class overload
    // Template specialization.
    BOOST_PP_EXPR_IIF(BOOST_PP_EXPAND(BOOST_LOCAL_is_tspec), <)
    BOOST_PP_IIF(BOOST_LOCAL_is_tspec,
        BOOST_PP_ENUM
    ,
        BOOST_PP_TUPLE_EAT(3)
    )(BOOST_LOCAL_overloads, BOOST_LOCAL_f_type, ~)
    BOOST_PP_EXPR_IIF(BOOST_PP_EXPAND(BOOST_LOCAL_is_tspec), >)
    // Bases.
    : // Overloads >= 2 so always at least 2 bases.
    BOOST_PP_ENUM(BOOST_LOCAL_overloads, BOOST_LOCAL_overload_inherit, ~)
{
public:
    template<BOOST_PP_ENUM(BOOST_LOCAL_overloads, BOOST_LOCAL_g_arg_tparam, ~)>
    /* implicit */ inline overload(
            BOOST_PP_ENUM(BOOST_LOCAL_overloads, BOOST_LOCAL_g_arg, ~))
            : // Overloads >= 2 so always at least 2 bases to initialize.
            BOOST_PP_ENUM(BOOST_LOCAL_overloads,
                    BOOST_LOCAL_overload_base_init, ~)
    {}

    BOOST_PP_REPEAT(BOOST_LOCAL_overloads, BOOST_LOCAL_using_operator_call, ~)
};

#   undef BOOST_LOCAL_is_tspec
#   undef BOOST_LOCAL_AUX_overloads
#endif // iteration

