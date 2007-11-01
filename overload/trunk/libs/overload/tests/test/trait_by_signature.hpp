
#ifndef _TEST_TRAIT_BY_SIGNATURE_HPP_
#define _TEST_TRAIT_BY_SIGNATURE_HPP_


namespace test02{

typedef boost::overload<sig0_t, sig1_t, sig2_t, sig3_t, sig4_t, sig5_t> 
        overload_type;

// MSVC 7.1 doesn't support the syntax overload_type::base_type::base_type::...
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
BOOST_STATIC_ASSERT(( 
    is_same<ovdtl::base_by_sig<sig3_t, overload_type>::type, 
            overload_type::base_type::base_type::base_type::base_type
    >::value 
));
#endif
BOOST_STATIC_ASSERT(( ovdtl::index<sig2_t, overload_type>::value == 2 ));
BOOST_STATIC_ASSERT(( ovdtl::has_signature<sig5_t, overload_type>::value ));
BOOST_STATIC_ASSERT(( !ovdtl::has_signature<sig6_t, overload_type>::value ));

}

#endif // _TEST_TRAIT_BY_SIGNATURE_HPP_

