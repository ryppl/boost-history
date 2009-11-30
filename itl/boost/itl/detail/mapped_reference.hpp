/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_DETAIL_MAPPED_REFERENCE_HPP_JOFA_091108
#define BOOST_ITL_DETAIL_MAPPED_REFERENCE_HPP_JOFA_091108

#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

namespace boost{namespace itl
{

template<class FirstT, class SecondT>
struct mapped_reference
{
    typedef FirstT  first_type;   
    typedef SecondT second_type; 
    typedef mapped_reference type;
    typedef typename 
        mpl::if_<is_const<second_type>, 
                       second_type&, 
                 const second_type&>::type second_reference_type;

    const FirstT&         first;
    second_reference_type second;

    mapped_reference(const FirstT& fst, second_reference_type snd) : first(fst), second(snd){}

    template<class FstT, class SndT>
    mapped_reference(const mapped_reference<FstT, SndT>& source):
        first(source.first), second(source.second){}

    template<class FstT, class SndT>
    operator std::pair<FstT,SndT>(){ return std::pair<FstT,SndT>(first, second); }
};


template<class FirstT, class SecondT>
inline bool operator == (const mapped_reference<FirstT, SecondT>& left, const mapped_reference<FirstT, SecondT>& right)
{ return left.first == right.first && left.second == right.second; }

template<class FirstT, class SecondT>
inline bool operator < (const mapped_reference<FirstT, SecondT>& left, const mapped_reference<FirstT, SecondT>& right)
{ return left.first < right.first || (!(right.first < left.first) && left.second < right.second); }

template<class FirstT, class SecondT>
inline bool operator != (const mapped_reference<FirstT, SecondT>& left, const mapped_reference<FirstT, SecondT>& right)
{ return !(left == right); }

template<class FirstT, class SecondT>
inline bool operator > (const mapped_reference<FirstT, SecondT>& left, const mapped_reference<FirstT, SecondT>& right)
{ return right < left; }

template<class FirstT, class SecondT>
inline bool operator <= (const mapped_reference<FirstT, SecondT>& left, const mapped_reference<FirstT, SecondT>& right)
{ return !(right < left); }

template<class FirstT, class SecondT>
inline bool operator >= (const mapped_reference<FirstT, SecondT>& left, const mapped_reference<FirstT, SecondT>& right)
{ return !(left < right); }

template<class FirstT, class SecondT>
inline mapped_reference<FirstT, SecondT> make_mapped_reference(const FirstT& left, SecondT& right)
{ return mapped_reference<FirstT, SecondT>(left, right); }

}} // namespace itl boost

#endif // BOOST_ITL_DETAIL_MAPPED_REFERENCE_HPP_JOFA_091108
