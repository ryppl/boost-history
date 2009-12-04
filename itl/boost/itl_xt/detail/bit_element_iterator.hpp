/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
Copyright (c) 1998: Charles E. Leiserson, Harald Prokop, Keith H. Randall
  (Bit sequencing using DeBruin sequence 
  http://supertech.csail.mit.edu/papers/debruijn.pdf)
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_DETAIL_BIT_ELEMENT_ITERATOR_HPP_JOFA_091109
#define BOOST_ITL_XT_DETAIL_BIT_ELEMENT_ITERATOR_HPP_JOFA_091109

#include <boost/itl_xt/bits.hpp>
#include <boost/mpl/not.hpp>

namespace boost{namespace itl
{

//------------------------------------------------------------------------------
template<class SegmentT> struct bitwise;

template<class DomainT, ITL_COMPARE Compare, template<class, ITL_COMPARE>class Interval, class BitSetT>
struct bitwise<std::pair<Interval<DomainT,Compare>const, BitSetT> >
{
    typedef bitwise type;
    typedef DomainT domain_type;
    typedef DomainT codomain_type;
    typedef DomainT element_type;
    typedef BitSetT                     bitset_type;
    typedef typename BitSetT::word_type word_type;

    BOOST_STATIC_CONSTANT(bit_range_type, digits = BitSetT::digits);
};

namespace biterate
{ template<class IteratorT, bool Forward> struct proceed; }

template <class SegmentIteratorT>
class bit_element_iterator
  : public boost::iterator_facade<
          bit_element_iterator<SegmentIteratorT>
        , typename bitwise<typename SegmentIteratorT::value_type>::element_type
        , boost::bidirectional_traversal_tag
        , typename bitwise<typename SegmentIteratorT::value_type>::element_type
    >
{
public:
    typedef SegmentIteratorT                                segment_iterator;
    typedef typename SegmentIteratorT::value_type           segment_type;
    typedef typename first_element<segment_type>::type      interval_type;
    typedef typename interval_type::difference_type         domain_difference_type;
    typedef typename bitwise<segment_type>::bitset_type     bitset_type;
    typedef typename bitwise<segment_type>::word_type       word_type;
    typedef typename bitwise<segment_type>::domain_type     domain_type;
    typedef typename bitwise<segment_type>::codomain_type   codomain_type;
    typedef typename bitwise<segment_type>::element_type    element_type;
    typedef element_type                                    value_type;
    typedef element_type                                    key_type;
    typedef element_type                                    data_type;

    BOOST_STATIC_CONSTANT(bit_range_type, digits = bitwise<segment_type>::digits );
    BOOST_STATIC_CONSTANT(bool,           is_forward  = mpl::not_<is_reverse<SegmentIteratorT> >::value );

    typedef typename biterate::proceed<segment_iterator,is_forward> Proceed;

    BOOST_STATIC_CONSTANT(bit_range_type, before = Proceed::before );
    BOOST_STATIC_CONSTANT(bit_range_type, ante   = Proceed::ante   );
    BOOST_STATIC_CONSTANT(bit_range_type, past   = Proceed::past   );
    BOOST_STATIC_CONSTANT(bit_range_type, beyond = Proceed::beyond );

private:
    struct enabler{};

public:

    bit_element_iterator()
        : _saltator(neutron<segment_iterator>::value())
        , _reptator(neutron<domain_difference_type>::value())
        , _on_bit(before)
    {}

    bit_element_iterator(segment_iterator jumper, bit_range_type start_pos)
        : _saltator(jumper)
        , _reptator(neutron<domain_difference_type>::value()) 
        , _on_bit(start_pos)
    {}

    template <class SaltatorT>
    bit_element_iterator
        ( bit_element_iterator<SaltatorT> const& other
        , typename enable_if<boost::is_convertible<SaltatorT*,SegmentIteratorT*>, enabler>::type = enabler())
        : _saltator(other._saltator)
        , _reptator(other._reptator) 
        , _on_bit(other._on_bit) 
    {}

private:
    BOOST_STATIC_CONSTANT(bit_range_type, shift       = log2_<digits>::value );

    friend class boost::iterator_core_access;
    template <class> friend class bit_element_iterator;

    template <class SaltatorT>
    bool equal(bit_element_iterator<SaltatorT> const& other) const
    {
        if(     _saltator != other._saltator)
            return false;
        else if(_reptator != other._reptator)
            return false;
        else if(_on_bit == ante)
            return other._on_bit == beyond || other.is_segment_begin();
        else if(ante == other._on_bit)
            return _on_bit == beyond || is_segment_begin();
        else 
            return _on_bit == other._on_bit;
    }

    void increment()
    { 
        if(_on_bit == ante)
            first_bit();
        
        next_bit();
        if(_on_bit == past)
        {   // The current bitset passed the end
            if(_reptator < pred(_saltator->first.length()))
            {
                ++_reptator; 
                _on_bit = ante;
            }
            else
            {
                ++_saltator;
                // NOTE: increment and dereference are unsecure now, because 
                // _saltator can (finally) be end here. decrement is secure. 
                _reptator = neutron<domain_difference_type>::value();
                _on_bit = ante;
            }
        }
    }

    void decrement()
    { 
        prev_bit();
        if(_on_bit == ante)
        {   // The current bitset passed the beginning
            if(neutron<domain_difference_type>::value() < _reptator)
            {
                --_reptator;
                _on_bit = past;
            }
            else
            {
                //assert: _saltator is not at begin here.
                --_saltator;
                // NOW: decrement insecure, because _saltator can (finally) be 
                // begin here. increment and dereference are secure.
                _reptator = _saltator->first.length();
                --_reptator;
                _on_bit = past;
            }
        }
    }

    element_type dereference()const
    {
        if(_on_bit == ante)
            first_bit();
        else if(_on_bit == past)
            last_bit();

        return (Proceed::inter_value(_reptator, _saltator->first) << shift) + _on_bit;
    }

    void first_bit()const
    {
        // Because interval_bitsets are neutron absorbers, _saltator->second.word() is not a neutron
        _on_bit   = Proceed::first_bit(_saltator->second.word());
    }

    void last_bit()const
    {
        // Because interval_bitsets are neutron absorbers, _saltator->second.word() is not a neutron
        _on_bit   = Proceed::last_bit(_saltator->second.word());
    }

    bool next_bit()
    {
        if(_on_bit == before)
        {
            ++_saltator;
            _reptator = neutron<domain_difference_type>::value();
            _on_bit   = Proceed::first_bit(_saltator->second.word());
        }
        else
            _on_bit = Proceed::next_bit(_saltator->second.word(), _on_bit);

        return _on_bit != past;
    }

    bool prev_bit()
    {
        if(_on_bit == beyond)
        {
            --_saltator;
            _reptator = _saltator->first.length();
            --_reptator;
            _on_bit   = Proceed::last_bit(_saltator->second.word());
        }
        else
            _on_bit = Proceed::prev_bit(_saltator->second.word(), _on_bit);

        return _on_bit != ante;
    }

    bool is_segment_begin()const
    {
        return _reptator == neutron<domain_difference_type>::value()
            && 0 <= _on_bit && _on_bit < digits
            && _on_bit == Proceed::first_bit(_saltator->second.word());
    }

private:
    segment_iterator               _saltator;  // satltare: to jump  : the fast moving iterator
    mutable domain_difference_type _reptator;  // reptare:  to sneak : the slow moving iterator 0 based
    mutable bit_range_type         _on_bit;    // position of the current on bit.
                                               //     -1 <= _on_bit <= digits
                                               // rend()               end()   

};

namespace biterate
{

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4706) // assignment within conditional expression
#pragma warning(disable:4146) // unary minus operator applied to unsigned type, result still unsigned
#endif                        


/* Bit-iteration is coded on the bases of the work of
Charles E. Leiserson, Harald Prokop, Keith H. Randall
  http://supertech.csail.mit.edu/papers/debruijn.pdf

and also Sean Eron Anderson:
http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
*/

//DeBruin sequence
static unsigned char index32[] =
{
   0,  1, 28,  2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17,  4, 8, 
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11,  5, 10, 9
};

inline static nat32 high_bit(nat32 v)
{
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return (v >> 1) + 1;
}

template<typename WordT, bit_range_type Digits> 
struct forward
{
    typedef nat32                  word_type;
    typedef forward<WordT, Digits> type;

    BOOST_STATIC_CONSTANT( word_type,      digits = Digits                    );
    BOOST_STATIC_CONSTANT( word_type,      w1     = static_cast<word_type>(1) );
    BOOST_STATIC_CONSTANT( word_type,      top    = w1 << (digits-w1)         );
    BOOST_STATIC_CONSTANT( word_type,      factor = 0x077CB531U               );
    BOOST_STATIC_CONSTANT( word_type,      shift  = 27                        );
    BOOST_STATIC_CONSTANT( bit_range_type, ante   = -1                        );
    BOOST_STATIC_CONSTANT( bit_range_type, past   = digits                    );
    BOOST_STATIC_CONSTANT( bit_range_type, last   = past-1                    );

    static bit_range_type first_bit(word_type value)
    {
        return index32[(((value & -value) * factor)) >> shift];
    }

    static bit_range_type next_bit(word_type value, bit_range_type cur_pos)
    {
        if(cur_pos == last)
            return past;
        else
        {
            // next contains all bits of value above cur_pos
            word_type next = value & ~((1<<(cur_pos+1))-1);
            if(!next)
                return past;
            else
                return index32[(((next & -next) * factor)) >> shift];
        }
    }

    static bit_range_type last_bit(word_type value)
    {
        return index32[((high_bit(value) * factor)) >> shift];
    }

    static bit_range_type prev_bit(word_type value, bit_range_type cur_pos)
    {
        if(cur_pos == 0)
            return ante;
        else
        {
            // next contains all bits of value above cur_pos
            word_type prev = value & ((1<<cur_pos)-1);
            if(!prev)
                return ante;
            else
                return index32[((high_bit(prev) * factor)) >> shift];
        }
    }
};


template<> struct forward<nat64, 64>
{
    typedef nat64                 word_type;
    typedef nat32                 half_type;
    typedef forward<word_type,64> type;

    BOOST_STATIC_CONSTANT( word_type, digits     = 64                        );
    BOOST_STATIC_CONSTANT( word_type, w1         = static_cast<word_type>(1) );
    BOOST_STATIC_CONSTANT( word_type, top        = w1 << (digits-w1)         );
    BOOST_STATIC_CONSTANT( half_type, factor     = 0x077CB531U               );
    BOOST_STATIC_CONSTANT( half_type, shift      = 27                        );
    BOOST_STATIC_CONSTANT( bit_range_type, ante  = -1                        );
    BOOST_STATIC_CONSTANT( bit_range_type, past  = digits                    );
    BOOST_STATIC_CONSTANT( bit_range_type, last  = past-1                    );
    BOOST_STATIC_CONSTANT( bit_range_type, half  = 32                        );
    BOOST_STATIC_CONSTANT( word_type, lower_mask = (w1 << 32)-w1            );


    static half_type lower_next(word_type value, bit_range_type cur_pos)
    {
        return static_cast<half_type>(lower_mask & (value & ~((1<<(cur_pos+1))-1)));
    }

    static half_type upper_next(word_type value, bit_range_type cur_pos)
    {
        return static_cast<half_type>(value >> half) & ~((1<<(cur_pos-half+1))-1);
    }

    static bit_range_type first_bit(word_type value)
    {
        half_type lower = static_cast<half_type>(lower_mask & value);
        if(lower)
            return index32[(((lower & -lower) * factor)) >> shift];
        else
        {
            half_type upper = static_cast<half_type>(value >> half);
            return half + index32[(((upper & -upper) * factor)) >> shift];
        }
    }


    static bit_range_type next_bit(word_type value, bit_range_type cur_pos)
    {
        if(cur_pos == last)
            return past;
        else
        {
            half_type low_next, up_next;
            if(half-1 <= cur_pos)
                if(up_next = upper_next(value, cur_pos))
                    return half + index32[(((up_next & -up_next) * factor)) >> shift];
                else
                    return past;
            else
                if(low_next = lower_next(value, cur_pos))
                    return index32[(((low_next & -low_next) * factor)) >> shift];
                else if(half_type up_next = upper_next(value, cur_pos))
                    return half + index32[(((up_next & -up_next) * factor)) >> shift];
                else
                    return past;
        }
    }

    static half_type lower_previous(word_type value, bit_range_type cur_pos)
    {
        return static_cast<half_type>(lower_mask & (value & ((1<<(cur_pos))-1)));
    }

    static half_type upper_previous(word_type value, bit_range_type cur_pos)
    {
        return static_cast<half_type>(value >> half) & ((1<<(cur_pos-half))-1);
    }

    static bit_range_type last_bit(word_type value)
    {
        half_type upper = static_cast<half_type>(value >> half);
        if(upper)
            return half + index32[((high_bit(upper) * factor)) >> shift];
        else
        {
            half_type lower = static_cast<half_type>(value & lower_mask);
            return index32[((high_bit(lower) * factor)) >> shift];
        }
    }

    static bit_range_type prev_bit(word_type value, bit_range_type cur_pos)
    {
        if(cur_pos == 0)
            return ante;
        else
        {
            half_type up_prev, low_prev;
            if(half == cur_pos)
                // warning C4706: assignment within conditional expression
                // This is intentional here.
                if(low_prev = static_cast<half_type>(lower_mask & value))
                    return index32[((high_bit(low_prev) * factor)) >> shift];
                else
                    return ante;
            else if(cur_pos < half || !(up_prev = upper_previous(value, cur_pos)))
                if(low_prev = lower_previous(value,cur_pos))
                    return index32[((high_bit(low_prev) * factor)) >> shift];
                else
                    return ante;
            else
                return half + index32[((high_bit(up_prev) * factor)) >> shift];
        }
    }
};

template<class IteratorT, bool Forward> 
struct proceed{};

template<class IteratorT>
struct proceed<IteratorT,true>
{
    typedef IteratorT                                    iterator;
    typedef typename iterator::value_type                segment_type;
    typedef typename first_element<segment_type>::type   interval_type;
    typedef typename interval_type::difference_type      difference_type;
    typedef typename bitwise<segment_type>::word_type    word_type;

    BOOST_STATIC_CONSTANT(bit_range_type, digits = bitwise<segment_type>::digits );
    BOOST_STATIC_CONSTANT(bit_range_type, before = -2       );
    BOOST_STATIC_CONSTANT(bit_range_type, ante   = -1       );
    BOOST_STATIC_CONSTANT(bit_range_type, past   = digits   );
    BOOST_STATIC_CONSTANT(bit_range_type, beyond = digits+1 );

    static bit_range_type first_bit(word_type value)
    { return forward<word_type,digits>::first_bit(value); }

    static bit_range_type last_bit(word_type value)
    { return forward<word_type,digits>::last_bit(value); }

    static bit_range_type next_bit(word_type value, bit_range_type cur_pos)
    { return forward<word_type,digits>::next_bit(value, cur_pos); }

    static bit_range_type prev_bit(word_type value, bit_range_type cur_pos)
    { return forward<word_type,digits>::prev_bit(value, cur_pos); }

    static difference_type inter_value(difference_type reptator, const interval_type inter_val)
    { return inter_val.first() + reptator; }

    static difference_type inter_base(const iterator& iter)
    { return neutron<difference_type>::value(); }

    static difference_type inter_ceil(const iterator& iter)
    { return iter->first.length(); }
};

template<class IteratorT>
struct proceed<IteratorT,false>
{
    typedef IteratorT                                    iterator;
    typedef typename iterator::value_type                segment_type;
    typedef typename first_element<segment_type>::type   interval_type;
    typedef typename interval_type::difference_type      difference_type;
    typedef typename bitwise<segment_type>::word_type    word_type;

    BOOST_STATIC_CONSTANT(bit_range_type, digits = bitwise<segment_type>::digits );
    BOOST_STATIC_CONSTANT(bit_range_type, beyond = -2       );
    BOOST_STATIC_CONSTANT(bit_range_type, past   = -1       );
    BOOST_STATIC_CONSTANT(bit_range_type, ante   = digits   );
    BOOST_STATIC_CONSTANT(bit_range_type, before = digits+1 );

    static bit_range_type first_bit(word_type value)
    { return forward<word_type,digits>::last_bit(value); }

    static bit_range_type last_bit(word_type value)
    { return forward<word_type,digits>::first_bit(value); }

    static bit_range_type next_bit(word_type value, bit_range_type cur_pos)
    { return forward<word_type,digits>::prev_bit(value, cur_pos); }

    static bit_range_type prev_bit(word_type value, bit_range_type cur_pos)
    { return forward<word_type,digits>::next_bit(value, cur_pos); }

    static difference_type inter_value(difference_type reptator, const interval_type inter_val)
    { return inter_val.last() - reptator; }
};

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

} //namespace biterate

}} // namespace itl boost

#endif // BOOST_ITL_XT_DETAIL_BIT_ELEMENT_ITERATOR_HPP_JOFA_091109



