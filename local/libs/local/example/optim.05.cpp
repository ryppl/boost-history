
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

// This is a non-local functor so it can be passed as template parameter.
// obj will be of local class type so it cannot be passed as template param (it is instead handled as a generic void pointer and the call function will cast it).
template< typename R, typename A0, size_t defaults >
class casting_function {
    typedef R (*call_type_0)(void*, A0);
    typedef R (*call_type_1)(void*);
public:
    // public
//    inline explicit casting_function(): obj_(), call0_(), call1_() {}
//    inline /* implicit for GCC */ casting_function(const casting_function& other): obj_(other.obj_), call0_(other.call0_), call1_(other.call1_) {}
    inline void init(void* obj, call_type_0 call0, call_type_1 call1) { obj_ = obj; call0_ = call0; call1_ = call1; }
    inline void operator()(A0 num) { call0_(obj_, num); } // function pointer call cannot be inlined
    inline void operator()() { call1_(obj_); }
    // private
private:
    void* obj_;
    call_type_0 call0_;
    call_type_1 call1_;
};

template< typename R, typename A0, size_t defaults >
class casting_function0 {
    typedef R (*call_type_0)(void*, A0);
public:
    inline void boost_local_auxXinit_call(void* obj, call_type_0 call0) { obj_ = obj; call0_ = call0; }
    inline void operator()(A0 arg0) { call0_(obj_, arg0); }
private:
    void* obj_;
    call_type_0 call0_;
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

    class boost_local_auxXfunctor19 {
        typedef boost_local_auxXresult_type19 (boost_local_auxXfunction_type) (const double& num);
//        typedef ::boost::local::function< boost_local_auxXfunction_type, 0 > boost_local_auxXfunctor_type; 
        typedef casting_function0< boost_local_auxXresult_type19, const double&, 0 > boost_local_auxXfunctor_type; 

        typedef ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & factorXboost_local_auxXtypeof_type ;
        typedef boost_se_params_t_19:: boost_se_param_t_1_19 & sumXboost_local_auxXtypeof_type ;     

    public:
        inline explicit boost_local_auxXfunctor19(void* bindings):
                  boost_local_auxXbind0( static_cast< boost_se_params_t_19* >(bindings)->boost_se_param_0_19.value )
                , boost_local_auxXbind1( static_cast< boost_se_params_t_19* >(bindings)->boost_se_param_1_19.value )
        {}

        inline boost_local_auxXresult_type19 operator()(
                ::boost::call_traits< ::boost::function_traits< boost_local_auxXfunction_type >::arg1_type >::param_type arg1) {
            return boost_local_auxXbody(boost_local_auxXbind0, boost_local_auxXbind1, arg1);
        }
        inline static boost_local_auxXresult_type19 boost_local_auxXcall0(void* obj, const double& num) {
            return static_cast<boost_local_auxXfunctor19*>(obj)->operator()(num);
        }
        inline static void boost_local_auxXinit_call(void* object, boost_local_auxXfunctor_type& functor) {
            functor.boost_local_auxXinit_call(object,  &boost_local_auxXcall0 );
        }
    private:
        ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type & boost_local_auxXbind0;
        boost_se_params_t_19:: boost_se_param_t_1_19 & boost_local_auxXbind1;

        boost::scope_exit::aux::undeclared boost_local_auxXargs;

        inline boost_local_auxXresult_type19 boost_local_auxXbody(
                  ::boost::add_const< boost_se_params_t_19:: boost_se_param_t_0_19 >::type& factor
                , boost_se_params_t_19:: boost_se_param_t_1_19& sum
                , const double& num
        ) const {
            sum += factor * num;
        }
    public:
        boost_local_auxXfunctor_type add; 
        inline void boost_local_auxXinit_recursion(boost_local_auxXfunctor_type& functor) { add = functor; }
    } add(boost_local_auxXargs.value);
    BOOST_TYPEOF(add.add) boost_local_auxXadd;
    add.boost_local_auxXinit_recursion(boost_local_auxXadd);
    add.boost_local_auxXinit_call(&add, boost_local_auxXadd);

//    add_fctor(1.23);
//    add_fctor();
//    add(3.21);
//    add();

    std::vector<double> v(1e4 * 1e2);
    std::fill(v.begin(), v.end(), 1.0);

    for (size_t n = 0; n < 1e4; ++n) {
        std::for_each(v.begin(), v.end(), add);
//        for (size_t i = 0; i < v.size(); ++i) add_lf(v[i]);
    }

    std::cout << sum << std::endl;
    assert(sum == 1e4 * 1e4 * 1e2);
    return 0;
}

