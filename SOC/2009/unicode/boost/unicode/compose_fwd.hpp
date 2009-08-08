#ifndef BOOST_UNICODE_COMPOSE_FWD_HPP
#define BOOST_UNICODE_COMPOSE_FWD_HPP

#include <boost/unicode/ucd/properties.hpp>
#include <boost/unicode/hangul.hpp>

#include <boost/integer/static_pow.hpp>
#include <climits>

#include <boost/iterator/pipe_iterator.hpp>
#include <vector>

namespace boost
{
namespace unicode
{

/** Computes 2 elevated to power \c o so as to use the option within bit masks. */
#define BOOST_UNICODE_OPTION(o) (boost::static_pow<2, (o)>::value)
#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
#undef BOOST_UNICODE_OPTION
#endif

/** Model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * that decomposes a combining character sequence, i.e. it transforms a combining
 * character sequence into its canonically ordered decomposed equivalent.
 * It applies UCD decompositions that match \c mask recursively as well as the Hangul decompositions
 * if \c mask matches canonical decompositions. */
struct decomposer
{
    typedef char32 input_type;
    typedef char32 output_type;
    
    decomposer(unsigned mask_ = BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical)) : mask(mask_)
    {
    }
    
    /** \post \c out is in Normalization Form D. */
    template<typename In, typename Out>
    std::pair<In, Out> ltr(In begin, In end, Out out)
    {
        do
        {
            char32 ch = *begin;
            if(ucd::get_combining_class(ch) != 0)
            {
                // canonical reorder, not handled yet
            }
        
            iterator_range<const char32*> dec = ucd::get_decomposition(ch);
            if(!empty(dec) && ((1 << ucd::get_decomposition_type(ch)) & mask))
            {
                out = pipe(dec, *this, out); // we decompose recursively
            }
            else if(BOOST_UNICODE_OPTION(ucd::decomposition_type::canonical) & mask)
            {
                out = hangul_decomposer()(ch, out);
            }
            else
            {
                *out++ = ch;
            }
            
            ++begin;
        }
        while(begin != end && ucd::get_combining_class(*begin) != 0);
        
        return std::make_pair(begin, out);
    }
    
    /** \post \c out is in Normalization Form D. */
    template<typename In, typename Out>
    std::pair<In, Out> rtl(In begin, In end, Out out)
    {
        // NOT IMPLEMENTED
        *out++ = *--end;
        return std::make_pair(end, out);
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
}

/** Model of \c \xmlonly<conceptname>Pipe</conceptname>\endxmlonly
 * that composes a sequence of code points, i.e. it converts a sequence
 * of code points into a single code point.
 * It applies UCD canonical compositions as well as the Hangul
 * compositions, excluding the ones from the composition
 * exclusion table. */
struct composer
{
    typedef char32 input_type;
    typedef char32 output_type;
    typedef mpl::int_<1> max_output;
    
    /** \pre [<tt>begin</tt>, <tt>end</tt>[ is in Normalization Form D. */
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
            const iterator_range<const ucd::unichar_compose_data_entry*> r =
                std::equal_range(
                    table_begin, table_end,
                    pos,
                    detail::composition_find<In>(offset)
                );
            
            ++pos;
            std::size_t sz = size(r);
            if(sz == 0) // no possible match
            {
                return hangul_composer().ltr(begin, end, out);
            }
            else if( (sz == 1 || pos == end) && offset == (r.begin()->decomp[0]-1)) // a complete match was found
            {
                *out++ = r.begin()->ch;
                return std::make_pair(pos, out);
            }
            else if(pos == end) // some possible matches but none complete
            {
                return hangul_composer().ltr(begin, end, out);
            }
            
            table_begin = r.begin();
            table_end = r.end();
            ++offset;
        }
    }
    
    /* This could by made faster using a sorted table of reversed strings */
    /** \pre [<tt>begin</tt>, <tt>end</tt>[ is in Normalization Form D. */
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
            if(p->decomp[p->decomp[0]] == *pos)
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
                if(
                    p->decomp[0] > offset &&
                    (pos == begin ? p->decomp[0] == offset+1 : true) && // if we are at the beginning, we only consider matches with the right size.
                    p->decomp[p->decomp[0]-offset] == *pos
                )
                    r2.push_back(p);
            }
            r.swap(r2);
        }
    }
};

} // namespace unicode
} // namespace boost

#endif
