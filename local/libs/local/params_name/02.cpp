
#include "aux_/preprocessor/sign/params_/is_void.hpp"
#include "aux_/preprocessor/keyword/bind.hpp"
#include "aux_/preprocessor/keyword/const_bind.hpp"
#include <boost/detail/preprocessor/keyword/default.hpp>
#include <boost/detail/preprocessor/keyword/this.hpp>
#include <boost/preprocessor.hpp>

// Parameter (unbound, const-bound, and bound) tuple internal representation:
//  ( // Unbound params. 
//    ((NIL, NIL)) (([auto | register] type name, default_value)) ...
//  , (NIL) ([&] const_bind_name) ...   // Const bound params.
//  , const_bind_this_count             // Number of const bound `this`.
//  , (NIL) ([&] bind_name) ...         // Bound params (not const).
//  , bind_this_count                   // Number of bound `this` (not const).
//  )

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_                    0
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_                1
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_     2
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_                      3
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_           4
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_                     5
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_                       6
            
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
            params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND_THIS_COUNT(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
            params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
            params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_NIL \
    ( \
        /* unbound params: nil-seq of 2-tuples */ \
        /* `((NIL, NIL)) ((classifier_type_name, default)) ...` */ \
        ( (BOOST_LOCAL_AUX_PP_NIL, BOOST_LOCAL_AUX_PP_NIL) ) \
    , \
        /* const-binds: nil-seq of 1-tuple `(NIL) (ref_name) ...` */ \
        (BOOST_LOCAL_AUX_PP_NIL) \
    , \
        0 /* number of const-bind `this` */ \
    , \
        /* binds: nil-seq of 1-tuple `(NIL) (ref_name) ...` */ \
        (BOOST_LOCAL_AUX_PP_NIL) \
    , \
        0 /* number of bind `this` */ \
    , \
        BOOST_PP_EMPTY /* no error */ \
    )

// unbind_classified_type_and_name: [auto | register] type name
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_UNBIND( \
        params, unbind_classified_type_and_name) \
    ( /* unbind params and defaults */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                params) \
        ((unbind_classified_type_and_name, BOOST_PP_EMPTY))\
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
    , /* const-bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
    , /* bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params) \
    , /* error message (if any) */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
                params) \
    ) 

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_UNBIND_DEFAULT_( \
        unbind_nilseq, default_value) \
    BOOST_PP_SEQ_POP_BACK(unbind_nilseq) \
    (( \
        /* last appended classifier type and name */ \
        BOOST_PP_TUPLE_ELEM(2, 0, BOOST_PP_SEQ_ELEM(BOOST_PP_DEC( \
                BOOST_PP_SEQ_SIZE(unbind_nilseq)), unbind_nilseq)) \
    , \
        /* trailing empty because defaults are optional */ \
        default_value BOOST_PP_EMPTY \
    ))

// default_value: a valid parameter default value (`-1`, etc)
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_UNBIND_DEFAULT( \
        params, default_value) \
    ( /* unbind params and defaults */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_UNBIND_DEFAULT_( \
                BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                        params), \
                default_value) \
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
    , /* const-bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
    , /* bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params) \
    , /* error message (if any) */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
                params) \
    ) 

// const_bind_qualified_name: [&] name (with name != `this`)
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_CONST_BIND( \
        params, const_bind_qualified_name) \
    ( /* unbind params and defaults */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                params) \
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
        (const_bind_qualified_name) \
    , /* const-bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
    , /* bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params) \
    , /* error message (if any) */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
                params) \
    ) 

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INC_CONST_BIND_THIS_COUNT(params) \
    ( /* unbind params and defaults */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                params) \
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
    , /* const-bind `this` count */ \
        BOOST_PP_INC(BOOST_PP_TUPLE_ELEM( \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params)) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
    , /* bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params) \
    , /* error message (if any) */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
                params) \
    ) 

// bind_qualified_name: [&] name
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_BIND( \
        params, bind_qualified_name) \
    ( /* unbind params and defaults */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                params) \
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
    , /* const-bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
        (bind_qualified_name) \
    , /* bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params) \
    , /* error message (if any) */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
                params) \
    ) 

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INC_BIND_THIS_COUNT(params) \
    ( /* unbind params and defaults */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                params) \
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
    , /* const-bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
    , /* bind `this` count */ \
        BOOST_PP_INC(BOOST_PP_TUPLE_ELEM( \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params)) \
    , /* error message (if any) */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_ERROR_, \
                params) \
    ) 

// error: `EMPTY` if no error, `ERROR_message_text EMPTY` if error.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SET_ERROR(params, error) \
    ( /* unbind params and defaults */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_UNBIND_, \
                params) \
    , /* const-bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_, \
                params) \
    , /* const-bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_CONST_BIND_THIS_COUNT_, \
                params) \
    , /* bind names */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_, \
                params) \
    , /* bind `this` count */ \
        BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_MAX_, \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INDEX_BIND_THIS_COUNT_, \
                params) \
    , /* error message (if any) */ \
        error \
    ) 

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_UNBIND_DEFAULT_( \
        params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_UNBIND_DEFAULT(params, \
            BOOST_DETAIL_PP_KEYWORD_DEFAULT_REMOVE_FRONT(elem))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_UNBIND_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_UNBIND(params, elem)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_CONST_BIND_THIS_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INC_CONST_BIND_THIS_COUNT(params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_CONST_BIND_VAR_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_CONST_BIND(params, \
            BOOST_LOCAL_AUX_PP_KEYWORD_CONST_BIND_REMOVE_FRONT(elem))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_CONST_BIND_(params, elem) \
    /* check from back because non `this` bounds might have `&` in front */ \
    BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK( \
            BOOST_LOCAL_AUX_PP_KEYWORD_CONST_BIND_REMOVE_FRONT(elem)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_CONST_BIND_THIS_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_CONST_BIND_VAR_ \
    )(params, elem)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_BIND_VAR_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_APPEND_BIND(params, \
            BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(elem))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_BIND_THIS_(params, elem) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_INC_BIND_THIS_COUNT(params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_BIND_(params, elem) \
    /* check from back because non `this` bounds might have `&` in front */ \
    BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_THIS_BACK( \
            BOOST_LOCAL_AUX_PP_KEYWORD_BIND_REMOVE_FRONT(elem)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_BIND_THIS_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_BIND_VAR_ \
    )(params, elem)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_(s, params, elem) \
    BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_DEFAULT_FRONT(elem), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_UNBIND_DEFAULT_ \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_KEYWORD_IS_CONST_BIND_FRONT(elem), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_CONST_BIND_ \
    , BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_KEYWORD_IS_BIND_FRONT(elem), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_BIND_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_UNBIND_ \
    )))(params, elem)

// Precondition: If seq contains a default param value `... (default ...) ...`,
// the default value element is never 1st (it always has a previous elem) and
// its previous element is a unbind param (no const-bind and no bind).
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALID_(params_seq) \
    BOOST_PP_SEQ_FOLD_LEFT(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ELEM_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_NIL, params_seq)

////

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_UNBIND_( \
        elem) \
    BOOST_PP_NOT(BOOST_PP_BITOR( \
          BOOST_DETAIL_PP_KEYWORD_IS_DEFAULT_FRONT(elem) \
        , BOOST_PP_BITOR( \
            BOOST_LOCAL_AUX_PP_KEYWORD_IS_CONST_BIND_FRONT(elem) \
            , BOOST_LOCAL_AUX_PP_KEYWORD_IS_BIND_FRONT(elem) \
        ) \
    ))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_PREV_( \
        params_seq, index, error) \
    BOOST_PP_IIF( \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_UNBIND_(\
                BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(index), params_seq)), \
        error /* no err, fwd existing one if any */ \
    , \
        BOOST_PP_CAT( \
            ERROR_default_must_follow_an_unbound_parameter_at_element_number_, \
            BOOST_PP_INC(index)) \
    )

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_INDEX_( \
        params_seq, index, error) \
    BOOST_PP_IF(index, \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_PREV_ \
    , \
        ERROR_default_value_cannot_be_specified_as_the_first_element \
        BOOST_PP_TUPLE_EAT(3) \
    )(params_seq, index, error)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_OP_DATA_( \
        params_seq, index, error) \
    ( \
        params_seq \
    , \
        BOOST_PP_INC(index) \
    , \
        BOOST_PP_IIF(BOOST_DETAIL_PP_KEYWORD_IS_DEFAULT_FRONT( \
                BOOST_PP_SEQ_ELEM(index, params_seq)), \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_INDEX_ \
        , \
            error BOOST_PP_TUPLE_EAT(3) /* no err, fwd existing one if any */\
        )(params_seq, index, error) \
    )

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_OP_(d, \
        params_index_error) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_OP_DATA_( \
            BOOST_PP_TUPLE_ELEM(3, 0, params_index_error), \
            BOOST_PP_TUPLE_ELEM(3, 1, params_index_error), \
            BOOST_PP_TUPLE_ELEM(3, 2, params_index_error))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_PRED_DATA_( \
        params_seq, index, error) \
    BOOST_PP_AND( \
          BOOST_PP_IS_EMPTY(error (/* expand empty */) ) \
        , BOOST_PP_LESS(index, BOOST_PP_SEQ_SIZE(params_seq)) \
    )

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_PRED_(d, \
        params_index_error) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_PRED_DATA_( \
            BOOST_PP_TUPLE_ELEM(3, 0, params_index_error), \
            BOOST_PP_TUPLE_ELEM(3, 1, params_index_error), \
            BOOST_PP_TUPLE_ELEM(3, 2, params_index_error))

// Expand to `EMPTY` if no error, or `ERROR_message EMPTY` if error.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_(params_seq) \
    BOOST_PP_TUPLE_ELEM(3, 2, BOOST_PP_WHILE( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_PRED_, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_OP_, \
            (params_seq, 0, BOOST_PP_EMPTY)))

///

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_VALIDATE_THIS_COUNT_(params) \
    BOOST_PP_IIF(BOOST_PP_GREATER(BOOST_PP_ADD( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT(params), \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND_THIS_COUNT(params)), 1), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SET_ERROR \
    , \
        params BOOST_PP_TUPLE_EAT(2) \
    )(params, ERROR_object_this_cannot_be_bound_multiple_times)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_VALIDATE_(params) \
    BOOST_PP_IIF(BOOST_PP_IS_EMPTY( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR(params) \
            (/* expand empty */)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_VALIDATE_THIS_COUNT_ \
    , \
        params BOOST_PP_TUPLE_EAT(1) \
    )(params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_YES_(params_seq, unused) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_VALIDATE_( \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALID_(params_seq))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_NO_(unused, error) \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SET_ERROR( \
                BOOST_LOCAL_AUX_PP_SIGN_PARAMS_NIL, error)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_( \
        params_seq, defaults_error) \
    BOOST_PP_IIF(BOOST_PP_IS_EMPTY(defaults_error (/* expand empty */)), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_YES_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_NO_ \
    )(params_seq, defaults_error)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_(params_seq) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_PARSE_(params_seq, \
            BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_VALIDATE_DEFAULTS_(params_seq))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_EMPTY_(unused) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS_NIL

// `params_seq` is: Empty ``, or `void`, or `(void)` for empty parameter list;
// Or, `{(param_type param_name)[(default default_value)] 
// | (bind_type bind_name}+` for non-empty parameters, where `bind_type` is
// `bind | bind& | const bind | const bind&` and `bind_name` is in scope.
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS(params) \
    BOOST_PP_IIF(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_IS_VOID(params), \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_EMPTY_ \
    , \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_SEQ_ \
    )(params)

#define BOOST_LOCAL_FUNCTION_PARAMS(params) \
    BOOST_LOCAL_AUX_PP_SIGN_PARAMS(params)

/** @todo Add to doc that this can never be bound by ref (as in C++) */

/** @todo Validate params: 
 * 1. const_bind_this_count + bind_this_count > 1 => ERROR_object_this_cannot_be_bound_multiple_times
 * 2. Inspect seq before parsing to make sure that default is never 1st and default always after an unbound param (no const-bind and no bind).
 */

BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )

BOOST_LOCAL_FUNCTION_PARAMS( (default -1) (int x) (double y)(default 1.23)
        (bind a) (bind& b) (const bind c) (const bind this) (const bind& d) )

BOOST_LOCAL_FUNCTION_PARAMS( (int x) (double y)(default 1.23)
        (bind a) (bind& b) (default -1) (const bind this) (const bind& d) )

BOOST_LOCAL_FUNCTION_PARAMS( (bind this) (int x) (bind this)
        (bind a) (const bind this) (const bind& d) )

BOOST_LOCAL_FUNCTION_PARAMS(void)

BOOST_LOCAL_FUNCTION_PARAMS( (void) )

BOOST_LOCAL_FUNCTION_PARAMS()

