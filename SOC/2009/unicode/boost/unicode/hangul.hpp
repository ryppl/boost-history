#ifndef BOOST_UNICODE_HANGUL_HPP
#define BOOST_UNICODE_HANGUL_HPP

/* These algorithms were adapted from the one provided by
 * UAX #15: Unicode Normalization Forms
 * http://www.unicode.org/reports/tr15/ */

#include <boost/cuchar.hpp>
#include <boost/mpl/int.hpp>
#include <boost/iterator/convert_iterator.hpp>

namespace boost
{
namespace unicode
{

namespace detail
{
    static const char32 SBase = 0xAC00;
    static const char32 LBase = 0x1100;
    static const char32 VBase = 0x1161;
    static const char32 TBase = 0x11A7;
    static const char32 LCount = 19;
    static const char32 VCount = 21;
    static const char32 TCount = 28;
    static const char32 NCount = VCount * TCount; // 588
    static const char32 SCount = LCount * NCount; // 11172
}

/** \c \xmlonly<conceptname>OneManyConverter</conceptname>\endxmlonly that
 * transforms a single Hangul syllable (LV or LVT) into its decomposed
 * form since those decompositions are not part of the UCD.
 * Other code points are left unchanged. */
struct hangul_decomposer : one_many_converter<hangul_decomposer>
{
    typedef char32 input_type;
    typedef char32 output_type;
    typedef mpl::int_<3> max_output;
    
    template<typename Out>
    Out operator()(char32 ch, Out out)
    {
        using namespace detail;
        
        char32 SIndex = ch - SBase;
        if(SIndex < 0 || SIndex >= SCount)
        {
            *out++ = ch;
            return out;
        }
        
        char32 L = LBase + SIndex / NCount;
        char32 V = VBase + (SIndex % NCount) / TCount;
        char32 T = TBase + SIndex % TCount;
        
        *out++ = L;
        *out++ = V;
        if(T != TBase)
            *out++ = T;
        
        return out;
    }
    
    static int len(char32 ch)
    {
        using namespace detail;
        
        char32 SIndex = ch - SBase;
        char32 TIndex = SIndex % TCount;
        
        if(SIndex < 0 || SIndex >= SCount)
            return 1;
        if(TIndex)
            return 3;
        return 2;
    }
    
};

/** \c \xmlonly<conceptname>Converter</conceptname>\endxmlonly that
 * transforms <L, V>, <L, V, T> and <LV, T> Hangul code points sequences into the
 * LV and LVT Hangul syllables, since those compositions are not part
 * of the UCD.
 * Other code points are left unchanged. */
struct hangul_composer
{
    typedef char32 input_type;
    typedef char32 output_type;
    typedef mpl::int_<1> max_output;
    
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {
        char32 ch = *begin++;
        
        if(is_l(ch) && begin != end)
        {
            char32 v = *begin;
            if(is_v(v))
            {
                ++begin;
                if(begin == end)
                {
                    *out++ = combine_l_v(ch, v);
                    return std::make_pair(begin, out);
                }

                char32 t = *begin;
                if(is_t(t))
                {
                    ++begin;
                    *out++ = combine_lv_t(combine_l_v(ch, v), t);
                    return std::make_pair(begin, out);
                }
                
                *out++ = combine_l_v(ch, v);
                return std::make_pair(begin, out);
            }
        }
        else if(is_lv(ch) && begin != end)
        {
            char32 t = *begin;
            if(is_t(t))
            {
                ++begin;
                *out++ = combine_lv_t(ch, t);
                return std::make_pair(begin, out);
            }
        }

        *out++ = ch;
        return std::make_pair(begin, out);
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        char32 ch = *--end;
        
        if(is_t(ch) && end != begin)
        {
            char32 v = *--end;
            if(is_v(v) && end != begin)
            {
                char32 l = *--end;
                if(is_l(l))
                {
                    *out++ = combine_lv_t(combine_l_v(l, v), ch);
                    return std::make_pair(end, out);
                }
                ++end;
            }
            else if(is_lv(v))
            {
                *out++ = combine_lv_t(v, ch);
                return std::make_pair(end, out);
            }
            ++end;
        }
        else if(is_v(ch) && end != begin)
        {
            char32 l = *--end;
            if(is_l(l))
            {
                *out++ = combine_l_v(l, ch);
                return std::make_pair(end, out);
            }
            ++end;
        }

        *out++ = ch;
        return std::make_pair(end, out);
    }
    
private:
    static bool is_l(char32 ch)
    {
        char32 LIndex = ch - detail::LBase;
        return 0 <= LIndex && LIndex < detail::LCount;
    }

    static bool is_v(char32 ch)
    {
        char32 VIndex = ch - detail::VBase;
        return 0 <= VIndex && VIndex < detail::VCount;
    }

    static char32 combine_l_v(char32 l, char32 v)
    {
        char32 LIndex = l - detail::LBase;
        char32 VIndex = v - detail::VBase;
        return detail::SBase + (LIndex * detail::VCount + VIndex) * detail::TCount;
    }

    static bool is_lv(char32 ch)
    {
        char32 SIndex = ch - detail::SBase;
        return 0 <= SIndex && SIndex < detail::SCount && (SIndex % detail::TCount) == 0;
    }

    static bool is_t(char32 ch)
    {
        char32 TIndex = ch - detail::TBase;
        return 0 < TIndex && TIndex < detail::TCount;
    }

    static char32 combine_lv_t(char32 lv, char32 t)
    {
        char32 TIndex = t - detail::TBase;
        return lv + TIndex;
    }
};

} // namespace unicode
} // namespace boost

#endif
