///////////////////////////////////////////////////////////////////////////////
//
// File: iterator_cast.hpp
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ITERATOR_CAST_EN_12_17_2004_HPP
#define ITERATOR_CAST_EN_12_17_2004_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost
{

///////////////////////////////////////////////////////////////////////////////
// iterator_cast
//
template<typename Iter>
inline Iter iterator_cast(Iter iter)
{
    return iter;
}

template<typename Base,typename Iter>
inline BOOST_DEDUCED_TYPENAME ::boost::disable_if<
    ::boost::is_same<Base,Iter>
  , Base
>::type
iterator_cast(Iter iter)
{
    return iterator_cast<Base>(iter.base());
}

///////////////////////////////////////////////////////////////////////////////
// auto_base_t
//
template<typename Iter>
struct auto_base_t
{
    explicit auto_base_t(Iter it)
        : iter(it)
    {
    }

    template<typename T>
    operator T() const
    {
        return iterator_cast<T>(iter);
    }

private:
    Iter iter;
};

///////////////////////////////////////////////////////////////////////////////
// auto_base
//
template<typename Iter>
inline auto_base_t<Iter> auto_base(Iter iter)
{
    return auto_base_t<Iter>(iter);
}

} // namespace boost

#endif
