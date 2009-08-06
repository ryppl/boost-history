#ifndef BOOST_UNICODE_HANGUL_HPP
#define BOOST_UNICODE_HANGUL_HPP

/* These algorithms were adapted from the one provided by
 * UAX #15: Unicode Normalization Forms
 * http://www.unicode.org/reports/tr15/ */

#include <boost/cuchar.hpp>
#include <boost/mpl/int.hpp>

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

/** \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly that
 * transforms a single Hangul syllable (LV or LVT) into its decomposed
 * form since those decompositions are not part of the UCD.
 * Other code points are left unchanged. */
struct hangul_decomposer
{
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
    
};

/** \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly that
 * transforms <L, V> and <LV, T> Hangul code points sequences into the
 * LV and LVT Hangul syllables, since those compositions are not part
 * of the UCD.
 * Other code points are left unchanged. */
struct hangul_composer
{
    typedef char32 output_type;
    typedef mpl::int_<1> max_output;
    
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {/*
        using namespace detail;

        char32 last = *begin; // copy first char
        result.append(last);

        for(int i=1; i<len; i++)
        {
            char32 ch = source.charAt(i);

            // 1. check to see if two current characters are L and V
            char32 LIndex = last - LBase;
            if(0 <= LIndex && LIndex < LCount)
            {
                char32 VIndex = ch - VBase;
                if(0 <= VIndex && VIndex < VCount)
                {
                    // make syllable of form LV
                    last = SBase + (LIndex * VCount + VIndex) * TCount;
                    result.setCharAt(result.length()-1, last); // reset last
                    continue; // discard ch
                }
            }


            // 2. check to see if two current characters are LV and T
            char32 SIndex = last - SBase;
            if(0 <= SIndex && SIndex < SCount && (SIndex % TCount) == 0)
            {
                char32 TIndex = ch - TBase;
                if(0 < TIndex && TIndex < TCount)
                {
                    // make syllable of form LVT
                    last += TIndex;
                    result.setCharAt(result.length()-1, last); // reset last
                    continue; // discard ch
                }
            }
            // if neither case was true, just add the character
            last = ch;
            result.append(ch);
        }
        */
        *out++ = *begin++;
        return std::make_pair(begin, out);
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        *out++ = *--end;
        return std::make_pair(end, out);
    }
};

} // namespace unicode
} // namespace boost

#endif
