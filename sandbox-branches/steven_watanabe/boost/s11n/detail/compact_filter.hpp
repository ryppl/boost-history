#ifndef BOOST_S11N_DETAIL_COMPACT_FILTER_HPP_INCLUDED
#define BOOST_S11N_DETAIL_COMPACT_FILTER_HPP_INCLUDED

#include <boost/cstdint.hpp>

namespace boost { namespace s11n { namespace compact_filter_detail
{

    // Maximum buffer size required for storing compacted number
    const int max_buffer_size = 10;
    
    template<class T>
    struct make_unsigned
    {
    };

    template<>
    struct make_unsigned<char>
    {
        typedef unsigned char type;
    };

    template<>
    struct make_unsigned<signed char>
    {
        typedef unsigned char type;
    };

    template<>
    struct make_unsigned<signed short>
    {
        typedef unsigned short type;
    };

    template<>
    struct make_unsigned<signed int>
    {
        typedef unsigned int type;
    };

    template<>
    struct make_unsigned<signed long>
    {
        typedef unsigned long type;
    };

    template<>
    struct make_unsigned<boost::int64_t>
    {
        typedef boost::uint64_t type;
    };

    template<class T> inline T rshift(T x, int n)
    {
        if (n < std::numeric_limits<T>::digits)
            return x >> n;
        else
            return 0;
    }

    template<class T> inline T lshift(T x, int n)
    {
        if (n < std::numeric_limits<T>::digits)
            return x << n;
        else
            return 0;
    }

    template<class T>
    inline int compact_unsigned(unsigned char *buffer, T n)
    {

        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_integer == true);
        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_signed == false);
        
        if (n < (boost::uint64_t(1) << 7))
        {
            buffer[0] = (unsigned char)(n);
            return 1;
        }
        else if (n < (boost::uint64_t(1) << 14))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7));
            return 2;
        }
        else if (n < (boost::uint64_t(1) << 21))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14));
            return 3;
        }
        else if (n < (boost::uint64_t(1) << 28))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21));
            return 4;
        }
        else if (n < (boost::uint64_t(1) << 35))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21) & 0x7F) | 0x80;
            buffer[4] = (unsigned char)(rshift(n, 28));
            return 5;
        }
        else if (n < (boost::uint64_t(1) << 42))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21) & 0x7F) | 0x80;
            buffer[4] = (unsigned char)(rshift(n, 28) & 0x7F) | 0x80;
            buffer[5] = (unsigned char)(rshift(n, 35));
            return 6;
        }
        else if (n < (boost::uint64_t(1) << 49))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21) & 0x7F) | 0x80;
            buffer[4] = (unsigned char)(rshift(n, 28) & 0x7F) | 0x80;
            buffer[5] = (unsigned char)(rshift(n, 35) & 0x7F) | 0x80;
            buffer[6] = (unsigned char)(rshift(n, 42));
            return 7;
        }
        else if (n < (boost::uint64_t(1) << 56))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21) & 0x7F) | 0x80;
            buffer[4] = (unsigned char)(rshift(n, 28) & 0x7F) | 0x80;
            buffer[5] = (unsigned char)(rshift(n, 35) & 0x7F) | 0x80;
            buffer[6] = (unsigned char)(rshift(n, 42) & 0x7F) | 0x80;
            buffer[7] = (unsigned char)(rshift(n, 49));
            return 8;
        }
        else if (n < (boost::uint64_t(1) << 63))
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21) & 0x7F) | 0x80;
            buffer[4] = (unsigned char)(rshift(n, 28) & 0x7F) | 0x80;
            buffer[5] = (unsigned char)(rshift(n, 35) & 0x7F) | 0x80;
            buffer[6] = (unsigned char)(rshift(n, 42) & 0x7F) | 0x80;
            buffer[7] = (unsigned char)(rshift(n, 49) & 0x7F) | 0x80;
            buffer[8] = (unsigned char)(rshift(n, 56));
            return 9;
        }
        else
        {
            buffer[0] = (unsigned char)(n & 0x7F) | 0x80;
            buffer[1] = (unsigned char)(rshift(n, 7) & 0x7F) | 0x80;
            buffer[2] = (unsigned char)(rshift(n, 14) & 0x7F) | 0x80;
            buffer[3] = (unsigned char)(rshift(n, 21) & 0x7F) | 0x80;
            buffer[4] = (unsigned char)(rshift(n, 28) & 0x7F) | 0x80;
            buffer[5] = (unsigned char)(rshift(n, 35) & 0x7F) | 0x80;
            buffer[6] = (unsigned char)(rshift(n, 42) & 0x7F) | 0x80;
            buffer[7] = (unsigned char)(rshift(n, 49) & 0x7F) | 0x80;
            buffer[8] = (unsigned char)(rshift(n, 56) & 0x7F) | 0x80;
            buffer[9] = (unsigned char)(rshift(n, 63));
            return 10;
        }

    }

    template<class T>
    inline bool expand_unsigned(const unsigned char *buffer, int size, T &n)
    {

        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_integer == true);
        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_signed == false);

        switch (size)
        {

        case 1:
            if ((buffer[0] & 0x80) == 0)
            {
                n = buffer[0];
                return true;
            }
            else
                return false;

        case 2:
            if ((buffer[1] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7);
                return true;
            }
            else
                return false;

        case 3:
            if ((buffer[2] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2]), 14);
                return true;
            }
            else
                return false;

        case 4:
            if ((buffer[3] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3]), 21);
                return true;
            }
            else
                return false;

        case 5:
            if ((buffer[4] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3] & 0x7F), 21) +
                    lshift(T(buffer[4]), 28);
                return true;
            }
            else
                return false;

        case 6:
            if ((buffer[5] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3] & 0x7F), 21) +
                    lshift(T(buffer[4] & 0x7F), 28) +
                    lshift(T(buffer[5]), 35);
                return true;
            }
            else
                return false;

        case 7:
            if ((buffer[6] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3] & 0x7F), 21) +
                    lshift(T(buffer[4] & 0x7F), 28) +
                    lshift(T(buffer[5] & 0x7F), 35) +
                    lshift(T(buffer[6]), 42);
                return true;
            }
            else
                return false;

        case 8:
            if ((buffer[7] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3] & 0x7F), 21) +
                    lshift(T(buffer[4] & 0x7F), 28) +
                    lshift(T(buffer[5] & 0x7F), 35) +
                    lshift(T(buffer[6] & 0x7F), 42) +
                    lshift(T(buffer[7]), 49);
                return true;
            }
            else
                return false;

        case 9:
            if ((buffer[8] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3] & 0x7F), 21) +
                    lshift(T(buffer[4] & 0x7F), 28) +
                    lshift(T(buffer[5] & 0x7F), 35) +
                    lshift(T(buffer[6] & 0x7F), 42) +
                    lshift(T(buffer[7] & 0x7F), 49) +
                    lshift(T(buffer[8]), 56);
                return true;
            }
            else
                return false;

        case 10:
            if ((buffer[9] & 0x80) == 0)
            {
                n = T(buffer[0] & 0x7F) + 
                    lshift(T(buffer[1] & 0x7F), 7) +
                    lshift(T(buffer[2] & 0x7F), 14) +
                    lshift(T(buffer[3] & 0x7F), 21) +
                    lshift(T(buffer[4] & 0x7F), 28) +
                    lshift(T(buffer[5] & 0x7F), 35) +
                    lshift(T(buffer[6] & 0x7F), 42) +
                    lshift(T(buffer[7] & 0x7F), 49) +
                    lshift(T(buffer[8] & 0x7F), 56) +
                    lshift(T(buffer[9]), 63);
                return true;
            }
            else
                return false;

        default:
            BOOST_ASSERT(0);
			return false;

        }
        
    }

    template<class T>
    inline int compact_signed(unsigned char *buffer, T n)
    {
        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_integer == true);
        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_signed == true);
        typedef typename make_unsigned<T>::type U;
        return compact_unsigned(buffer, U(n < 0 ? U(-(n + 1)) * 2 + 1 : U(n) * 2));
    }

    template<class T>
    inline bool expand_signed(const unsigned char *buffer, int size, T &n)
    {
        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_integer == true);
        BOOST_STATIC_ASSERT(std::numeric_limits<T>::is_signed == true);
        typedef typename make_unsigned<T>::type U;
        U u;
        if (expand_unsigned<U>(buffer, size, u))
        {
            n = (u & 1) ? -T(u / 2) - 1 : T(u / 2);
            return true;
        }
        else
            return false;
    }

} } }

#endif
