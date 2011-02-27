
#ifndef BOOST_LOCAL_AUX_FUNCTION_CODE_DEDUCE_RESULT_TYPE_HPP_
#define BOOST_LOCAL_AUX_FUNCTION_CODE_DEDUCE_RESULT_TYPE_HPP_

#include "../../symbol.hpp"
#include "../../scope_exit/scope_exit.hpp" // Use this lib's ScopeExit impl.
#include <boost/type_traits.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>

// This must follow the result type.
#define BOOST_LOCAL_AUX_FUNCTION_CODE_DEDUCE_RESULT_TYPE( \
        id, typename_keyword) \
    /* result type here */ (*BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_FUNC(id))(); \
    /* the many tagging, wrapping, etc that follow are taken from */ \
    /* Boost.ScopeExit type deduction mechanism and they are necessary */ \
    /* within template on GCC to work around a compiler internal error */ \
    typedef void (*BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_TAG(id))( \
            int BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_FUNC(id)); \
    typedef BOOST_PP_IIF(BOOST_PP_IS_EMPTY(typename_keyword), \
                BOOST_TYPEOF \
            , \
                BOOST_TYPEOF_TPL \
            )(boost::scope_exit::aux::wrap( \
            boost::scope_exit::aux::deref( \
                    BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_FUNC(id), \
                    (BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_TAG(id))0))) \
            BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_WRAP(id); \
    typedef typename_keyword BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_WRAP(id):: \
            type BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_CAPTURE(id); \
    struct BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_PARAMS(id) { \
        typedef BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_CAPTURE(id) \
                function_ptr_type;\
    }; \
    typedef typename_keyword boost::remove_pointer<typename_keyword \
            BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_PARAMS(id):: \
            function_ptr_type>::type \
            BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_FUNC_TYPE(id); \
    typedef typename_keyword boost::function_traits< \
            BOOST_LOCAL_AUX_SYMBOL_DEDUCE_RESULT_FUNC_TYPE(id)>::result_type \
            BOOST_LOCAL_AUX_SYMBOL_RESULT_TYPE(id);

#endif // #include guard

