
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

template< typename F, size_t defaults = 0 >
struct casting_function {};

/*
template< typename R, typename A0 >
class casting_function< R (A0), 0 > {
    typedef R (*call_type_0)(void*, A0);
public:
    inline void init(void* obj, call_type_0 call0) { obj_ = obj; call0_ = call0; }
    inline void operator()(A0 num) { call0_(obj_, num); }
private:
    void* obj_;
    call_type_0 call0_;
    void* call1_;
    void* call2_;
};
*/

template< typename R, typename A0 >
class casting_function< R (A0), 0 > {
    typedef void* object_ptr;
    typedef R (*call_ptr0)(object_ptr ,  typename ::boost::call_traits<A0 >::param_type);
public:
    inline void init(object_ptr object, call_ptr0 call0) {
        object_ = object;
        call0_ = call0;
    }
    inline R operator()( typename ::boost::call_traits<A0 >::param_type a0) const { return call0_(object_ ,  a0); }
private:
    object_ptr object_;
    call_ptr0 call0_;
    void* call1_;
    void* call2_;
};

int main() {
    double sum = 0.0;
    int factor = 1;

    void (*ERROR_missing_result_type_before_the_local_function_parameter_macro_id19)(); typedef void (*boost_local_auxXdeduce_result_tag19)( int ERROR_missing_result_type_before_the_local_function_parameter_macro_id19); 
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref( ERROR_missing_result_type_before_the_local_function_parameter_macro_id19, (boost_local_auxXdeduce_result_tag19)0))) boost_local_auxXdeduce_result_wrap19; 
    typedef boost_local_auxXdeduce_result_wrap19::type boost_local_auxXdeduce_result_capture19; 
    struct boost_local_auxXdeduce_result_params19 { typedef boost_local_auxXdeduce_result_capture19 function_ptr_type; }; 
    typedef boost::remove_pointer<  boost_local_auxXdeduce_result_params19::function_ptr_type >::type boost_local_auxXdeduce_result_function_type19; 
    typedef boost::function_traits< boost_local_auxXdeduce_result_function_type19>::result_type boost_local_auxXresult_type19;
    typedef void (*boost_se_tag_0_19)(int & factor ); 
    typedef void (*boost_se_tag_1_19)(int & sum );   
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref(& factor, (boost_se_tag_0_19)0))) boost_se_wrapped_t_0_19; 
    typedef boost_se_wrapped_t_0_19::type boost_se_capture_t_0_19; 
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref(& sum, (boost_se_tag_1_19)0))) boost_se_wrapped_t_1_19; 
    typedef boost_se_wrapped_t_1_19::type boost_se_capture_t_1_19;   
    struct boost_se_params_t_19 {
        typedef boost_se_capture_t_0_19 boost_se_param_t_0_19; 
        typedef boost_se_capture_t_1_19 boost_se_param_t_1_19;   
        boost::scope_exit::aux::member< boost_se_param_t_0_19, boost_se_tag_0_19 > boost_se_param_0_19; 
        boost::scope_exit::aux::member< boost_se_param_t_1_19, boost_se_tag_1_19 > boost_se_param_1_19;
    } boost_local_auxXparams19 = {
#ifdef BOOST_SCOPE_EXIT_AUX_TPL_WORKAROUND
        {
#endif
        boost::scope_exit::aux::deref(& factor, (boost_se_tag_0_19)0)
#ifdef BOOST_SCOPE_EXIT_AUX_TPL_WORKAROUND
        }
#endif        
        ,
#ifdef BOOST_SCOPE_EXIT_AUX_TPL_WORKAROUND
        {
#endif
        boost::scope_exit::aux::deref(& sum, (boost_se_tag_1_19)0)   
#ifdef BOOST_SCOPE_EXIT_AUX_TPL_WORKAROUND
        }
#endif        
    }; 
    boost::scope_exit::aux::declared< boost::scope_exit::aux::resolve< sizeof(boost_local_auxXargs) >::cmp1<0>::cmp2 > boost_local_auxXargs; 
    boost_local_auxXargs.value = &boost_local_auxXparams19; 

    class local_add
// Cannot use virtual-base trick because it prevents optimizations also when passing the local functor as template param on C++03. 
// Use casting trick instead to allow for C++03 optimizations.
//        : public ::boost::local::aux::abstract_function<boost_local_auxXresult_type19 (const double& num), 0>
    {
        // Function and functor types.
        typedef boost_local_auxXresult_type19 (boost_local_auxXfunction_type) (const double& num);
        typedef casting_function< boost_local_auxXfunction_type, 0 > casting_function_type; 
        // For local-typeof.
        typedef ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & factorXboost_local_auxXtypeof_type ;
        typedef boost_se_params_t_19:: boost_se_param_t_1_19 & sumXboost_local_auxXtypeof_type ;     
    public:
        inline explicit local_add(void* binds):
                  bind1(static_cast< boost_se_params_t_19* >(binds)->boost_se_param_0_19.value)
                , bind2(static_cast< boost_se_params_t_19* >(binds)->boost_se_param_1_19.value)
        {}
        // Leave here even if call is used so this class can be used as functor for C++03 optimizations.
        inline boost_local_auxXresult_type19 operator()(::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type>::arg1_type>::param_type arg1) {
            return body(bind1, bind2, arg1);
        }
        inline boost_local_auxXresult_type19 operator()() {
            return body(bind1, bind2);
        }
        // To pass local function as tparam on non C++03.
        inline static boost_local_auxXresult_type19 call0(void* obj, const double& num) {
            return static_cast<local_add*>(obj)->operator()(num);
        }
        inline static boost_local_auxXresult_type19 call1(void* obj) {
            return static_cast<local_add*>(obj)->operator()();
        }
        inline static void init_functor(void* object, casting_function_type& fctor) {
            fctor.init(object, &call0); // here knows default_counts
        }
    private:
        // Use binds as single mem vars instead of accessing `boost_se_params_t_19* boost_local_auxXbinds` all the times improves performance.
        // Always add ref because these data must just reference the actual data stored in the params struct declared outside this class (and passed to the ctor by
        // ptr).
        ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & bind1;
        boost_se_params_t_19:: boost_se_param_t_1_19 & bind2;
        // For nesting.
        boost::scope_exit::aux::undeclared boost_local_auxXargs;
        // Body.
        inline boost_local_auxXresult_type19 body(
                  ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type& factor
                , boost_se_params_t_19:: boost_se_param_t_1_19& sum
                , const double& num = 0.0) const {
            sum += factor * num;
        }
    public:
        // This must always be the non-local functor even if optimizing because it cannot be a ref to this local functors because ctor cannot init the ref because it
        // does not know the local function name.
        casting_function_type add; 
        inline void init_recursion(casting_function_type& fctor) { add = fctor; } // cannot call from ctor to allow for opt
    } add_fctor(boost_local_auxXargs.value);
    BOOST_TYPEOF(add_fctor.add) add;
    add_fctor.init_recursion(add);
    add_fctor.init_functor(&add_fctor, add);

//    add_fctor(1.23);
//    add_fctor();
//    add(3.21);
//    add();

    std::vector<double> v(1e4 * 1e2);
    std::fill(v.begin(), v.end(), 1.0);

    for (size_t n = 0; n < 1e4; ++n) {
        std::for_each(v.begin(), v.end(), add_fctor);
//        for (size_t i = 0; i < v.size(); ++i) add_lf(v[i]);
    }

    std::cout << sum << std::endl;
    assert(sum == 1e4 * 1e4 * 1e2);
    return 0;
}

