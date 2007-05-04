#ifndef BOOST_DETAIL_STORABLE
#define BOOST_DETAIL_STORABLE

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost {
namespace detail {

///  Converts a type into a storable type by removing const qualifiers and references.
template<typename T>
struct storable : public boost::remove_const<typename boost::remove_reference<T>::type > {};


} // namespace detail
} // namespace boost

#endif // BOOST_DETAIL_STORABLE