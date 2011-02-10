
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    double sum = 0.0;
    int factor = 10;

    // Determine bound parameter types via Boost.Typeof (from Boost.ScopeExit).
    
    typedef void (*boost_se_tag_0_add20)(int factor );
    typedef void (*boost_se_tag_1_add20)(int &sum );
    
    typedef __typeof__(
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

    typedef __typeof__(
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
    
    // Local function.

    typedef ::boost::local::function_ref< void ( double ) , 0 > 
            boost_local_auxXaddXref;
    struct boost_local_auxXfunctorXadd : 
            ::boost::local::aux::function_base< void ( double ), 0> {
        explicit boost_local_auxXfunctorXadd(void* bind_params) : 
                boost_local_auxXparams( (boost_se_params_t_add20*) bind_params)
                {} 
        void operator()( double num ) { 
            return boost_local_auxXbody( 
                  *this 
                , boost_local_auxXparams-> boost_se_param_0_add20.value 
                , boost_local_auxXparams-> boost_se_param_1_add20.value 
                , num 
            ); 
        } 
    private: 
        boost_se_params_t_add20* boost_local_auxXparams; 
        static void boost_local_auxXbody( 
              boost_local_auxXfunctorXadd& add 
            , ::boost::add_const< 
                  boost_se_params_t_add20:: boost_se_param_t_0_add20 
              >::type factor 
            , boost_se_params_t_add20:: boost_se_param_t_1_add20 &sum 
            , double num 
    ) { // The body.
        sum += factor * num;
        std::clog << "Summed: " << sum << std::endl;
    }
    // The END macro.
    }; 
    boost_local_auxXfunctorXadd boost_local_auxXobjectXadd( 
            boost_local_auxXargs.value); 
    boost_local_auxXaddXref add( boost_local_auxXobjectXadd);

    add(100.0);
    return 0;
}

