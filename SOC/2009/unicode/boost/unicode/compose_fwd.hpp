#ifndef BOOST_UNICODE_COMPOSE_FWD_HPP
#define BOOST_UNICODE_COMPOSE_FWD_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/hangul.hpp>

#include <boost/integer/static_pow.hpp>
#include <climits>

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/adaptor/filtered.hpp>

#include <vector>

namespace boost
{
namespace unicode
{
    
#define BOOST_UNICODE_OPTION(o) (boost::static_pow<2, (o)>::value)
#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
#undef BOOST_UNICODE_OPTION
#endif

/** Model of \c \xmlonly<conceptname>OneManyPipe</conceptname>\endxmlonly
 * that decomposes a code point, i.e. it converts a code point into a
 * sequence of code points.
 * It applies UCD decompositions that match \c mask as well as the Hangul decompositions. */
struct decomposer
{
    typedef char32 input_type;
    typedef char32 output_type;
    
    decomposer(unsigned mask_ = BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical)) : mask(mask_)
    {
    }
    
    template<typename Out>
    Out operator()(char32 ch, Out out)
    {
        iterator_range<const char32*> dec = ucd::get_decomposition(ch);
        if(!empty(dec) && ((1 << ucd::get_decomposition_type(ch)) & mask))
        {
            out = copy(dec, out);
        }
        else
        {
            out = hangul_decomposer()(ch, out);
        }
        return out;
    }
    
private:
    unsigned mask;
};


namespace detail
{
    template<typename In>
    struct composition_find
    {
        typedef bool result_type;
        
        composition_find(std::size_t offset_) : offset(offset_)
        {
        }
        
        bool operator()(const ucd::unichar_compose_data_entry& lft, const ucd::unichar_compose_data_entry& rgt) const
        {
            return std::lexicographical_compare(
                lft.decomp+1, lft.decomp+1+std::min(lft.decomp[0], rgt.decomp[0]),
                rgt.decomp+1, rgt.decomp+1+std::min(lft.decomp[0], rgt.decomp[0])
            );
        }
        
        bool operator()(const ucd::unichar_compose_data_entry& lft, In rgt) const
        {   
            return lft.decomp[0] > offset && lft.decomp[1+offset] < *rgt;
        }
        
        bool operator()(In lft, const ucd::unichar_compose_data_entry& rgt) const
        {
            return rgt.decomp[0] > offset && *lft < rgt.decomp[1+offset];
        }
        
    private:
        std::size_t offset;
    };
    
    struct mask_compose_data_entry
    {
        mask_compose_data_entry(unsigned mask_) : mask(mask_)
        {
        }
        
        bool operator()(const ucd::unichar_compose_data_entry& entry) const
        {
            return ((1 << ucd::get_decomposition_type(entry.ch)) & mask) != 0;
        }
        
    private:
        unsigned mask;
    };
}

/** Model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * that composes a sequence of code points, i.e. it converts a sequence
 * of code points into a single code point.
 * It applies UCD compositions that match \c mask as well as the Hangul
 * compositions, excluding the ones from the composition
 * exclusion table. */
struct composer
{
    typedef char32 input_type;
    typedef char32 output_type;
    
    composer(unsigned mask_ = BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical)) : mask(mask_)
    {
    }
    
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {
        const ucd::unichar_compose_data_entry* table_begin = ucd::__uni_compose_entry;
        const ucd::unichar_compose_data_entry* table_end = table_begin + ucd::__uni_compose_entry_size;
        
        /* We reduce the set of possible results as we consider one more
         * code point.
         * The set of possible results is contiguous (before the mask filtering) since the table is sorted */
        In pos = begin;
        std::size_t offset = 0;
        for(;;)
        {
            filter_range<
                detail::mask_compose_data_entry,
                const iterator_range<const ucd::unichar_compose_data_entry*>
            > r =
                make_filtered_range(
                    make_iterator_range(
                        std::equal_range(
                            table_begin, table_end,
                            pos,
                            detail::composition_find<In>(offset)
                        )
                    ),
                    detail::mask_compose_data_entry(mask)
                );
            
            std::ptrdiff_t sz = distance(r);
            if(sz == 1 && offset == (r.begin()->decomp[0]-1))
            {
                *out++ = r.begin()->ch;
                return std::make_pair(++pos, out);
            }
            else if(sz == 0 || ++pos == end)
            {
                return hangul_composer().ltr(begin, end, out);
            }
            
            table_begin = r.begin().base();
            table_end = r.end().base();
            ++offset;
        }
    }
    
    /* This could by made faster using a sorted table of reversed strings */
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        const ucd::unichar_compose_data_entry* table_begin = ucd::__uni_compose_entry;
        const ucd::unichar_compose_data_entry* table_end = table_begin + ucd::__uni_compose_entry_size;
        
        std::vector<const ucd::unichar_compose_data_entry*> r;
        std::size_t offset = 0;
        In pos = end;
        --pos;
        
        /* First pass, we copy the possible results into a vector */
        for(const ucd::unichar_compose_data_entry* p = table_begin; p != table_end; ++p)
        {
            if(detail::mask_compose_data_entry(mask)(*p) && p->decomp[p->decomp[0]] == *pos)
                r.push_back(p);
        }
        
        /* i-th pass, we copy the possible results into another vector then swap it
         * with the previous one */
        for(;;)
        {
            if(r.size() == 1 && offset == (r.front()->decomp[0]-1))
            {
                *out++ = r.front()->ch;
                return std::make_pair(pos, out);
            }
            else if(r.size() == 0 || pos == begin)
            {
                return hangul_composer().rtl(begin, end, out);
            }

            ++offset;
            --pos;
            std::vector<const ucd::unichar_compose_data_entry*> r2;
            typedef std::vector<const ucd::unichar_compose_data_entry*>::const_iterator iterator;
            for(iterator it = r.begin(); it != r.end(); ++it)
            {
                const ucd::unichar_compose_data_entry* p = *it;
                if(detail::mask_compose_data_entry(mask)(*p) && p->decomp[0] > offset && p->decomp[p->decomp[0]-offset] == *pos)
                    r2.push_back(p);
            }
            r.swap(r2);
        }
    }
    
private:
    unsigned mask;
};

} // namespace unicode
} // namespace boost

#endif
