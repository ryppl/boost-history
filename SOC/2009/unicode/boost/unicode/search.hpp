#ifndef BOOST_UNICODE_SEARCH_HPP
#define BOOST_UNICODE_SEARCH_HPP

#include <boost/range.hpp>
#include <boost/algorithm/string/compare.hpp>

namespace boost
{

namespace algorithm
{

template<typename F, typename B>
struct boundary_finder
{
    boundary_finder(F f_, B b_) : f(f_), b(b_)
    {
    }
    
    template<typename Iterator>
    iterator_range<Iterator> operator()(Iterator begin, Iterator end) const
    {
        iterator_range<Iterator> range;
        Iterator curr_begin = begin;
        for(;;)
        {
            range = f(curr_begin, end);
            if(
                empty(range)
             || ((range.begin() == begin || b(begin, end, range.begin()))
                 && (range.end() == end || b(begin, end, range.end())))
            )
                return range;
            
            curr_begin = range.begin();
            ++curr_begin;
        }
    }
    
private:
    mutable F f;
    mutable B b;
};

template<typename F, typename B>
boundary_finder<F, B> make_boundary_finder(F f, B b)
{
    return boundary_finder<F, B>(f, b);
}

} // namespace algorithm

namespace unicode
{
    
template<typename Match, typename Comp = is_equal>
struct simple_finder
{
    simple_finder(const Match& match_, const Comp& comp_ = Comp()) : match(match_), comp(comp_)
    {
    }
    
    template<typename Iterator>
    iterator_range<Iterator> ltr(Iterator begin, Iterator end)
    {
        typedef typename range_iterator<const Match>::type Iterator_match;
    
        for(Iterator it = begin; it != end; ++it)
        {
            Iterator it_curr = it;
            Iterator_match it_match = boost::begin(match);
            for(; it_match != boost::end(match) && it_curr != end; ++it_match, ++it_curr)
            {
                if(!comp(*it_match, *it_curr))
                    break;
            }
    
            if(it_match == boost::end(match))
                return make_iterator_range(it, it_curr);
        }
        return make_iterator_range(end, end);
    }
    
    template<typename Iterator>
    iterator_range<Iterator> rtl(Iterator begin, Iterator end)
    {
        typedef typename range_iterator<const Match>::type Iterator_match;
    
        for(Iterator it = end; ; --it)
        {
            Iterator it_curr = it;
            Iterator_match it_match = boost::end(match);
            for(; ; --it_match, --it_curr)
            {
                if(!comp(*it_match, *it_curr))
                    break;
                    
                if(it_match == boost::begin(match) || it_curr == begin)
                    break;
            }
    
            if(it_match == boost::begin(match))
                return make_iterator_range(it_curr, it);
                
            if(it == begin)
                break;
        }
        return make_iterator_range(begin, begin);
    }
    
private:
    const Match& match;
    Comp comp;
};

template<typename Match, typename Comp>
simple_finder<Match, Comp> make_simple_finder(const Match& match, Comp comp = Comp())
{
    return simple_finder<Match, Comp>(match, comp);
}

template<typename Match>
simple_finder<Match> make_simple_finder(const Match& match)
{
    return simple_finder<Match>(match);
}

template<typename F, typename B>
struct boundary_finder
{
    boundary_finder(F f_, B b_) : f(f_), b(b_)
    {
    }
    
    template<typename Iterator>
    iterator_range<Iterator> ltr(Iterator begin, Iterator end)
    {
        iterator_range<Iterator> range;
        Iterator curr_begin = begin;
        for(;;)
        {
            range = f.ltr(curr_begin, end);
            if(
                empty(range)
             || ((range.begin() == begin || b(begin, end, range.begin()))
                 && (range.end() == end || b(begin, end, range.end())))
            )
                return range;
            
            curr_begin = range.begin();
            ++curr_begin;
        }
    }
    
    template<typename Iterator>
    iterator_range<Iterator> rtl(Iterator begin, Iterator end)
    {
        iterator_range<Iterator> range;
        Iterator curr_end = end;
        for(;;)
        {
            range = f.rtl(begin, curr_end);
            if(
                empty(range)
             || ((range.begin() == begin || b(begin, end, range.begin()))
                 && (range.end() == end || b(begin, end, range.end())))
            )
                return range;
            
            curr_end = range.end();
            --curr_end;
        }
    }
    
private:
    F f;
    B b;
};

template<typename F, typename B>
boundary_finder<F, B> make_boundary_finder(F f, B b)
{
    return boundary_finder<F, B>(f, b);
}

} // namespace unicode

} // namespace boost

#endif
