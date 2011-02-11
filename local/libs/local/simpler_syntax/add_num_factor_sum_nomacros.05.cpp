
#include <boost/local/function.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/at.hpp>
#include <iostream>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    void // Result type (outside the macro).

    // The PARAMS macro.

    ERROR_missing_result_type_at_line_20 (int num) ;

    // Determine bound parameter types via Boost.Typeof (from Boost.ScopeExit).
    
    typedef void (*boost_se_tag_0_id20)(int factor );
    typedef void (*boost_se_tag_1_id20)(int &sum );
    
    typedef BOOST_TYPEOF(
        boost::type_of::ensure_obj(
            boost::scope_exit::aux::wrap(
                boost::scope_exit::aux::deref(
                      factor
                    , (boost_se_tag_0_id20)0
                )
            )
        )
    ) boost_se_wrapped_t_0_id20; 
    typedef boost_se_wrapped_t_0_id20::type boost_se_capture_t_0_id20; 

    typedef BOOST_TYPEOF(
        boost::type_of::ensure_obj(
            boost::scope_exit::aux::wrap(
                boost::scope_exit::aux::deref(
                      &sum
                    , (boost_se_tag_1_id20)0
                )
            )
        )
    ) boost_se_wrapped_t_1_id20;
    typedef boost_se_wrapped_t_1_id20::type boost_se_capture_t_1_id20;
    
    struct boost_se_params_t_id20 { 
        typedef boost_se_capture_t_0_id20 boost_se_param_t_0_id20; 
        typedef boost_se_capture_t_1_id20 boost_se_param_t_1_id20; 
        boost::scope_exit::aux::member<
              boost_se_param_t_0_id20
            , boost_se_tag_0_id20 
        > boost_se_param_0_id20; 
        boost::scope_exit::aux::member< 
              boost_se_param_t_1_id20
            , boost_se_tag_1_id20 
        > boost_se_param_1_id20; 
    } boost_local_auxXparams_id20 = { 
          { boost::scope_exit::aux::deref(factor, (boost_se_tag_0_id20)0) } 
        , { boost::scope_exit::aux::deref(&sum, (boost_se_tag_1_id20)0) } 
    }; 
    
    boost::scope_exit::aux::declared< 
        boost::scope_exit::aux::resolve< 
            sizeof(boost_local_auxXargs) 
        >::cmp1<0>::cmp2 
    > boost_local_auxXargs;
    boost_local_auxXargs.value = &boost_local_auxXparams_id20;
    
    // Determine unbound parameter types (handle attached param type-name-dflt).
    //      F( (void) (f)(double num = -1.23, int x = 1) )
    // or   F( void, f, (double num = -1.23, int x = 1) )
//#define RESULT_TYPE void
//#define ARG0        double num = -1.23
//    typedef RESULT_TYPE (function_type_id20)(ARG0);
//    typedef boost::function_types::parameter_types<function_type_id20>::type
//            param_types_id20;
//    typedef boost::mpl::at_c<param_types_id20, 0>::type arg_type0_id20;

    typedef boost::function_traits<ERROR_missing_result_type_at_line_20>::
            result_type result_type_id20;

    // Local function.

    struct boost_local_auxXfunctorXid20 : 
            ::boost::local::aux::function_base< function_type_id20, 0> {
        ::boost::local::function_ref< function_type_id20, 0 >*
                deduce_function_ref_type;

        // Generic param names because unseparated pp param type-name.
        void operator()( arg_type0_id20 arg0 ) { 
            return boost_local_auxXbody( 
                  boost_local_auxXparams-> boost_se_param_0_id20.value 
                , boost_local_auxXparams-> boost_se_param_1_id20.value 
                , arg0
            ); 
        } 
    private: 
        boost_se_params_t_id20* boost_local_auxXparams;
        // Ideally static so can't use `this` instead of `this_` but then can't
        // access `add` member var for recursion...
        /*static*/ void boost_local_auxXbody( 
              ::boost::add_const< 
                  boost_se_params_t_id20:: boost_se_param_t_0_id20 
              >::type factor 
            , boost_se_params_t_id20:: boost_se_param_t_1_id20 &sum 
            , int num // Param type-name.
    ) { // The body.
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
        add(1); // Recursive call.
    }
    
    // The END macro (the function name `add` available only here).
    
        boost_local_auxXfunctorXid20& add; // For recursive calls.
    public:
        explicit boost_local_auxXfunctorXid20(void* bind_params) :
                  boost_local_auxXparams( (boost_se_params_t_id20*) bind_params)
                , add(*this)
                {} 
    } boost_local_auxXobjectXadd(boost_local_auxXargs.value); 
    BOOST_TYPEOF(*(boost_local_auxXobjectXadd.deduce_function_ref_type))
            add(boost_local_auxXobjectXadd);

    // The rest of the code.

    add(100.0);
    double a[3] = {1.1, 2.2, 3.3};
    std::for_each(a, a + 3, add);
    return 0;
}

