
#include <boost/local/function.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/at.hpp>
#include <iostream>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

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
#define RESULT_TYPE void
#define ARG0        double num = -1.23
    typedef RESULT_TYPE (function_type_id20)(ARG0);
    typedef boost::function_types::parameter_types<function_type_id20>::type
            param_types_id20;
    typedef boost::mpl::at_c<param_types_id20, 0>::type arg_type0_id20;

    // Local function.

    struct boost_local_auxXfunctorXid20 : 
            ::boost::local::aux::function_base< function_type_id20, 0> {
        typedef ::boost::local::function_ref< function_type_id20, 0 >
                function_ref_type;

        explicit boost_local_auxXfunctorXid20(void* bind_params) : 
                boost_local_auxXparams( (boost_se_params_t_id20*) bind_params)
                {} 
        // Generic param names because unseparated param type-name-dflt.
        void operator()( arg_type0_id20 arg0 ) { 
            return boost_local_auxXbody( 
                  *this 
                , boost_local_auxXparams-> boost_se_param_0_id20.value 
                , boost_local_auxXparams-> boost_se_param_1_id20.value 
                , arg0
            ); 
        } 
    private: 
        boost_se_params_t_id20* boost_local_auxXparams;
        static void boost_local_auxXbody( 
            // Use `self_` instead of function name for recursion...
              boost_local_auxXfunctorXid20& self_
            , ::boost::add_const< 
                  boost_se_params_t_id20:: boost_se_param_t_0_id20 
              >::type factor 
            , boost_se_params_t_id20:: boost_se_param_t_1_id20 &sum 
            , ARG0
    ) { // The body.
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
    }
    
    // The END macro.
    
    public:
        function_ref_type* deduce_function_ref_type;

    } boost_local_auxXobjectXadd(boost_local_auxXargs.value); 
    BOOST_TYPEOF(*(boost_local_auxXobjectXadd.deduce_function_ref_type))
            add(boost_local_auxXobjectXadd);

    // The rest of the code.

    add(100.0);
    double a[3] = {1.1, 2.2, 3.3};
    std::for_each(a, a + 3, add);
    return 0;
}

