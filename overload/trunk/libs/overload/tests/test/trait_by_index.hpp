
#ifndef _TEST_TRAIT_BY_INDEX_HPP_
#define _TEST_TRAIT_BY_INDEX_HPP_


namespace test01{

typedef boost::overload<sig0_t, sig1_t, sig2_t, sig3_t, sig4_t, sig5_t> 
        overload_type;

BOOST_STATIC_ASSERT(( ovdtl::extent<overload_type>::value == 6 ));
// MSVC 7.1 doesn't support the syntax overload_type::base_type::base_type::...
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1400)
BOOST_STATIC_ASSERT(( 
    is_same<ovdtl::base_by_idx<3, overload_type>::type, 
            overload_type::base_type::base_type::base_type::base_type
    >::value 
));
#endif
BOOST_STATIC_ASSERT(( is_same<ovdtl::signature<2,overload_type>::type, sig2_t>::value ));
BOOST_STATIC_ASSERT(( 
    is_same<ovdtl::function<5, overload_type>::type, 
            boost::function<sig5_t> 
    >::value 
));

}

#endif // _TEST_TRAIT_BY_INDEX_HPP_

