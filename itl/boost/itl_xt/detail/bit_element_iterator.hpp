/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_DETAIL_BIT_ELEMENT_ITERATOR_HPP_JOFA_091109
#define BOOST_ITL_XT_DETAIL_BIT_ELEMENT_ITERATOR_HPP_JOFA_091109

#include <boost/itl_xt/bits.hpp>

namespace boost{namespace itl
{

namespace iterator_state
{
    enum { dereferencable, end };
}

//------------------------------------------------------------------------------
template<class SegmentT> struct bitwise;

template<class DomainT, ITL_COMPARE Compare, template<class, ITL_COMPARE>class Interval, class BitSetT>
struct bitwise<std::pair<Interval<DomainT,Compare>const, BitSetT> >
{
    typedef bitwise type;
    typedef DomainT domain_type;
    typedef DomainT codomain_type;
    typedef DomainT element_type;
    typedef typename BitSetT::word_type word_type;

    BOOST_STATIC_CONSTANT(bit_range_type, digits = BitSetT::digits);
};

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
    typedef typename bitwise<segment_type>::domain_type     domain_type;
    typedef typename bitwise<segment_type>::codomain_type   codomain_type;
    typedef typename bitwise<segment_type>::element_type    element_type;

private:
    struct enabler{};

public:

    bit_element_iterator()
        : _saltator(neutron<segment_iterator>::value())
        , _reptator(neutron<domain_difference_type>::value())
        , _on_bit(ante)
    {}

    explicit bit_element_iterator(segment_iterator jumper, int state = iterator_state::dereferencable)
        : _saltator(jumper)
        , _reptator(neutron<domain_difference_type>::value()) 
    {
        if(state == iterator_state::dereferencable)
            _on_bit = biterate::proceed<word_type,digits>::next(jumper->second.word(), ante);
    }

    template <class SaltatorT>
    bit_element_iterator
        ( bit_element_iterator<SaltatorT> const& other
        , typename enable_if<boost::is_convertible<SaltatorT*,SegmentIteratorT*>, enabler>::type = enabler())
        : _saltator(other._saltator)
        , _reptator(other._reptator) 
        , _on_bit(other._on_bit) 
    {}

private:
    typedef typename bitwise<segment_type>::word_type word_type;
    BOOST_STATIC_CONSTANT(bit_range_type, digits = bitwise<segment_type>::digits );
    BOOST_STATIC_CONSTANT(bit_range_type, shift  = log2_<digits>::value );
    BOOST_STATIC_CONSTANT(bit_range_type, past   = digits );
    BOOST_STATIC_CONSTANT(bit_range_type, ante   = -1     );

    friend class boost::iterator_core_access;
    template <class> friend class bit_element_iterator;

    template <class SaltatorT>
    bool equal(bit_element_iterator<SaltatorT> const& other) const
    {
        return this->_saltator == other._saltator
            && this->_reptator == other._reptator;
    }

    void increment()
    { 
        if(!next_bit()) // find and update the next on bit.
        {
            if(_reptator < pred(_saltator->first.length()))
            {
                ++_reptator; 
                _on_bit = ante;
            }
            else
            {
                ++_saltator;
                _reptator = neutron<domain_difference_type>::value();
                _on_bit = ante;
            }
            // The existence of at least one bit is asserted (absorber trait)
        }
    }

    void decrement()
    { 
        if(!prev_bit()) // find and update the previous on bit.
        {
            if(neutron<domain_difference_type>::value() < _reptator)
                --_reptator; 
            else
            {
                --_saltator;
                _reptator = _saltator->first.length();
                --_reptator;
            }
            // The existence of at least one bit is asserted (absorber trait)
            _on_bit = past;
            next_bit();
        }
    }

    element_type dereference()const
    {
        return (_saltator->first.first() << shift) + _reptator + _on_bit;
    }

    bool next_bit()
    {
        BOOST_ASSERT(ante <= _on_bit && _on_bit < past);
        _on_bit = biterate::proceed<word_type,digits>::next(_saltator->second.word(), _on_bit);
        return _on_bit != past;
    }

    bool prev_bit()
    {
        BOOST_ASSERT(ante < _on_bit && _on_bit <= past);
        _on_bit = biterate::proceed<word_type,digits>::prev(_saltator->second.word(), _on_bit);
        return _on_bit != ante;
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

#if (defined _MSC_VER)
# pragma warning( push )
// C4146: unary minus operator applied to unsigned type, result still unsigned
# pragma warning( disable : 4146 )
#endif


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
struct proceed
{
    typedef nat32                  word_type;
    typedef proceed<WordT, Digits> type;

    BOOST_STATIC_CONSTANT( word_type, digits    = Digits                    );
    BOOST_STATIC_CONSTANT( word_type, w1        = static_cast<word_type>(1) );
    BOOST_STATIC_CONSTANT( word_type, top       = w1 << (digits-w1)         );
    BOOST_STATIC_CONSTANT( word_type, factor    = 0x077CB531U               );
    BOOST_STATIC_CONSTANT( word_type, shift     = 27                        );
    BOOST_STATIC_CONSTANT( bit_range_type, ante = -1                        );
    BOOST_STATIC_CONSTANT( bit_range_type, past = digits                    );
    BOOST_STATIC_CONSTANT( bit_range_type, last = past-1                    );

    static bit_range_type next(word_type value, bit_range_type cur_pos)
    {
        BOOST_ASSERT( ante <= cur_pos && cur_pos < past );

        if(cur_pos == last)
            return past;
        else if(cur_pos == ante)
            return index32[(((value & -value) * factor)) >> shift];
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

    static bit_range_type prev(word_type value, bit_range_type cur_pos)
    {
        BOOST_ASSERT(ante < cur_pos && cur_pos <= past);

        if(cur_pos == 0)
            return ante;
        else if(cur_pos == past)
             return index32[((high_bit(value) * factor)) >> shift];
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


template<> struct proceed<nat64, 64>
{
    typedef nat64                 word_type;
    typedef nat32                 half_type;
    typedef proceed<word_type,64> type;

    BOOST_STATIC_CONSTANT( word_type, digits    = 64                        );
    BOOST_STATIC_CONSTANT( word_type, w1        = static_cast<word_type>(1) );
    BOOST_STATIC_CONSTANT( word_type, top       = w1 << (digits-w1)         );
    BOOST_STATIC_CONSTANT( half_type, factor    = 0x077CB531U               );
    BOOST_STATIC_CONSTANT( half_type, shift     = 27                        );
    BOOST_STATIC_CONSTANT( bit_range_type, ante = -1                        );
    BOOST_STATIC_CONSTANT( bit_range_type, past = digits                    );
    BOOST_STATIC_CONSTANT( bit_range_type, last = past-1                    );
    BOOST_STATIC_CONSTANT( bit_range_type, half = 32                        );

    BOOST_STATIC_CONSTANT( word_type, lower_mask = (w1 << 32)-w1            );


    static half_type lower_next(word_type value, bit_range_type cur_pos)
    {
        return static_cast<half_type>(lower_mask & (value & ~((1<<(cur_pos+1))-1)));
    }

    static half_type upper_next(word_type value, bit_range_type cur_pos)
    {
        return static_cast<half_type>(value >> half) & ~((1<<(cur_pos-half+1))-1);
    }

    static bit_range_type next(word_type value, bit_range_type cur_pos)
    {
        BOOST_ASSERT(cur_pos < past );

        if(cur_pos == last)
            return past;
        else if(cur_pos == ante)
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
                else
                {
                    half_type up_next = upper_next(value, cur_pos);
                    return half + index32[(((up_next & -up_next) * factor)) >> shift];
                }
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

    static bit_range_type prev(word_type value, bit_range_type cur_pos)
    {
        BOOST_ASSERT(ante < cur_pos);

        if(cur_pos == 0)
            return ante;
        else if(cur_pos == past)
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
        else
        {
            half_type up_prev, low_prev;
            if(half == cur_pos)
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

#if (defined _MSC_VER)
# pragma warning( pop ) // C4146: unary minus operator applied to unsigned type,
                        // result still unsigned
#endif

} //namespace biterate

}} // namespace itl boost

#endif // BOOST_ITL_XT_DETAIL_BIT_ELEMENT_ITERATOR_HPP_JOFA_091109



