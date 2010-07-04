#ifndef BOOST_UNICODE_DETAIL_CAT_HPP
#define BOOST_UNICODE_DETAIL_CAT_HPP

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
/** INTERNAL ONLY */
#define BOOST_UNICODE_CAT(ns, thing) ns##::##thing
#else
#define BOOST_UNICODE_CAT(ns, thing) ns::thing
#endif

#endif
