

#include <boost/variadic_macro_data/VariadicMacroData.hpp> // Proposed lib.
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
    
VMD_DATA_SIZE(1, 2) // 2
VMD_DATA_SIZE(1)    // 1
VMD_DATA_SIZE()     // 1 not 0 :((

#define PP_VA_EAT(...) /* must expand to nothing */

#define PP_VA_SIZE_1OR0_(maybe_empty) \
        BOOST_PP_IIF(BOOST_PP_IS_EMPTY(maybe_empty (/*exapnd empty */) ), 0, 1)

#define PP_VA_SIZE_(size, ...) \
    BOOST_PP_IIF(BOOST_PP_EQUAL(size, 1), \
        PP_VA_SIZE_1OR0_ \
    , \
        size PP_VA_EAT \
    )(__VA_ARGS__ BOOST_PP_EMPTY)

#define PP_VA_SIZE(...) PP_VA_SIZE_(VMD_DATA_SIZE(__VA_ARGS__), __VA_ARGS__)
    
PP_VA_SIZE(1, 2)    // 2
PP_VA_SIZE(1)       // 1
PP_VA_SIZE()        // 0 :))



#define PP_VA_IS_EMPTY(...) BOOST_PP_IS_EMPTY(__VA_ARGS__ BOOST_PP_EMPTY())


#define X(...) PP_VA_IS_EMPTY(__VA_ARGS__)

X(1)
X()



