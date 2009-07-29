#ifndef BOOST_DETAIL_UNSPECIFIED_HPP
#define BOOST_DETAIL_UNSPECIFIED_HPP

namespace boost
{
    namespace detail
    {
        template<typename T>
        struct unspecified
        {
            typedef T type;
        };
    }
}

#endif
