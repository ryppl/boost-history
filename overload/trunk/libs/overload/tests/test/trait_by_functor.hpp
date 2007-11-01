
#ifndef _TEST_TRAIT_BY_FUNCTOR_HPP_
#define _TEST_TRAIT_BY_FUNCTOR_HPP_


namespace test03 {

typedef boost::overload<sig0_t, sig1_t, sig2_t, sig3_t, sig4_t, 
                        sig5_t, sig6_t, sig7_t, sig8_t, sig9_t> 
        overload_type;

BOOST_STATIC_ASSERT(( ovdtl::functor_has_signature<f4_t, sig4_t>::value ));

}


namespace test04 {

typedef boost::overload<sig0_t, sig1_t, sig2_t, sig3_t, sig4_t, sig5_t, sig6_t, sig7_t>
        overload_type;

BOOST_STATIC_ASSERT(( 
    is_same<ovdtl::base_by_functor<f0_t, overload_type>::type, 
            overload_type::base_type
    >::value
));
BOOST_STATIC_ASSERT(( 
    is_same<ovdtl::signature_by_functor<f5_t, ovdtl::base_by_functor<f0_t, overload_type>::type >::type, 
            sig5_t
    >::value 
));
BOOST_STATIC_ASSERT(( 
    is_same<ovdtl::function_by_functor<f1_t, overload_type>::type, 
            boost::function<sig1_t> 
    >::value 
));
BOOST_STATIC_ASSERT(( ovdtl::index_by_functor<f2_t, overload_type>::value == 2 ));
BOOST_STATIC_ASSERT(( ovdtl::has_functor_signature<f4_t, overload_type>::value ));
BOOST_STATIC_ASSERT(( !ovdtl::has_functor_signature<f7_t, overload_type>::value ));

}


#endif // _TEST_TRAIT_BY_FUNCTOR_HPP_

