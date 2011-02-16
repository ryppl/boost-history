
#ifndef BOOST_LOCAL_AUX_PP_VA_TO_SEQ_HPP_
#define BOOST_LOCAL_AUX_PP_VA_TO_SEQ_HPP_

#include <boost/config.hpp>

#if !defined(BOOST_NO_VARIADIC_MACROS) // If no variadics then no macros.

/** @todo Move used files from VMD library into local/aux_/pp/va/vmd/... */
#include <boost/variadic_macro_data/VariadicMacroData.hpp> // Proposed lib.
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/control/if.hpp>

// Private API.

// Interface to proposed Boost.VariadicMacroData.
#define BOOST_LOCAL_AUX_PP_VA_SIZE_(...) VMD_DATA_SIZE(__VA_ARGS__)
#define BOOST_LOCAL_AUX_PP_VA_TO_SEQ_(...) VMD_DATA_TO_PP_SEQ(__VA_ARGS__)

#define BOOST_LOCAL_AUX_PP_VA_SAME_(...) __VA_ARGS__

#define BOOST_LOCAL_AUX_PP_VA_EAT_(...) /* must expand to nothing */

#define BOOST_LOCAL_AUX_PP_VA_IS_NOT_UNARY_(...) \
    BOOST_PP_NOT(BOOST_PP_IS_UNARY(__VA_ARGS__))

#define BOOST_LOCAL_AUX_PP_IS_VA_(...) \
    BOOST_PP_IF(BOOST_PP_GREATER( /* IIF does not expand on MSVC */ \
            BOOST_LOCAL_AUX_PP_VA_SIZE_(__VA_ARGS__), 1), \
        1 BOOST_LOCAL_AUX_PP_VA_EAT_ \
    , \
        BOOST_LOCAL_AUX_PP_VA_IS_NOT_UNARY_ \
    )(__VA_ARGS__)

// Public API.

#define BOOST_LOCAL_AUX_PP_VA_TO_SEQ(...) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_IS_VA_(__VA_ARGS__), \
        BOOST_LOCAL_AUX_PP_VA_TO_SEQ_ \
    , \
        BOOST_LOCAL_AUX_PP_VA_SAME_ \
    )(__VA_ARGS__)

#endif // BOOST_NO_VARIADIC_MACROS

#endif // #include guard

