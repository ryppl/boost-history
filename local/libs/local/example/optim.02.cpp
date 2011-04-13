
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

// This is a non-local functor so it can be passed as template parameter.
// obj will be of local class type so it cannot be passed as template param (it is instead handled as a generic void pointer and the call function will cast it).
template< typename R, typename A0, size_t defaults >
class casting_function {
    typedef R (*call_function)(void* obj, A0);
public:
    // public
    explicit casting_function(void* obj = 0, call_function call = 0): obj_(obj), call_(call) {}
    /* implicit for GCC */ casting_function(casting_function const& other): obj_(other.obj_), call_(other.call_) {}
    // precondition: set
    inline void operator()(A0 num) { call_(obj_, num); } // function pointer call cannot be inlined
    // private
    inline void set(void* obj = 0, call_function call = 0) { obj_ = obj; call_ = call; }
private:
    void* obj_;
    call_function call_;
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
    
    class boost_local_auxXfunctor19 
        : public ::boost::local::aux::abstract_function< boost_local_auxXresult_type19  (  const double& num    ), 0 > 
    { 
        typedef boost_local_auxXresult_type19 (boost_local_auxXfunction_type) (  const double& num    ); 
    public: 
        explicit boost_local_auxXfunctor19(void* binding_data):
                  boost_local_auxXbinds(static_cast< boost_se_params_t_19*>(binding_data))
                , factor(boost_local_auxXbinds->boost_se_param_0_19.value)
                , sum(boost_local_auxXbinds-> boost_se_param_1_19.value)
        {
            boost_local_auxXinit_recursion(); 
        } 
        boost_local_auxXresult_type19 operator()(::boost::call_traits< ::boost::function_traits<boost_local_auxXfunction_type>::arg1_type>::param_type arg1) {
            return boost_local_auxXbody(arg1);
        } 
    private: 
        typedef ::boost::local::function<boost_local_auxXfunction_type, 0 > boost_local_auxXfunctor_type; 
        typedef ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & factorXboost_local_auxXtypeof_type ;   
        typedef boost_se_params_t_19:: boost_se_param_t_1_19 & sumXboost_local_auxXtypeof_type ;     
        boost_se_params_t_19* boost_local_auxXbinds; 
        boost::scope_exit::aux::undeclared boost_local_auxXargs;
        
        ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & factor;
        boost_se_params_t_19:: boost_se_param_t_1_19  & sum;

        boost_local_auxXresult_type19 boost_local_auxXbody(const double& num) const {
            sum += factor * num;
        }

    private: 
        boost_local_auxXfunctor_type add; 
    private: 
        void boost_local_auxXinit_recursion() { add = *this; } 
    } add_bl(boost_local_auxXargs.value);
    ::boost::local::function< boost_local_auxXresult_type19  (  const double& num    ), 0 > add_f(add_bl);

    class local_add
// Cannot use virtual-base trick because it prevents optimizations also when passing the local functor as template param on C++03. 
// Use casting trick instead to allow for C++03 optimizations.
//        : public ::boost::local::aux::abstract_function<boost_local_auxXresult_type19 (const double& num), 0>
    {
        // Function and functor types.
        typedef boost_local_auxXresult_type19 (boost_local_auxXfunction_type) (const double& num);
        typedef casting_function< boost_local_auxXresult_type19, const double&, 0 > casting_function_type; 
        // For local-typeof.
        typedef ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & factorXboost_local_auxXtypeof_type ;
        typedef boost_se_params_t_19:: boost_se_param_t_1_19 & sumXboost_local_auxXtypeof_type ;     
    public:
        explicit local_add(void* binds):
                  bind1(static_cast< boost_se_params_t_19* >(binds)->boost_se_param_0_19.value)
                , bind2(static_cast< boost_se_params_t_19* >(binds)->boost_se_param_1_19.value)
        {}
        // Leave here even if call is used so this class can be used as functor for C++03 optimizations.
        inline boost_local_auxXresult_type19 operator()(const double& num) {
            return body(
                    //boost_local_auxXbinds-> boost_se_param_0_19.value, boost_local_auxXbinds-> boost_se_param_1_19.value
                    bind1, bind2
                    , num);
        }
        // To pass local function as tparam on non C++03.
        inline static boost_local_auxXresult_type19 call(void* obj, const double& num) {
            return static_cast<local_add*>(obj)->operator()(num);
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
        inline boost_local_auxXresult_type19 body(const int& factor, double& sum, const double& num) const {
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
    add.set(&add_fctor, &add_fctor.call); // cannot set in ctor before init_recursion to allow for opt

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

