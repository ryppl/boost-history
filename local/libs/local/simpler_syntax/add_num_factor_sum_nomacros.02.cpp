
#include <boost/local/function.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/at.hpp>
#include <iostream>

int main() {
    double sum = 0.0;
    int factor = 10;

    // Determine bound parameter types via Boost.Typeof (from Boost.ScopeExit).
    
    typedef void (*boost_se_tag_0_add20)(int factor );
    typedef void (*boost_se_tag_1_add20)(int &sum );
    
    typedef BOOST_TYPEOF(
        boost::type_of::ensure_obj(
            boost::scope_exit::aux::wrap(
                boost::scope_exit::aux::deref(
                      factor
                    , (boost_se_tag_0_add20)0
                )
            )
        )
    ) boost_se_wrapped_t_0_add20; 
    typedef boost_se_wrapped_t_0_add20::type boost_se_capture_t_0_add20; 

    typedef BOOST_TYPEOF(
        boost::type_of::ensure_obj(
            boost::scope_exit::aux::wrap(
                boost::scope_exit::aux::deref(
                      &sum
                    , (boost_se_tag_1_add20)0
                )
            )
        )
    ) boost_se_wrapped_t_1_add20;
    typedef boost_se_wrapped_t_1_add20::type boost_se_capture_t_1_add20;
    
    struct boost_se_params_t_add20 { 
        typedef boost_se_capture_t_0_add20 boost_se_param_t_0_add20; 
        typedef boost_se_capture_t_1_add20 boost_se_param_t_1_add20; 
        boost::scope_exit::aux::member<
              boost_se_param_t_0_add20
            , boost_se_tag_0_add20 
        > boost_se_param_0_add20; 
        boost::scope_exit::aux::member< 
              boost_se_param_t_1_add20
            , boost_se_tag_1_add20 
        > boost_se_param_1_add20; 
    } boost_local_auxXparams_add20 = { 
          { boost::scope_exit::aux::deref(factor, (boost_se_tag_0_add20)0) } 
        , { boost::scope_exit::aux::deref(&sum, (boost_se_tag_1_add20)0) } 
    }; 
    
    boost::scope_exit::aux::declared< 
        boost::scope_exit::aux::resolve< 
            sizeof(boost_local_auxXargs) 
        >::cmp1<0>::cmp2 
    > boost_local_auxXargs;
    boost_local_auxXargs.value = &boost_local_auxXparams_add20;
    
    // Determine unbound parameter types (handle attached param type-name-dflt).
    //      F( (void) (f)(double num = -1.23, int x = 1) )
    // or   F( void, f, (double num = -1.23, int x = 1) )
#define RESULT_TYPE void
#define ARG0        double num = -1.23
    typedef RESULT_TYPE (function_type_add20)(ARG0);
    typedef boost::function_types::parameter_types<function_type_add20>::type
            param_types_add20;
    typedef boost::mpl::at_c<param_types_add20, 0>::type arg_type0_add20;

    // Local function.

    /** @todo Is there a (compile-time) way to count default params from a function type? If not, I need to separate the dflt so I count them at pp-time... */
    typedef ::boost::local::function_ref< function_type_add20, 0 > 
            boost_local_auxXaddXref;
    struct boost_local_auxXfunctorXadd : 
            ::boost::local::aux::function_base< function_type_add20, 0> {
        explicit boost_local_auxXfunctorXadd(void* bind_params) : 
                boost_local_auxXparams( (boost_se_params_t_add20*) bind_params)
                {} 
        // Generic param names because unseparated param type-name-dflt.
        void operator()( arg_type0_add20 arg0 ) { 
            return boost_local_auxXbody( 
                  *this 
                , boost_local_auxXparams-> boost_se_param_0_add20.value 
                , boost_local_auxXparams-> boost_se_param_1_add20.value 
                , arg0
            ); 
        } 
    private: 
        boost_se_params_t_add20* boost_local_auxXparams; 
        static void boost_local_auxXbody( 
            // I need to know the function name at the decl for recursion.
              boost_local_auxXfunctorXadd& add 
            , ::boost::add_const< 
                  boost_se_params_t_add20:: boost_se_param_t_0_add20 
              >::type factor 
            , boost_se_params_t_add20:: boost_se_param_t_1_add20 &sum 
            , ARG0
    ) { // The body.
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
    }
    
    // The END macro.

    }; 
    boost_local_auxXfunctorXadd boost_local_auxXobjectXadd( 
            boost_local_auxXargs.value); 
    boost_local_auxXaddXref add( boost_local_auxXobjectXadd);

    // The rest of the code.

    add(100.0);
    return 0;
}

