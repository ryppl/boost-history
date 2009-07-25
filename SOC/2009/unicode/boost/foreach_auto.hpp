#ifndef BOOST_FOREACH_AUTO

#include <boost/typeof/typeof.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_FOREACH_AUTO(name, range)                                \
if(boost::begin(range) != boost::end(range))                           \
if(bool BOOST_PP_CAT(_once_, __LINE__) = 1)                            \
for(                                                                   \
    BOOST_AUTO(name, *boost::begin(range));                            \
    BOOST_PP_CAT(_once_, __LINE__);                                    \
    BOOST_PP_CAT(_once_, __LINE__) = 0                                 \
)                                                                      \
for(                                                                   \
    BOOST_AUTO(BOOST_PP_CAT(_it_, __LINE__), boost::begin(range));     \
        BOOST_PP_CAT(_it_, __LINE__) != boost::end(range)              \
     && (name = *BOOST_PP_CAT(_it_, __LINE__), 1);                     \
    ++BOOST_PP_CAT(_it_, __LINE__)                                     \
)                                                                      \

#endif
