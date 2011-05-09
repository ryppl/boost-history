
#include <boost/preprocessor.hpp>

for t in range(types):
    for u in range(types):
        for v in range(types):
                print t, u, v, w

#define Y(z, n, u) \

#define X(z, n, u) \
    BOOST_PP_REPEAT_ ## a(2, Y, ~)

BOOST_PP_REPEAT(2, X, ~)
    

