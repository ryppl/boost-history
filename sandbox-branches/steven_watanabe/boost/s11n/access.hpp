#ifndef BOOST_S11N_ACCESS_HPP_INCLUDED
#define BOOST_S11N_ACCESS_HPP_INCLUDED

namespace boost { namespace s11n
{

    class access
    {
    public:
        template<class Archive, class T> static void serialize(Archive &ar, T &t)
        {
            t.serialize(ar);
        }
    };

} }

#endif
